#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateColor.h"
#include "SOItemAffix.h"
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

/** Equipment slot an item occupies. `None` = not equippable (materials, quest items, etc.). */
UENUM(BlueprintType)
enum class ESOEquipSlot : uint8
{
	None      UMETA(DisplayName = "None"),
	MainHand  UMETA(DisplayName = "Main Hand"),
	OffHand   UMETA(DisplayName = "Off Hand"),
	Head      UMETA(DisplayName = "Head"),
	Chest     UMETA(DisplayName = "Chest"),
	Legs      UMETA(DisplayName = "Legs"),
	Boots     UMETA(DisplayName = "Boots"),
	Amulet    UMETA(DisplayName = "Amulet"),
	Ring1     UMETA(DisplayName = "Ring 1"),
	Ring2     UMETA(DisplayName = "Ring 2")
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

	/** Slot this item equips into. `None` = not equippable. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Meta")
	ESOEquipSlot EquipSlot = ESOEquipSlot::None;

	/** Convenience — color hint for tooltips / labels based on Rarity. */
	UFUNCTION(BlueprintPure, Category = "Item|Presentation")
	FLinearColor GetRarityColor() const;

	// -----------------------------------------------------------------------
	// Random affixes — populated on drop-time instances by USOLootRoller.
	// Empty on hand-authored editor templates.
	// -----------------------------------------------------------------------

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Item|Affixes", Transient)
	TArray<FSOItemAffix> RolledAffixes;

	/** Pre-formatted tooltip lines, one per rolled affix. Empty for un-rolled templates. */
	UFUNCTION(BlueprintPure, Category = "Item|Affixes")
	TArray<FString> GetAffixDescriptions() const;

	// -----------------------------------------------------------------------
	// Legendary Uniques — hand-authored items with a fixed special effect.
	// -----------------------------------------------------------------------

	/** Marks this as a hand-authored legendary/unique. USOLootRoller passes these through without rerolling rarity or affixes. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Legendary")
	bool bIsLegendaryUnique = false;

	/** Flavor text shown beneath the name on legendary/unique tooltips. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Legendary", meta = (MultiLine = "true"))
	FText LegendaryFlavorText;

	/** Fixed special ability this item grants while equipped. Checked via ASOCharacter::HasLegendaryEffect. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Legendary")
	ESOLegendaryEffect LegendaryEffect = ESOLegendaryEffect::None;

	// -----------------------------------------------------------------------
	// Item Sets — null means this item isn't part of a set.
	// -----------------------------------------------------------------------

	/**
	 * The set this item belongs to, if any. USOEquipmentComponent counts how
	 * many equipped items share the same ItemSet and applies every bonus
	 * tier whose piece threshold is met. USOLootRoller forces Rarity to Set
	 * and skips random affixes for any item with this set.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Set")
	TObjectPtr<class USOItemSetData> ItemSet;

	// -----------------------------------------------------------------------
	// Gem Sockets — USOEquipmentComponent::SocketGem fills these in.
	// -----------------------------------------------------------------------

	/** Number of gem sockets this item has. 0 = no sockets. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Sockets", meta = (ClampMin = "0", UIMin = "0", UIMax = "6"))
	int32 MaxSockets = 0;

	/** Gems currently socketed. Each entry's bonus has already been baked into this item's own stat fields. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Item|Sockets", Transient)
	TArray<TObjectPtr<class USOGemData>> SocketedGems;

	UFUNCTION(BlueprintPure, Category = "Item|Sockets")
	int32 GetFreeSocketCount() const { return FMath::Max(0, MaxSockets - SocketedGems.Num()); }

	/**
	 * The editor-authored template this instance was duplicated from (see
	 * USOLootRoller::RollItemInstance). Empty for hand-placed template assets
	 * themselves. Lets save/load reconstruct a dropped instance's base stats
	 * before re-applying RolledAffixes and SocketedGems.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Item|Meta", Transient)
	TSoftObjectPtr<USOItemData> SourceTemplate;
};
