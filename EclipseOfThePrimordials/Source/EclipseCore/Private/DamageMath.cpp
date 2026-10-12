// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "EclipseCore/DamageMath.h"

namespace Eclipse
{
	float ComputeArmorMitigation(float Armor, float ArmorPenetration01, const DamageTuning& Tuning)
	{
		const float EffectiveArmor = Max(0.f, Armor) * (1.f - Clamp(ArmorPenetration01, 0.f, 1.f));
		if (EffectiveArmor <= 0.f)
		{
			return 0.f;
		}
		return Min(EffectiveArmor / (EffectiveArmor + Tuning.ArmorK), Tuning.MaxMitigation);
	}

	float ComputeFinalDamage(float BaseDamage, float TargetArmor, float ArmorPenetration01,
		float DamageTakenMultiplier, float OutgoingMultiplier, const DamageTuning& Tuning)
	{
		if (BaseDamage <= 0.f)
		{
			return 0.f;
		}
		const float Mitigation = ComputeArmorMitigation(TargetArmor, ArmorPenetration01, Tuning);
		return BaseDamage * (1.f - Mitigation) * Max(0.f, DamageTakenMultiplier) * Max(0.f, OutgoingMultiplier);
	}

	float ComputeLinkedDamageShare(float DamageDealtToLinkedTarget, float ShareFraction01)
	{
		return Max(0.f, DamageDealtToLinkedTarget) * Clamp(ShareFraction01, 0.f, 1.f);
	}

	bool ShouldExecute(float CurrentHealth, float MaxHealth, EMobTier Tier, float Threshold01)
	{
		if (MaxHealth <= 0.f || CurrentHealth <= 0.f)
		{
			return false;
		}
		const float Pct = CurrentHealth / MaxHealth;
		switch (Tier)
		{
		case EMobTier::TrashSwarmer:    return true;
		case EMobTier::Captain:         return Pct <= Threshold01;
		case EMobTier::PrimordialBrute: return Pct <= Threshold01 * 0.5f;
		default:                        return false;
		}
	}
}
