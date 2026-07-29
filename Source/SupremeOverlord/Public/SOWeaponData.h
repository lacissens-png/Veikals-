#pragma once

#include "CoreMinimal.h"
#include "SOItemData.h"
#include "SOWeaponData.generated.h"

UENUM(BlueprintType)
enum class ESOWeaponType : uint8
{
	Sword UMETA(DisplayName = "Sword"),
	Axe   UMETA(DisplayName = "Axe"),
	Mace  UMETA(DisplayName = "Mace"),
	Staff UMETA(DisplayName = "Staff"),
	Wand  UMETA(DisplayName = "Wand"),
	Orb   UMETA(DisplayName = "Orb")
};

/**
 * Weapon item — additive damage bonuses and a cooldown multiplier stacked
 * on top of the character's base stats.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOWeaponData : public USOItemData
{
	GENERATED_BODY()

public:
	USOWeaponData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	ESOWeaponType WeaponType = ESOWeaponType::Sword;

	/** Flat damage added to the character's PrimaryAttackDamage. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float PrimaryDamageBonus = 10.0f;

	/** Flat damage added to the character's ShadowBoltDamage. Mostly non-zero on caster weapons. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float ShadowBoltDamageBonus = 0.0f;

	/** Multiplier applied to PrimaryAttackCooldown (0.8 = 20% faster). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "3.0"))
	float PrimaryAttackCooldownMultiplier = 1.0f;

	/** Flat bonus [0-1] added to the wielder's crit chance while this weapon is equipped. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CritChanceBonus = 0.0f;
};
