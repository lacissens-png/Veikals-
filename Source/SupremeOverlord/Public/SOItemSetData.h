#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SOItemAffix.h"
#include "SOItemSetData.generated.h"

/**
 * Defines one equipment set's bonus ladder. Referenced directly from
 * USOItemData::ItemSet on each piece that belongs to the set — no ID
 * registry or asset-manager lookup needed, matching how other
 * cross-references (TrapClass, MinionClass, etc.) work in this codebase.
 *
 * USOEquipmentComponent counts how many equipped items share the same
 * ItemSet and applies every tier whose PiecesRequired is met.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOItemSetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set")
	FText SetName;

	/** Bonus thresholds, e.g. a 2pc and a 4pc entry. Order doesn't matter — all met thresholds stack. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set")
	TArray<FSOSetBonusTier> Tiers;
};
