// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Items/EclipseItemTypes.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "EclipseInventoryComponent.generated.h"

class UEclipseInventoryComponent;
struct FEclipseInventoryList;

USTRUCT()
struct FEclipseInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY() FEclipseItemData Item;

	void PostReplicatedAdd(const FEclipseInventoryList& List);
	void PostReplicatedChange(const FEclipseInventoryList& List);
	void PreReplicatedRemove(const FEclipseInventoryList& List);
};

USTRUCT()
struct FEclipseInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY() TArray<FEclipseInventoryEntry> Entries;

	/** Back-pointer for client callbacks. Not replicated. */
	UPROPERTY(NotReplicated) TObjectPtr<UEclipseInventoryComponent> Owner = nullptr;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FEclipseInventoryEntry, FEclipseInventoryList>(Entries, DeltaParms, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FEclipseInventoryList> : public TStructOpsTypeTraitsBase2<FEclipseInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

USTRUCT(BlueprintType)
struct FEclipseRuneStack
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Runes") int32 RuneId = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Runes") int32 Count = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEclipseOnInventoryChanged);
DECLARE_MULTICAST_DELEGATE(FEclipseOnEquipmentChangedNative);

/**
 * Server-authoritative inventory, owned by AEclipsePlayerState (survives pawn death).
 *
 * REPLICATION
 *   Items / materials / rune stash replicate COND_OwnerOnly: other players never
 *   receive your stash (bandwidth + anti-snooping). Visible equipment for other
 *   players should be a separate, tiny replicated "appearance" array (not shown).
 *
 * AUTHORITY
 *   Every mutating function checks HasAuthority(). The only client entry points
 *   are the Server_ RPCs, which take ids and indices - never item data.
 */
UCLASS(ClassGroup = (Eclipse), meta = (BlueprintSpawnableComponent))
class ECLIPSEGAME_API UEclipseInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEclipseInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ---- Server API --------------------------------------------------------------
	/** Assigns a fresh ItemGuid when Item.ItemGuid == 0. Returns the guid (0 on failure). */
	int64 AddItem(FEclipseItemData Item);
	bool RemoveItem(int64 ItemGuid, FEclipseItemData* OutRemoved = nullptr);
	FEclipseItemData* FindItemMutable(int64 ItemGuid);
	/** Must be called after mutating an item obtained from FindItemMutable. */
	void MarkItemChanged(int64 ItemGuid);

	bool EquipItem(int64 ItemGuid);
	bool UnequipItem(int64 ItemGuid);

	const FEclipseMaterialWallet& GetWallet() const { return Materials; }
	void SetWallet(const FEclipseMaterialWallet& NewWallet);

	bool TakeRune(int32 RuneId);
	void GiveRune(int32 RuneId, int32 Count = 1);

	// ---- Read API (server + owning client) ----------------------------------------
	const FEclipseItemData* FindItem(int64 ItemGuid) const;
	void GetEquippedItems(TArray<const FEclipseItemData*>& Out) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<FEclipseItemData> GetAllItems() const;

	// ---- Client requests ------------------------------------------------------------
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_EquipItem(int64 ItemGuid);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_UnequipItem(int64 ItemGuid);

	/** Rune is taken from the stash and routed to the Ritual/Effect half by its type. */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_SocketRune(int64 ItemGuid, uint8 PairIndex, int32 RuneId);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_UnsocketRune(int64 ItemGuid, uint8 PairIndex, bool bRitualHalf);

	UPROPERTY(BlueprintAssignable) FEclipseOnInventoryChanged OnInventoryChanged;
	/** Native: fires on server and owning client whenever equipped items or their sockets change. */
	FEclipseOnEquipmentChangedNative OnEquipmentChanged;

	/** Called by fast-array callbacks on the owning client. */
	void HandleReplicatedEntryChanged(const FEclipseInventoryEntry& Entry);

protected:
	virtual void BeginPlay() override;

	UFUNCTION() void OnRep_Materials();

	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (ClampMin = "1"))
	int32 Capacity = 120;

private:
	int32 IndexOf(int64 ItemGuid) const;
	static int64 MakeItemGuid();

	UPROPERTY(Replicated)
	FEclipseInventoryList Items;

	UPROPERTY(ReplicatedUsing = OnRep_Materials)
	FEclipseMaterialWallet Materials;

	UPROPERTY(Replicated)
	TArray<FEclipseRuneStack> RuneStash;
};
