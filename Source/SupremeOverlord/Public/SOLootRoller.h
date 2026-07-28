#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SOItemAffix.h"
#include "SOLootRoller.generated.h"

class USOItemData;

/**
 * Turns a static, editor-authored USOItemData template into a distinct,
 * rarity-rolled drop instance with random affixes baked directly into its
 * stat fields (PrimaryDamageBonus, MaxHealthBonus, etc.) — the rest of the
 * pipeline (equipment, inventory, vendor) never needs to know an item is
 * "rolled"; it just sees a USOItemData* with slightly different numbers.
 *
 * Hand-authored legendaries/uniques (Template->bIsLegendaryUnique) are
 * duplicated but otherwise passed through unmodified — their rarity and
 * fixed LegendaryEffect are exactly as the designer set them.
 */
UCLASS()
class SUPREMEOVERLORD_API USOLootRoller : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Duplicates Template into a new instance (Outer'd to Outer, e.g. GetTransientPackage()),
	 * rolls a rarity tier and a matching number of random affixes scaled by ItemLevel, and
	 * applies them to the instance. Returns null if Template is null.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Loot")
	static USOItemData* RollItemInstance(const USOItemData* Template, UObject* Outer, int32 ItemLevel = 1);

private:
	static ESOItemRarity RollRarity();
	static int32         GetAffixCountForRarity(ESOItemRarity Rarity);
	static float         RollAffixMagnitude(ESOAffixStat Stat, int32 ItemLevel);
	static FString       BuildAffixDescription(ESOAffixStat Stat, float Value);
	static void          ApplyAffix(USOItemData* Item, const FSOItemAffix& Affix);
};
