#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateColor.h"
#include "SOItemData.generated.h"

class UTexture2D;

/** Rough Diablo-style rarity tiers driving color/tooltip layout later. */
UENUM(BlueprintType)
enum class ESOItemRarity : uint8
{
	Common     UMETA(DisplayName = "Common"),
	Magic      UMETA(DisplayName = "Magic"),
	Rare       UMETA(DisplayName = "Rare"),
	Unique     UMETA(DisplayName = "Unique"),
	Legendary  UMETA(DisplayName = "Legendary"),
	Set        UMETA(DisplayName = "Set")
};

/**
 * Base data asset for every item in Supreme Overlord.
 * PrimaryDataAsset so it participates in the asset registry — useful
 * for loot-table filtering and BP dropdowns.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Presentation")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Presentation", meta = (MultiLine = "true"))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Presentation")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Meta", meta = (ClampMin = "1", UIMin = "1"))
	int32 ItemLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Meta")
	ESOItemRarity Rarity = ESOItemRarity::Common;

	/** Convenience — color hint for tooltips / labels based on Rarity. */
	UFUNCTION(BlueprintPure, Category = "Item|Presentation")
	FLinearColor GetRarityColor() const;
};
