// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "EclipseCore/StatusEffects.h"

namespace Eclipse
{
	TierCCRules GetTierCCRules(EMobTier Tier)
	{
		switch (Tier)
		{
		case EMobTier::TrashSwarmer:    return { 1.00f, false, false };
		case EMobTier::Captain:         return { 0.75f, false, false };
		case EMobTier::PrimordialBrute: return { 0.50f, false, true };
		case EMobTier::Boss:            return { 0.00f, true, true };
		case EMobTier::Player:          return { 1.00f, false, false };
		default:                        return {};
		}
	}

	// -------------------------------------------------------------------------
	// StatusContainer
	// -------------------------------------------------------------------------

	EApplyResult StatusContainer::ApplyRaw(const StatusApplication& App)
	{
		// Knockback is instantaneous: it has no meaningful duration but must still be
		// recorded for one frame so the movement step can consume the impulse.
		const bool bInstant = App.Effect == EStatusEffect::Knockback;
		if (App.Duration <= 0.f && !bInstant)
		{
			return EApplyResult::Rejected;
		}

		StatusSlot& Slot = Slots[Idx(App.Effect)];
		const bool bWasActive = Slot.IsActive();
		const float Duration = bInstant ? Max(App.Duration, 0.05f) : App.Duration;

		switch (App.Effect)
		{
		case EStatusEffect::Bleed:
		{
			// Intensity stacking: each application adds a stack (capped) and refreshes.
			// Magnitude stores the per-stack DPS of the strongest application seen.
			Slot.Stacks = static_cast<u8>(Min<int>(Slot.Stacks + 1, MaxBleedStacks));
			Slot.Magnitude = bWasActive ? Max(Slot.Magnitude, App.Magnitude) : App.Magnitude;
			Slot.Remaining = Max(Slot.Remaining, Duration);
			Slot.SourceId = App.SourceId;
			return bWasActive ? EApplyResult::Stacked : EApplyResult::Applied;
		}
		case EStatusEffect::Vulnerable:
		{
			const float Bonus = Max(App.Magnitude, MinVulnerableBonus);
			Slot.Magnitude = bWasActive ? Max(Slot.Magnitude, Bonus) : Bonus;
			Slot.Remaining = Max(Slot.Remaining, Duration);
			Slot.Stacks = 1;
			Slot.SourceId = App.SourceId;
			return bWasActive ? EApplyResult::Refreshed : EApplyResult::Applied;
		}
		case EStatusEffect::Unstoppable:
		{
			// Becoming Unstoppable purges existing crowd control (design rule).
			ClearCrowdControl();
			break;
		}
		default:
			break;
		}

		// Default policy: keep strongest magnitude, refresh to the longer duration.
		Slot.Magnitude = bWasActive ? Max(Slot.Magnitude, App.Magnitude) : App.Magnitude;
		Slot.Remaining = Max(Slot.Remaining, Duration);
		Slot.Stacks = 1;
		Slot.SourceId = App.SourceId;
		return bWasActive ? EApplyResult::Refreshed : EApplyResult::Applied;
	}

	void StatusContainer::ClearCrowdControl()
	{
		for (std::size_t I = 0; I < StatusEffectCount; ++I)
		{
			if (IsCrowdControl(static_cast<EStatusEffect>(I)))
			{
				Slots[I] = StatusSlot{};
			}
		}
	}

	float StatusContainer::Tick(float Dt)
	{
		if (Dt <= 0.f)
		{
			return 0.f;
		}

		float DotDamage = 0.f;
		for (std::size_t I = 0; I < StatusEffectCount; ++I)
		{
			StatusSlot& Slot = Slots[I];
			if (!Slot.IsActive())
			{
				continue;
			}

			// Integrate DoT only over the time the effect was actually alive this step,
			// so a 0.3s remaining burn ticked with Dt=1.0 deals 0.3s worth of damage.
			const float Alive = Min(Dt, Slot.Remaining);
			const EStatusEffect E = static_cast<EStatusEffect>(I);
			if (E == EStatusEffect::Bleed)
			{
				DotDamage += Slot.Magnitude * static_cast<float>(Slot.Stacks) * Alive;
			}
			else if (E == EStatusEffect::Burn)
			{
				DotDamage += Slot.Magnitude * Alive;
			}

			Slot.Remaining -= Dt;
			if (Slot.Remaining <= 0.f)
			{
				Slot = StatusSlot{};
			}
		}
		return DotDamage;
	}

	float StatusContainer::GetDamageTakenMultiplier() const
	{
		const StatusSlot& V = Slots[Idx(EStatusEffect::Vulnerable)];
		return V.IsActive() ? 1.f + Max(V.Magnitude, MinVulnerableBonus) : 1.f;
	}

	float StatusContainer::GetMoveSpeedScale() const
	{
		if (IsDisabled())
		{
			return 0.f;
		}
		const StatusSlot& S = Slots[Idx(EStatusEffect::Slow)];
		return S.IsActive() ? Clamp(1.f - S.Magnitude, 0.1f, 1.f) : 1.f;
	}

	u16 StatusContainer::GetActiveMask() const
	{
		u16 Mask = 0;
		for (std::size_t I = 0; I < StatusEffectCount; ++I)
		{
			if (Slots[I].IsActive())
			{
				Mask = static_cast<u16>(Mask | (1u << I));
			}
		}
		return Mask;
	}

	// -------------------------------------------------------------------------
	// Stagger
	// -------------------------------------------------------------------------

	StaggerOutcome ApplyCrowdControlToBossStagger(StaggerState& State, const StaggerConfig& Cfg,
		const StatusApplication& App, bool bTargetUnstoppable)
	{
		StaggerOutcome Out;
		Out.Max = State.GetMax(Cfg);
		Out.Current = State.Current;

		if (!IsCrowdControl(App.Effect))
		{
			Out.Result = EStaggerResult::NotCrowdControl;
			return Out;
		}
		if (State.IsStaggered())
		{
			// While broken the boss is already disabled; extra CC is wasted (no banking).
			Out.Result = EStaggerResult::AlreadyStaggered;
			return Out;
		}

		float Fill = (App.Effect == EStatusEffect::Knockback)
			? Cfg.KnockbackFlatFill
			: Cfg.FillPerCCSecond[Idx(App.Effect)] * Max(App.Duration, 0.f);
		if (bTargetUnstoppable)
		{
			Fill *= Cfg.UnstoppableFillScale;
		}

		const float Before = State.Current;
		State.Current = Min(State.Current + Fill, Out.Max); // Overflow is discarded.
		State.TimeSinceLastFill = 0.f;

		Out.Added = State.Current - Before;
		Out.Current = State.Current;

		if (State.Current >= Out.Max && Out.Max > 0.f)
		{
			State.StaggeredRemaining = Cfg.StaggeredDuration;
			State.BreakCount += 1;
			Out.Result = EStaggerResult::Broke;
		}
		else
		{
			Out.Result = EStaggerResult::Filled;
		}
		return Out;
	}

	bool TickStagger(StaggerState& State, const StaggerConfig& Cfg, float Dt)
	{
		if (Dt <= 0.f)
		{
			return false;
		}

		if (State.IsStaggered())
		{
			State.StaggeredRemaining -= Dt;
			if (State.StaggeredRemaining <= 0.f)
			{
				// Recover: meter resets fully so the next break requires the (grown) max.
				State.StaggeredRemaining = 0.f;
				State.Current = 0.f;
				State.TimeSinceLastFill = 0.f;
				return true;
			}
			return false;
		}

		State.TimeSinceLastFill += Dt;
		if (State.TimeSinceLastFill > Cfg.DecayDelay && State.Current > 0.f)
		{
			// Only drain for the portion of Dt that lies beyond the grace period.
			const float DrainTime = Min(Dt, State.TimeSinceLastFill - Cfg.DecayDelay);
			State.Current = Max(0.f, State.Current - Cfg.DecayPerSecond * DrainTime);
		}
		return false;
	}

	// -------------------------------------------------------------------------
	// Pipeline
	// -------------------------------------------------------------------------

	ApplyOutcome ApplyStatusEffect(StatusTarget& Target, const StatusApplication& App)
	{
		ApplyOutcome Out;
		const TierCCRules Rules = GetTierCCRules(Target.Tier);
		const bool bTargetUnstoppable = Target.Statuses.Has(EStatusEffect::Unstoppable);

		if (IsCrowdControl(App.Effect) && !App.bIgnoreImmunity)
		{
			// ---- Bosses: CC becomes stagger -------------------------------------
			if (Rules.bConvertCCToStagger)
			{
				if (Target.Stagger == nullptr || Target.StaggerCfg == nullptr)
				{
					return Out; // Misconfigured boss: reject rather than disabling it.
				}

				const StaggerOutcome S = ApplyCrowdControlToBossStagger(*Target.Stagger, *Target.StaggerCfg, App,
					bTargetUnstoppable);
				Out.StaggerAdded = S.Added;

				if (S.Result == EStaggerResult::Broke)
				{
					const float Window = Target.StaggerCfg->StaggeredDuration;

					StatusApplication Stun;
					Stun.Effect = EStatusEffect::Stun;
					Stun.Duration = Window;
					Stun.SourceId = App.SourceId;
					Stun.bIgnoreImmunity = true;
					Target.Statuses.ApplyRaw(Stun);

					StatusApplication Vuln;
					Vuln.Effect = EStatusEffect::Vulnerable;
					Vuln.Duration = Window;
					Vuln.Magnitude = Target.StaggerCfg->StaggeredVulnerableBonus;
					Vuln.SourceId = App.SourceId;
					Target.Statuses.ApplyRaw(Vuln);

					Out.Result = EApplyResult::StaggerBroken;
					Out.EffectiveDuration = Window;
				}
				else if (S.Result == EStaggerResult::Filled)
				{
					Out.Result = EApplyResult::ConvertedToStagger;
				}
				return Out; // Never apply the raw CC to a boss.
			}

			// ---- Everyone else ---------------------------------------------------
			if (bTargetUnstoppable)
			{
				Out.Result = EApplyResult::ResistedByImmunity;
				return Out;
			}
			if (App.Effect == EStatusEffect::Knockback && Rules.bImmuneToKnockback)
			{
				Out.Result = EApplyResult::ResistedByImmunity;
				return Out;
			}

			StatusApplication Scaled = App;
			Scaled.Duration = App.Duration * Rules.DurationScale;
			Out.Result = Target.Statuses.ApplyRaw(Scaled);
			Out.EffectiveDuration = Scaled.Duration;
			return Out;
		}

		// Boss scripts must not purge an active stagger window by going Unstoppable.
		if (App.Effect == EStatusEffect::Unstoppable && Target.Stagger && Target.Stagger->IsStaggered())
		{
			return Out;
		}

		Out.Result = Target.Statuses.ApplyRaw(App);
		Out.EffectiveDuration = App.Duration;
		return Out;
	}
}
