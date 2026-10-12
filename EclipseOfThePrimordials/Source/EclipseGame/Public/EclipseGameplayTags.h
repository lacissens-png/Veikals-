// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// Native gameplay tags. Declared in C++ so they exist before any asset loads and
// typos fail at compile time instead of silently never matching.

#pragma once

#include "NativeGameplayTags.h"

namespace EclipseTags
{
	// --- Ritual events: sent to the INSTIGATOR's ASC; runewords bind to these ---
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ritual_UltimateUsed);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ritual_CrowdControlApplied);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ritual_HeavyDamageTaken);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ritual_EnemyKilled);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ritual_ResourceSpent);

	// --- SetByCaller data keys ---
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_ArmorPenetration);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_ResourceDelta);

	// --- Ability classification ---
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Ultimate);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Source_Runeword);

	// --- Loose status tags mirrored onto the ASC (drive ActivationBlockedTags, anims, UI) ---
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Vulnerable);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Unstoppable);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Bleed);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Burn);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Freeze);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Stun);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Slow);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Knockback);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Staggered);

	// --- Gameplay cues (cosmetic only; never carry gameplay state) ---
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Stagger_Break);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_TimeStop);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_DeathWave);
	ECLIPSEGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_OrbitalBeam);

	/** Maps a status index (Eclipse::EStatusEffect) to its loose tag. */
	ECLIPSEGAME_API FGameplayTag GetStatusTag(uint8 StatusIndex);
}
