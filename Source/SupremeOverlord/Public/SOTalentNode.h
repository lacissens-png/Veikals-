#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SOTalentNode.generated.h"

class UTexture2D;

/**
 * Effect a talent grants when purchased. Kept as a small enum + magnitude so
 * a node's tuning can round-trip through save data and the effect resolver
 * stays in one switch.
 */
UENUM(BlueprintType)
enum class ESOTalentEffect : uint8
{
	None                    UMETA(DisplayName = "None"),
	FlatMaxHealth           UMETA(DisplayName = "+X Max Health"),
	FlatMaxMana             UMETA(DisplayName = "+X Max Mana"),
	FlatPrimaryDamage       UMETA(DisplayName = "+X Primary Attack Damage"),
	FlatShadowBoltDamage    UMETA(DisplayName = "+X Shadow Bolt Damage"),
	FlatLifeDrainHealFrac   UMETA(DisplayName = "+X Life Drain Heal Fraction"),
	FlatManaRegen           UMETA(DisplayName = "+X Mana Regen / s"),
	MultMovementSpeed       UMETA(DisplayName = "×(1+X) Movement Speed")
};

USTRUCT(BlueprintType)
struct FSOTalentEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	ESOTalentEffect EffectType = ESOTalentEffect::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float Magnitude = 0.0f;
};

/**
 * One node in the talent tree. Nodes are DataAssets, not embedded in a
 * mega-asset, so a designer can drag them into levels/UI/other assets and
 * reference them by hard pointer. Prereqs are hard pointers too — every
 * node the tree cares about is already loaded.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOTalentNode : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Talent|Presentation")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Talent|Presentation", meta = (MultiLine = "true"))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Talent|Presentation")
	TObjectPtr<UTexture2D> Icon;

	/** Talent points spent to unlock this node. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Talent|Cost", meta = (ClampMin = "1", UIMin = "1", UIMax = "20"))
	int32 PointCost = 1;

	/** All of these must be unlocked before this node is available. Empty = root node. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Talent|Cost")
	TArray<TObjectPtr<USOTalentNode>> Prerequisites;

	/** Stat effects applied to the owning ASOCharacter on unlock. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Talent|Effects")
	TArray<FSOTalentEffect> Effects;
};
