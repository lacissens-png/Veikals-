#pragma once

#include "CoreMinimal.h"
#include "SOItemData.h"
#include "SOMaterialData.generated.h"

/**
 * Crafting material. No stats — just an item identity the inventory can
 * count. Rarity carries over from USOItemData for the tooltip color.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOMaterialData : public USOItemData
{
	GENERATED_BODY()

public:
	USOMaterialData();

	/** How high the inventory allows this material to stack. Zero = unlimited. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material", meta = (ClampMin = "0", UIMin = "0", UIMax = "1000"))
	int32 MaxStack = 99;
};
