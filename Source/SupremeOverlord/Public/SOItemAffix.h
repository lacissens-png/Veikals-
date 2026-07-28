#pragma once

#include "CoreMinimal.h"
#include "SOItemAffix.generated.h"

/** Stat a random affix can roll onto a weapon or armor instance. */
UENUM(BlueprintType)
enum class ESOAffixStat : uint8
{
	PrimaryDamage    UMETA(DisplayName = "Primary Damage"),
	ShadowBoltDamage UMETA(DisplayName = "Shadow Bolt Damage"),
	AttackSpeed      UMETA(DisplayName = "Attack Speed"),
	MaxHealth        UMETA(DisplayName = "Maximum Health"),
	MaxMana          UMETA(DisplayName = "Maximum Mana"),
	MovementSpeed    UMETA(DisplayName = "Movement Speed"),
	DamageReduction  UMETA(DisplayName = "Damage Reduction"),
};

/** One rolled stat bonus on a specific item instance, applied directly to its stat fields. */
USTRUCT(BlueprintType)
struct FSOItemAffix
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Affix")
	ESOAffixStat Stat = ESOAffixStat::PrimaryDamage;

	/** Rolled magnitude — flat for Damage/Health/Mana, fractional (0.1 = 10%) for Speed/Reduction/AttackSpeed. */
	UPROPERTY(BlueprintReadOnly, Category = "Affix")
	float Value = 0.0f;

	/** Pre-formatted tooltip line, e.g. "+42 Primary Damage". */
	UPROPERTY(BlueprintReadOnly, Category = "Affix")
	FString Description;
};

/**
 * Fixed, hand-authored special ability a legendary/unique item can carry.
 * Distinct from random rarity/affix rolls — set on the DataAsset by a designer
 * alongside bIsLegendaryUnique, and checked by ASOCharacter::HasLegendaryEffect
 * at the relevant cast sites.
 */
UENUM(BlueprintType)
enum class ESOLegendaryEffect : uint8
{
	None                UMETA(DisplayName = "None"),
	ShadowBoltChain     UMETA(DisplayName = "Shadow Bolt Chains"),
	CorpseExplosionFree UMETA(DisplayName = "Free Corpse Explosion"),
	EndlessMinions      UMETA(DisplayName = "Endless Minions"),
	VampiricStrikes     UMETA(DisplayName = "Vampiric Strikes"),
};
