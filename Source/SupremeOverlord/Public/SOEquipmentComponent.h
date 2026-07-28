#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOItemData.h"
#include "SOEquipmentComponent.generated.h"

class USOWeaponData;
class USOArmorData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSOOnSlotChanged,
	ESOEquipSlot,  Slot,
	USOItemData*,  OldItem,
	USOItemData*,  NewItem);

/**
 * Multi-slot equipment container. Owns one item per ESOEquipSlot; when a
 * slot changes we re-emit `Owner->EquipWeapon` for the MainHand slot (so
 * existing weapon-slot code keeps working unchanged) and re-aggregate
 * armor stats onto the owning character.
 *
 * Stat aggregation is idempotent: RecomputeAggregateStats derives the
 * total from every equipped armor at once, then diffs against the last
 * applied totals so no bonus is ever counted twice.
 */
UCLASS(ClassGroup = (SupremeOverlord), meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOEquipmentComponent();

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Equipment")
	FSOOnSlotChanged OnSlotChanged;

	/**
	 * Puts an item into the correct slot (using Item->EquipSlot). Returns
	 * true on success, false if the item was null or non-equippable.
	 * The previously-equipped item, if any, is unequipped in the same call.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Equipment")
	bool Equip(USOItemData* Item);

	/** Clears one slot. Returns the item that was removed (or null). */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Equipment")
	USOItemData* Unequip(ESOEquipSlot Slot);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Equipment")
	USOItemData* GetItemInSlot(ESOEquipSlot Slot) const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Equipment")
	const TMap<ESOEquipSlot, TObjectPtr<USOItemData>>& GetAllEquipped() const { return EquippedItems; }

	/**
	 * Sums MaxHealth/Mana/DR/SpeedMult from every equipped armor plus any
	 * active item-set bonuses (2pc/4pc/etc., see USOItemSetData), and applies
	 * the combined delta to the owner.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Equipment")
	void RecomputeAggregateStats();

	/** Human-readable lines for every currently active set bonus tier, e.g. "Bonewalker's Regalia (4pc): +12% Damage Reduction". */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Equipment")
	const TArray<FString>& GetActiveSetBonusDescriptions() const { return ActiveSetBonusDescriptions; }

private:
	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Equipment", Transient)
	TMap<ESOEquipSlot, TObjectPtr<USOItemData>> EquippedItems;

	// Last applied aggregate values, so we can diff and never double-count.
	float AppliedMaxHealthBonus     = 0.0f;
	float AppliedMaxManaBonus       = 0.0f;
	float AppliedSpeedMultiplier    = 1.0f;
	float AppliedDamageReductionPct = 0.0f;

	UPROPERTY(Transient)
	TArray<FString> ActiveSetBonusDescriptions;
};
