// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// Runewords.h
// -----------------------------------------------------------------------------
// Runes, paired sockets, and the RunewordEvaluator.
//
// MODEL
//   Each socket PAIR holds one Ritual rune (condition) + one Effect rune (action).
//   Ritual runes generate "Offering" when their condition fires; the paired
//   Effect rune triggers once the pair's Offering bank covers its cost (and its
//   cooldown is ready). Cost 0 => fires on every qualifying event (cooldown-gated).
//
// "CONTINUOUS" EVALUATION WITHOUT POLLING
//   Instead of scanning all equipment every frame, the evaluator is
//   event-driven:
//     * Rebuild()   - runs only when equipment or sockets change. Validates
//                     every pair and builds a per-condition index.
//     * EvaluateRuneword(event) - O(bindings for that condition), typically 0-2.
//     * Tick(dt)    - advances cooldowns only.
//   The UE layer (UEclipseRunewordComponent) dynamically subscribes to the GAS
//   gameplay-event tag of every condition that has >= 1 binding, and
//   unsubscribes when the last binding for a condition disappears.
// -----------------------------------------------------------------------------
#pragma once

#include "EclipseCore/Items.h"

#include <array>
#include <vector>

namespace Eclipse
{
	enum class ERuneType : u8
	{
		Ritual, // Condition / trigger.
		Effect  // Action / outcome.
	};

	enum class ERitualCondition : u8
	{
		OnUltimateUsed,
		OnCrowdControlApplied,
		OnHeavyDamageTaken,   // Magnitude = damage as a fraction of max health.
		OnEnemyKilled,
		OnResourceSpent,      // Magnitude = resource amount spent.
		Count
	};
	constexpr std::size_t RitualConditionCount = Idx(ERitualCondition::Count);

	enum class ERuneEffectAction : u8
	{
		TriggerTimeStop,      // Param = duration (seconds).
		SpawnFleshColossus,   // Param = lifetime (seconds).
		CastRealitySlash,     // Param = damage scalar.
		GainUnstoppable,      // Param = duration.
		Count
	};

	struct RuneDefinition
	{
		u32 Id = InvalidId;
		ERuneType Type = ERuneType::Ritual;

		// --- Ritual ---
		ERitualCondition Condition = ERitualCondition::OnUltimateUsed;
		/** Event Magnitude must be >= Threshold to count (e.g. 0.2 = "20% of max life in one hit"). */
		float Threshold = 0.f;
		u32 OfferingGenerated = 0;

		// --- Effect ---
		ERuneEffectAction Action = ERuneEffectAction::TriggerTimeStop;
		float Param = 0.f;
		u32 OfferingCost = 0;
		float Cooldown = 0.f;
	};

	class ECLIPSECORE_API RuneDatabase
	{
	public:
		void Add(const RuneDefinition& Def);
		const RuneDefinition* Find(u32 Id) const;

	private:
		std::vector<RuneDefinition> Defs; // Sorted by Id.
	};

	enum class ESocketError : u8
	{
		None,
		InvalidPairIndex,
		UnknownRune,
		SlotOccupied,
		SlotEmpty
	};

	/** Inserts a rune into the correct half of a pair based on its type. */
	ECLIPSECORE_API ESocketError InsertRune(ItemInstance& Item, u8 PairIndex, u32 RuneId, const RuneDatabase& Db);
	/** Removes and returns the rune id of the given type from a pair. */
	ECLIPSECORE_API ESocketError RemoveRune(ItemInstance& Item, u8 PairIndex, ERuneType Type, u32& OutRuneId);

	enum class ERunewordValidity : u8
	{
		Valid,
		Incomplete,
		UnknownRune,
		RitualSlotHoldsEffect,
		EffectSlotHoldsRitual
	};

	/** Pure validation of a single pair. Out pointers are set only when Valid. */
	ECLIPSECORE_API ERunewordValidity ValidateRunewordPair(const SocketPair& Pair, const RuneDatabase& Db,
		const RuneDefinition*& OutRitual, const RuneDefinition*& OutEffect);

	struct RitualEvent
	{
		ERitualCondition Condition = ERitualCondition::OnUltimateUsed;
		float Magnitude = 1.f;
	};

	struct RuneEffectInvocation
	{
		ERuneEffectAction Action = ERuneEffectAction::TriggerTimeStop;
		float Param = 0.f;
		u64 ItemGuid = 0;
		u8 PairIndex = 0;
	};

	/** A validated, active Ritual->Effect pairing. Rune definitions are copied (no dangling pointers). */
	struct RunewordBinding
	{
		u64 ItemGuid = 0;
		u8 PairIndex = 0;
		RuneDefinition Ritual;
		RuneDefinition Effect;
		u32 OfferingBank = 0;
		float CooldownRemaining = 0.f;
	};

	class ECLIPSECORE_API RunewordEvaluator
	{
	public:
		/**
		 * Re-derives all bindings from equipped items. Offering banks and cooldowns
		 * are carried over for bindings that still exist (same item, pair, runes),
		 * so unequipping an unrelated ring does not reset progress (or let players
		 * reset cooldowns by re-equipping).
		 */
		void Rebuild(const std::vector<const ItemInstance*>& Equipped, const RuneDatabase& Db);

		/**
		 * Feeds one gameplay event. Appends every effect that fires to Out.
		 * @return number of effects triggered.
		 */
		int EvaluateRuneword(const RitualEvent& Event, std::vector<RuneEffectInvocation>& Out);

		void Tick(float Dt);

		const std::vector<RunewordBinding>& GetBindings() const { return Bindings; }
		std::size_t GetBindingCountFor(ERitualCondition C) const { return ByCondition[Idx(C)].size(); }

	private:
		std::vector<RunewordBinding> Bindings;
		std::array<std::vector<u16>, RitualConditionCount> ByCondition;
	};
}
