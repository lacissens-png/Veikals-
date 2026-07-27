#pragma once

#include "CoreMinimal.h"
#include "SOItemData.h"
#include "SOArmorData.generated.h"

/**
 * Passive armor item — additive stat bonuses stacked on the character by
 * the equipment component. Slot is authored on the base ItemData via
 * EquipSlot (defaults to Chest on new instances).
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOArmorData : public USOItemData
{
	GENERATED_BODY()

public:
	USOArmorData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor|Stats", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float MaxHealthBonus = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor|Stats", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float MaxManaBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor|Stats", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float DamageReductionPct = 0.0f;

	/** Multiplier applied to MovementSpeed while equipped (0.9 = -10%, 1.05 = +5%). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor|Stats", meta = (ClampMin = "0.25", UIMin = "0.25", UIMax = "2.0"))
	float MovementSpeedMultiplier = 1.0f;
};
