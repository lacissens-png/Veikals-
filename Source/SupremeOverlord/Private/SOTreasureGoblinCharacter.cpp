#include "SOTreasureGoblinCharacter.h"

ASOTreasureGoblinCharacter::ASOTreasureGoblinCharacter()
{
	// Flees rather than fights.
	bFleeFromPlayer      = true;
	FleeDistance         = 900.0f;
	FleeSpeedMultiplier  = 1.6f;
	SightRadius          = 2000.0f;
	LoseSightRadius      = 3000.0f;

	// Melee stats are irrelevant since it never attacks, but keep them harmless just in case.
	AttackDamage  = 0.0f;
	AttackRange   = 0.0f;

	// The whole point: dying is a jackpot.
	LootRollCount      = 6;
	bGuaranteedItemDrop = true;
	ItemDropChance      = 1.0f;

	XPReward        = 150;
	CorruptionValue = 20.0f;
}
