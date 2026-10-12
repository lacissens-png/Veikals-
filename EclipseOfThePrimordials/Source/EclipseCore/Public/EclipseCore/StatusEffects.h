// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// StatusEffects.h
// -----------------------------------------------------------------------------
// Status effect data model, the boss Stagger Meter, and the single application
// pipeline every status passes through (players, swarm mobs and bosses alike).
//
// MEMORY / PERFORMANCE
//   StatusContainer is a fixed-size array indexed by EStatusEffect: no heap
//   allocation, no hashing, ~130 bytes. The swarm keeps one per mob in a
//   contiguous TArray, so ticking 100-500 mobs' statuses is a linear,
//   cache-friendly sweep.
//
// PIPELINE (ApplyStatusEffect)
//
//   StatusApplication ──► is CC? ──no──► ApplyRaw (stack policy) ──► outcome
//                            │
//                           yes
//                            │
//               tier converts CC to stagger? (Boss)
//                 │yes                         │no
//                 ▼                            ▼
//   ApplyCrowdControlToBossStagger      target Unstoppable? ──yes──► Immune
//     │ filled        │ broke                   │no
//     ▼               ▼                         ▼
//  ConvertedTo-   force Stun +            scale duration by tier
//  Stagger        Vulnerable              (Captain 75%, Brute 50%) ► ApplyRaw
// -----------------------------------------------------------------------------
#pragma once

#include "EclipseCore/EclipseCoreTypes.h"

#include <array>

namespace Eclipse
{
	/** Keep in sync with EEclipseStatusEffect (UE mirror) - static_asserted there. */
	enum class EStatusEffect : u8
	{
		Vulnerable,   // Debuff: target takes +Magnitude (>= 20%) increased damage.
		Unstoppable,  // Buff: immune to crowd control; applying it purges active CC.
		Bleed,        // DoT: stacks intensity (each stack adds Magnitude DPS).
		Burn,         // DoT: refreshes duration, keeps strongest Magnitude.
		Freeze,       // CC: full disable (no move / no act).
		Stun,         // CC: full disable.
		Slow,         // CC: Magnitude = fraction of move speed removed.
		Knockback,    // CC: instantaneous displacement; Magnitude = impulse.
		Count
	};
	constexpr std::size_t StatusEffectCount = Idx(EStatusEffect::Count);

	/** Gameplay tier. Drives CC resistance and whether CC converts into stagger. */
	enum class EMobTier : u8
	{
		TrashSwarmer,
		Captain,          // Elites: project aura modifiers to nearby swarmers.
		PrimordialBrute,  // Heavy: halved CC, immune to knockback.
		Boss,             // CC never disables; fills the Stagger Meter instead.
		Player,
		Count
	};

	constexpr bool IsCrowdControl(EStatusEffect E)
	{
		return E == EStatusEffect::Freeze || E == EStatusEffect::Stun
			|| E == EStatusEffect::Slow || E == EStatusEffect::Knockback;
	}

	constexpr bool IsDamageOverTime(EStatusEffect E)
	{
		return E == EStatusEffect::Bleed || E == EStatusEffect::Burn;
	}

	/** A request to apply a status. Constructed by abilities / runes / mob attacks. */
	struct StatusApplication
	{
		EStatusEffect Effect = EStatusEffect::Vulnerable;
		float Duration = 0.f;
		float Magnitude = 0.f;
		u32 SourceId = 0;            // Opaque instigator id (net id / player index) for attribution.
		bool bIgnoreImmunity = false; // Internal: stagger break forces Stun through Unstoppable.
	};

	struct StatusSlot
	{
		float Remaining = 0.f;
		float Magnitude = 0.f;
		u8 Stacks = 0;
		u32 SourceId = 0;

		bool IsActive() const { return Remaining > 0.f; }
	};

	enum class EApplyResult : u8
	{
		Applied,             // Newly applied.
		Refreshed,           // Already active; duration/magnitude refreshed.
		Stacked,             // Intensity stack added (Bleed).
		ResistedByImmunity,  // Target is Unstoppable.
		ConvertedToStagger,  // Boss: CC filled stagger meter instead of disabling.
		StaggerBroken,       // Boss: this CC filled the meter -> boss is now stunned & vulnerable.
		Rejected             // Invalid request (zero duration, boss already staggered, ...).
	};

	struct ApplyOutcome
	{
		EApplyResult Result = EApplyResult::Rejected;
		float EffectiveDuration = 0.f;
		float StaggerAdded = 0.f;

		/** True if the application "landed" from a gameplay perspective (used for OnCrowdControlApplied rituals). */
		bool Landed() const
		{
			return Result == EApplyResult::Applied || Result == EApplyResult::Refreshed
				|| Result == EApplyResult::Stacked || Result == EApplyResult::ConvertedToStagger
				|| Result == EApplyResult::StaggerBroken;
		}
	};

	/** Per-tier crowd-control handling. */
	struct TierCCRules
	{
		float DurationScale = 1.f;
		bool bConvertCCToStagger = false;
		bool bImmuneToKnockback = false;
	};

	ECLIPSECORE_API TierCCRules GetTierCCRules(EMobTier Tier);

	/**
	 * Fixed-capacity status storage. One slot per effect type.
	 * Not thread-safe; owned by exactly one simulation thread (game thread or a
	 * swarm ParallelFor worker that owns that mob index).
	 */
	class ECLIPSECORE_API StatusContainer
	{
	public:
		static constexpr u8 MaxBleedStacks = 10;
		/** Design contract: Vulnerable is always at least +20% damage taken. */
		static constexpr float MinVulnerableBonus = 0.20f;

		bool Has(EStatusEffect E) const { return Slots[Idx(E)].IsActive(); }
		const StatusSlot& Get(EStatusEffect E) const { return Slots[Idx(E)]; }

		/** Applies with stacking rules only. Immunity/tier logic lives in ApplyStatusEffect(). */
		EApplyResult ApplyRaw(const StatusApplication& App);

		void Remove(EStatusEffect E) { Slots[Idx(E)] = StatusSlot{}; }
		void ClearCrowdControl();
		void ClearAll() { Slots = {}; }

		/**
		 * Advances all timers by Dt and returns DoT damage dealt during the step.
		 * The damage is pre-mitigation; callers run it through their damage path
		 * (which also applies Vulnerable, so DoTs benefit from it too).
		 */
		float Tick(float Dt);

		/** 1.0 + Vulnerable bonus. */
		float GetDamageTakenMultiplier() const;
		/** Frozen or stunned: cannot move, attack or cast. */
		bool IsDisabled() const { return Has(EStatusEffect::Stun) || Has(EStatusEffect::Freeze); }
		/** 0 when disabled, otherwise 1 - slow. */
		float GetMoveSpeedScale() const;
		/** Bit i set => effect i active. Cheap replication / UI payload. */
		u16 GetActiveMask() const;

	private:
		std::array<StatusSlot, StatusEffectCount> Slots{};
	};

	// -------------------------------------------------------------------------
	// Stagger Meter (bosses)
	// -------------------------------------------------------------------------

	struct StaggerConfig
	{
		float BaseMaxStagger = 100.f;
		/** Each break raises the max by this fraction so repeated CC-locking gets harder. */
		float MaxGrowthPerBreak = 0.25f;
		/** N seconds the boss stays stunned + vulnerable after a break. */
		float StaggeredDuration = 5.f;
		/** Vulnerable bonus applied for the stagger window (>= 0.20 by design contract). */
		float StaggeredVulnerableBonus = 0.30f;
		/** Grace period after the last fill before the meter starts draining. */
		float DecayDelay = 2.5f;
		float DecayPerSecond = 8.f;
		/** Bosses in an Unstoppable phase still accrue stagger, at a reduced rate. */
		float UnstoppableFillScale = 0.5f;
		/** Stagger points per second of *intended* CC duration, per effect. */
		std::array<float, StatusEffectCount> FillPerCCSecond = {
			0.f,  // Vulnerable
			0.f,  // Unstoppable
			0.f,  // Bleed
			0.f,  // Burn
			22.f, // Freeze
			25.f, // Stun
			6.f,  // Slow
			0.f   // Knockback (flat, see below)
		};
		float KnockbackFlatFill = 12.f;
	};

	struct StaggerState
	{
		float Current = 0.f;
		float TimeSinceLastFill = 0.f;
		float StaggeredRemaining = 0.f;
		u32 BreakCount = 0;

		bool IsStaggered() const { return StaggeredRemaining > 0.f; }
		float GetMax(const StaggerConfig& Cfg) const
		{
			return Cfg.BaseMaxStagger * (1.f + Cfg.MaxGrowthPerBreak * static_cast<float>(BreakCount));
		}
	};

	enum class EStaggerResult : u8
	{
		NotCrowdControl,
		AlreadyStaggered,
		Filled,
		Broke
	};

	struct StaggerOutcome
	{
		EStaggerResult Result = EStaggerResult::NotCrowdControl;
		float Added = 0.f;
		float Current = 0.f;
		float Max = 0.f;
	};

	/**
	 * Converts a crowd-control application into stagger on a boss.
	 * Does NOT apply the resulting Stun/Vulnerable; ApplyStatusEffect() does that
	 * so the meter math stays a pure, separately testable function.
	 *
	 * @param bTargetUnstoppable  Boss is in an Unstoppable phase -> fill is scaled down.
	 */
	ECLIPSECORE_API StaggerOutcome ApplyCrowdControlToBossStagger(StaggerState& State, const StaggerConfig& Cfg,
		const StatusApplication& App, bool bTargetUnstoppable);

	/** Advances decay / staggered window. Returns true on the frame the boss recovers. */
	ECLIPSECORE_API bool TickStagger(StaggerState& State, const StaggerConfig& Cfg, float Dt);

	// -------------------------------------------------------------------------
	// Unified pipeline
	// -------------------------------------------------------------------------

	/** Non-owning view of everything the pipeline needs to know about a target. */
	struct StatusTarget
	{
		StatusContainer& Statuses;
		EMobTier Tier = EMobTier::TrashSwarmer;
		StaggerState* Stagger = nullptr;          // Required when Tier == Boss.
		const StaggerConfig* StaggerCfg = nullptr; // Required when Tier == Boss.
	};

	ECLIPSECORE_API ApplyOutcome ApplyStatusEffect(StatusTarget& Target, const StatusApplication& App);
}
