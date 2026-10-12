// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// DamageMath.h
// -----------------------------------------------------------------------------
// Shared, pure damage formulas. Used by the GAS damage execution (players and
// bosses) AND by the swarm's direct damage path so both agree exactly.
// -----------------------------------------------------------------------------
#pragma once

#include "EclipseCore/EclipseCoreTypes.h"
#include "EclipseCore/StatusEffects.h"

namespace Eclipse
{
	struct DamageTuning
	{
		/** Armor curve constant: Mitigation = Armor / (Armor + K). */
		float ArmorK = 1000.f;
		/** Hard cap so armor stacking can never trivialise content. */
		float MaxMitigation = 0.85f;
	};

	/** Fraction of damage removed by armor after penetration, in [0, MaxMitigation]. */
	ECLIPSECORE_API float ComputeArmorMitigation(float Armor, float ArmorPenetration01, const DamageTuning& Tuning = {});

	/**
	 * Final damage = Base * (1 - Mitigation(Armor * (1 - Pen))) * DamageTakenMultiplier * OutgoingMultiplier.
	 * Reality Slash passes ArmorPenetration01 = 0.5 ("ignores 50% armor").
	 */
	ECLIPSECORE_API float ComputeFinalDamage(float BaseDamage, float TargetArmor, float ArmorPenetration01,
		float DamageTakenMultiplier, float OutgoingMultiplier = 1.f, const DamageTuning& Tuning = {});

	/**
	 * Puppet Master's Thread: damage mirrored onto EACH other linked enemy.
	 * Shared damage is not re-shared (callers must flag it) to prevent feedback loops.
	 */
	ECLIPSECORE_API float ComputeLinkedDamageShare(float DamageDealtToLinkedTarget, float ShareFraction01);

	/**
	 * Execute rule (Grasping Heart, death waves):
	 *  - Trash: always executable.
	 *  - Captain: below Threshold health.
	 *  - Brute: below Threshold * 0.5.
	 *  - Boss / Player: never.
	 */
	ECLIPSECORE_API bool ShouldExecute(float CurrentHealth, float MaxHealth, EMobTier Tier, float Threshold01);
}
