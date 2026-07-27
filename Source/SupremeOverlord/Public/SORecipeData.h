#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SORecipeData.generated.h"

class USOItemData;
class USOMaterialData;

/** One line of a recipe: N of this material. */
USTRUCT(BlueprintType)
struct FSORecipeIngredient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ingredient")
	TObjectPtr<USOMaterialData> Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ingredient", meta = (ClampMin = "1", UIMin = "1", UIMax = "100"))
	int32 Count = 1;
};

/**
 * Crafting recipe: N materials → 1 result item.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USORecipeData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe|Presentation")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	TArray<FSORecipeIngredient> Ingredients;

	/** Item granted when the recipe is crafted. Typically a USOWeaponData or USOArmorData. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe")
	TObjectPtr<USOItemData> Result;

	/** Gold cost per craft (in addition to the materials). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recipe", meta = (ClampMin = "0", UIMin = "0", UIMax = "100000"))
	int32 GoldCost = 0;
};
