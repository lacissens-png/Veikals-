// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// Replicable / Blueprint-visible mirrors of Eclipse::ItemInstance and friends.
//
// FEclipseItemData is the project's `FItemData`. It is a pure DATA snapshot:
// clients may read it for UI, but every mutation is done by converting to the
// core type on the server, calling a core rule (Reforge/Imbue/Socket), and
// converting back. See EclipseItemConversion.h.

#pragma once

#include "CoreMinimal.h"
#include "EclipseCore/Items.h"

#include "EclipseItemTypes.generated.h"

UENUM(BlueprintType)
enum class EEclipseItemRarity : uint8
{
	Common,
	Magic,
	Rare,
	Legendary,
	AncestralEldritch
};

UENUM(BlueprintType)
enum class EEclipseItemSlot : uint8
{
	MainHand,
	OffHand,
	TwoHand,
	Helm,
	Chest,
	Gloves,
	Legs,
	Boots,
	Amulet,
	Ring
};

static_assert(static_cast<uint8>(EEclipseItemRarity::AncestralEldritch) + 1 == static_cast<uint8>(Eclipse::EItemRarity::Count), "Rarity mirror out of sync");
static_assert(static_cast<uint8>(EEclipseItemSlot::Ring) + 1 == static_cast<uint8>(Eclipse::EItemSlot::Count), "Slot mirror out of sync");

USTRUCT(BlueprintType)
struct FEclipseAffixRoll
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item") int32 DefinitionId = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Item") float Value = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Item") float Quality = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Item") bool bGreater = false;
};

USTRUCT(BlueprintType)
struct FEclipseSocketPair
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item") int32 RitualRuneId = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Item") int32 EffectRuneId = 0;
};

USTRUCT(BlueprintType)
struct FEclipseMaterialWallet
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Materials") int32 SoulDust = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Materials") int32 ObsidianOre = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Materials") int32 PrimordialBlood = 0;
};

/** The project's FItemData. */
USTRUCT(BlueprintType)
struct FEclipseItemData
{
	GENERATED_BODY()

	/** Server-assigned, globally unique (64-bit) id. Clients reference items by this only. */
	UPROPERTY(BlueprintReadOnly, Category = "Item") int64 ItemGuid = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Item") int32 BaseId = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Item") EEclipseItemSlot Slot = EEclipseItemSlot::MainHand;
	UPROPERTY(BlueprintReadOnly, Category = "Item") EEclipseItemRarity Rarity = EEclipseItemRarity::Common;
	UPROPERTY(BlueprintReadOnly, Category = "Item") int32 ItemLevel = 1;

	/** Server-only roll seed. NotReplicated: clients never learn it (reroll prediction defence-in-depth). */
	UPROPERTY(NotReplicated, SaveGame) int64 Seed = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Item") TArray<FEclipseAffixRoll> Affixes;
	UPROPERTY(BlueprintReadOnly, Category = "Item") int32 AspectId = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Item") float AspectValue = 0.f;

	/** Bit i => affix i is Reforgable. Write-once (see Eclipse::SelectReforgeAffixes). */
	UPROPERTY(BlueprintReadOnly, Category = "Reforge") uint8 ReforgeSelectionMask = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Reforge") int32 ReforgeCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Sockets") TArray<FEclipseSocketPair> Sockets;

	UPROPERTY(BlueprintReadOnly, Category = "Item") bool bEquipped = false;
};

/** Blueprint-free alias matching the design document's naming. */
using FItemData = FEclipseItemData;

namespace EclipseItems
{
	ECLIPSEGAME_API Eclipse::ItemInstance ToCore(const FEclipseItemData& In);
	/** Writes rolled/mutable fields back; preserves UE-only fields (bEquipped). */
	ECLIPSEGAME_API void FromCore(const Eclipse::ItemInstance& In, FEclipseItemData& Out);

	ECLIPSEGAME_API Eclipse::MaterialBundle ToCore(const FEclipseMaterialWallet& In);
	ECLIPSEGAME_API FEclipseMaterialWallet FromCore(const Eclipse::MaterialBundle& In);
}
