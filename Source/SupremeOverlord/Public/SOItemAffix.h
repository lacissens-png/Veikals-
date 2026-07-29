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
	CritChance       UMETA(DisplayName = "Critical Hit Chance"),
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

/**
 * One threshold in an item set's bonus ladder — granted once at least
 * PiecesRequired items from the set are equipped simultaneously, stacking
 * with every lower threshold also met (standard 2pc/4pc/6pc ARPG design).
 * Only the four stats USOArmorData already supports are meaningful here
 * (MaxHealth, MaxMana, MovementSpeed, DamageReduction) — set bonuses are
 * character-wide, applied by USOEquipmentComponent alongside armor stats.
 */
USTRUCT(BlueprintType)
struct FSOSetBonusTier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetBonus", meta = (ClampMin = "2", UIMin = "2", UIMax = "8"))
	int32 PiecesRequired = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetBonus")
	FSOItemAffix Bonus;
};
