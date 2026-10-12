// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "EclipseGameplayTags.h"

#include "EclipseCore/StatusEffects.h"

namespace EclipseTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Ritual_UltimateUsed, "Event.Ritual.UltimateUsed", "Owner activated an ultimate ability.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Ritual_CrowdControlApplied, "Event.Ritual.CrowdControlApplied", "Owner landed crowd control (or stagger) on an enemy.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Ritual_HeavyDamageTaken, "Event.Ritual.HeavyDamageTaken", "Owner took a hit >= threshold of max health. Magnitude = fraction.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Ritual_EnemyKilled, "Event.Ritual.EnemyKilled", "Owner killed an enemy.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Ritual_ResourceSpent, "Event.Ritual.ResourceSpent", "Owner spent class resource. Magnitude = amount.");

	UE_DEFINE_GAMEPLAY_TAG(Data_Damage, "Data.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Data_ArmorPenetration, "Data.ArmorPenetration");
	UE_DEFINE_GAMEPLAY_TAG(Data_ResourceDelta, "Data.ResourceDelta");

	UE_DEFINE_GAMEPLAY_TAG(Ability_Type_Ultimate, "Ability.Type.Ultimate");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Source_Runeword, "Ability.Source.Runeword");

	UE_DEFINE_GAMEPLAY_TAG(Status_Vulnerable, "Status.Vulnerable");
	UE_DEFINE_GAMEPLAY_TAG(Status_Unstoppable, "Status.Unstoppable");
	UE_DEFINE_GAMEPLAY_TAG(Status_Bleed, "Status.Bleed");
	UE_DEFINE_GAMEPLAY_TAG(Status_Burn, "Status.Burn");
	UE_DEFINE_GAMEPLAY_TAG(Status_Freeze, "Status.Freeze");
	UE_DEFINE_GAMEPLAY_TAG(Status_Stun, "Status.Stun");
	UE_DEFINE_GAMEPLAY_TAG(Status_Slow, "Status.Slow");
	UE_DEFINE_GAMEPLAY_TAG(Status_Knockback, "Status.Knockback");
	UE_DEFINE_GAMEPLAY_TAG(State_Staggered, "State.Staggered");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Stagger_Break, "GameplayCue.Stagger.Break");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_TimeStop, "GameplayCue.TimeStop");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_DeathWave, "GameplayCue.DeathWave");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_OrbitalBeam, "GameplayCue.OrbitalBeam");

	FGameplayTag GetStatusTag(uint8 StatusIndex)
	{
		static_assert(Eclipse::StatusEffectCount == 8, "Update the status tag table.");
		switch (static_cast<Eclipse::EStatusEffect>(StatusIndex))
		{
		case Eclipse::EStatusEffect::Vulnerable:  return Status_Vulnerable;
		case Eclipse::EStatusEffect::Unstoppable: return Status_Unstoppable;
		case Eclipse::EStatusEffect::Bleed:       return Status_Bleed;
		case Eclipse::EStatusEffect::Burn:        return Status_Burn;
		case Eclipse::EStatusEffect::Freeze:      return Status_Freeze;
		case Eclipse::EStatusEffect::Stun:        return Status_Stun;
		case Eclipse::EStatusEffect::Slow:        return Status_Slow;
		case Eclipse::EStatusEffect::Knockback:   return Status_Knockback;
		default:                                  return FGameplayTag();
		}
	}
}
