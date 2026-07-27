#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOInventoryComponent.generated.h"

class USOMaterialData;
class USORecipeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSOOnMaterialCountChanged,
	USOMaterialData*, Material,
	int32,            NewCount,
	int32,            Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnRecipeCrafted,
	USORecipeData*,   Recipe,
	class USOItemData*, ResultItem);

/**
 * Material counters + crafting entry point. Tracks how many of each
 * `USOMaterialData` the player has (respecting per-material MaxStack) and
 * consumes ingredients/gold to produce recipe results.
 *
 * Deliberately minimal: no full item inventory grid — for now the
 * equipment component owns equipped items and the crafted result is
 * auto-equipped when possible or delegated to the on-crafted BP hook.
 */
UCLASS(ClassGroup = (SupremeOverlord), meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Inventory")
	FSOOnMaterialCountChanged OnMaterialCountChanged;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Inventory")
	FSOOnRecipeCrafted OnRecipeCrafted;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Inventory")
	int32 GetMaterialCount(USOMaterialData* Material) const;

	/** Adds Count of Material (respecting MaxStack). Returns the amount actually added. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Inventory")
	int32 AddMaterial(USOMaterialData* Material, int32 Count);

	/** Removes Count of Material. Returns the amount actually removed. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Inventory")
	int32 RemoveMaterial(USOMaterialData* Material, int32 Count);

	/** Returns true iff every ingredient + GoldCost is currently satisfied. */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Inventory|Crafting")
	bool CanCraft(USORecipeData* Recipe, FString& OutReason) const;

	/**
	 * Consumes ingredients + gold and grants the recipe's result. Auto-equips the
	 * result via the owning character's EquipmentComponent when the result is
	 * equippable. Returns true on success.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Inventory|Crafting")
	bool Craft(USORecipeData* Recipe);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Inventory")
	const TMap<TObjectPtr<USOMaterialData>, int32>& GetAllMaterials() const { return Materials; }

private:
	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Inventory", Transient)
	TMap<TObjectPtr<USOMaterialData>, int32> Materials;
};
