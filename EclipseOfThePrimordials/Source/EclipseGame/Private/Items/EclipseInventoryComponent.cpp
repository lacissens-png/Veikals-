// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Items/EclipseInventoryComponent.h"

#include "EclipseCore/Runewords.h"
#include "Items/EclipseItemDatabase.h"
#include "Net/UnrealNetwork.h"

// =============================================================================
// Fast array callbacks (owning client only)
// =============================================================================

void FEclipseInventoryEntry::PostReplicatedAdd(const FEclipseInventoryList& List)
{
	if (List.Owner) { List.Owner->HandleReplicatedEntryChanged(*this); }
}

void FEclipseInventoryEntry::PostReplicatedChange(const FEclipseInventoryList& List)
{
	if (List.Owner) { List.Owner->HandleReplicatedEntryChanged(*this); }
}

void FEclipseInventoryEntry::PreReplicatedRemove(const FEclipseInventoryList& List)
{
	if (List.Owner) { List.Owner->HandleReplicatedEntryChanged(*this); }
}

// =============================================================================

UEclipseInventoryComponent::UEclipseInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// Set here (not only BeginPlay): initial replication can arrive before BeginPlay on clients.
	Items.Owner = this;
}

void UEclipseInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UEclipseInventoryComponent, Items, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UEclipseInventoryComponent, Materials, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UEclipseInventoryComponent, RuneStash, COND_OwnerOnly);
}

void UEclipseInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Items.Owner = this;
}

void UEclipseInventoryComponent::HandleReplicatedEntryChanged(const FEclipseInventoryEntry& Entry)
{
	OnInventoryChanged.Broadcast();
	// Coarse but cheap: equipment rebuilds are event-driven and rare (equip, socket).
	OnEquipmentChanged.Broadcast();
}

void UEclipseInventoryComponent::OnRep_Materials()
{
	OnInventoryChanged.Broadcast();
}

int64 UEclipseInventoryComponent::MakeItemGuid()
{
	// 128-bit random GUID folded to 64 bits: collision probability is negligible
	// at any realistic item count, and needs no central allocator.
	const FGuid G = FGuid::NewGuid();
	const uint64 Hi = (static_cast<uint64>(G.A) << 32) | G.B;
	const uint64 Lo = (static_cast<uint64>(G.C) << 32) | G.D;
	const int64 Id = static_cast<int64>(Hi ^ Lo);
	return Id == 0 ? 1 : Id;
}

int32 UEclipseInventoryComponent::IndexOf(int64 ItemGuid) const
{
	return Items.Entries.IndexOfByPredicate([ItemGuid](const FEclipseInventoryEntry& E) { return E.Item.ItemGuid == ItemGuid; });
}

const FEclipseItemData* UEclipseInventoryComponent::FindItem(int64 ItemGuid) const
{
	const int32 I = IndexOf(ItemGuid);
	return I == INDEX_NONE ? nullptr : &Items.Entries[I].Item;
}

FEclipseItemData* UEclipseInventoryComponent::FindItemMutable(int64 ItemGuid)
{
	if (!GetOwner()->HasAuthority())
	{
		return nullptr;
	}
	const int32 I = IndexOf(ItemGuid);
	return I == INDEX_NONE ? nullptr : &Items.Entries[I].Item;
}

TArray<FEclipseItemData> UEclipseInventoryComponent::GetAllItems() const
{
	TArray<FEclipseItemData> Out;
	Out.Reserve(Items.Entries.Num());
	for (const FEclipseInventoryEntry& E : Items.Entries)
	{
		Out.Add(E.Item);
	}
	return Out;
}

void UEclipseInventoryComponent::GetEquippedItems(TArray<const FEclipseItemData*>& Out) const
{
	for (const FEclipseInventoryEntry& E : Items.Entries)
	{
		if (E.Item.bEquipped)
		{
			Out.Add(&E.Item);
		}
	}
}

int64 UEclipseInventoryComponent::AddItem(FEclipseItemData Item)
{
	if (!GetOwner()->HasAuthority() || Items.Entries.Num() >= Capacity)
	{
		return 0;
	}
	if (Item.ItemGuid == 0 || IndexOf(Item.ItemGuid) != INDEX_NONE)
	{
		Item.ItemGuid = MakeItemGuid();
	}
	Item.bEquipped = false;

	FEclipseInventoryEntry& Entry = Items.Entries.AddDefaulted_GetRef();
	Entry.Item = MoveTemp(Item);
	Items.MarkItemDirty(Entry);
	OnInventoryChanged.Broadcast();
	return Entry.Item.ItemGuid;
}

bool UEclipseInventoryComponent::RemoveItem(int64 ItemGuid, FEclipseItemData* OutRemoved)
{
	const int32 I = GetOwner()->HasAuthority() ? IndexOf(ItemGuid) : INDEX_NONE;
	if (I == INDEX_NONE)
	{
		return false;
	}
	const bool bWasEquipped = Items.Entries[I].Item.bEquipped;
	if (OutRemoved)
	{
		*OutRemoved = Items.Entries[I].Item;
	}
	Items.Entries.RemoveAtSwap(I);
	Items.MarkArrayDirty();
	OnInventoryChanged.Broadcast();
	if (bWasEquipped)
	{
		OnEquipmentChanged.Broadcast();
	}
	return true;
}

void UEclipseInventoryComponent::MarkItemChanged(int64 ItemGuid)
{
	const int32 I = GetOwner()->HasAuthority() ? IndexOf(ItemGuid) : INDEX_NONE;
	if (I == INDEX_NONE)
	{
		return;
	}
	Items.MarkItemDirty(Items.Entries[I]);
	OnInventoryChanged.Broadcast();
	if (Items.Entries[I].Item.bEquipped)
	{
		OnEquipmentChanged.Broadcast();
	}
}

bool UEclipseInventoryComponent::EquipItem(int64 ItemGuid)
{
	const int32 I = GetOwner()->HasAuthority() ? IndexOf(ItemGuid) : INDEX_NONE;
	if (I == INDEX_NONE || Items.Entries[I].Item.bEquipped)
	{
		return false;
	}

	const EEclipseItemSlot Slot = Items.Entries[I].Item.Slot;
	const int32 SlotCapacity = Slot == EEclipseItemSlot::Ring ? 2 : 1;

	// Resolve conflicts: same slot (respecting ring capacity) and 2H vs 1H/OH.
	int32 SameSlotEquipped = 0;
	for (FEclipseInventoryEntry& E : Items.Entries)
	{
		if (!E.Item.bEquipped)
		{
			continue;
		}
		const bool bTwoHandConflict =
			(Slot == EEclipseItemSlot::TwoHand && (E.Item.Slot == EEclipseItemSlot::MainHand || E.Item.Slot == EEclipseItemSlot::OffHand))
			|| ((Slot == EEclipseItemSlot::MainHand || Slot == EEclipseItemSlot::OffHand) && E.Item.Slot == EEclipseItemSlot::TwoHand);
		const bool bSlotFull = E.Item.Slot == Slot && ++SameSlotEquipped >= SlotCapacity;
		if (bTwoHandConflict || bSlotFull)
		{
			E.Item.bEquipped = false;
			Items.MarkItemDirty(E);
		}
	}

	Items.Entries[I].Item.bEquipped = true;
	Items.MarkItemDirty(Items.Entries[I]);
	OnInventoryChanged.Broadcast();
	OnEquipmentChanged.Broadcast();
	return true;
}

bool UEclipseInventoryComponent::UnequipItem(int64 ItemGuid)
{
	const int32 I = GetOwner()->HasAuthority() ? IndexOf(ItemGuid) : INDEX_NONE;
	if (I == INDEX_NONE || !Items.Entries[I].Item.bEquipped)
	{
		return false;
	}
	Items.Entries[I].Item.bEquipped = false;
	Items.MarkItemDirty(Items.Entries[I]);
	OnInventoryChanged.Broadcast();
	OnEquipmentChanged.Broadcast();
	return true;
}

void UEclipseInventoryComponent::SetWallet(const FEclipseMaterialWallet& NewWallet)
{
	if (GetOwner()->HasAuthority())
	{
		Materials = NewWallet;
		OnInventoryChanged.Broadcast();
	}
}

bool UEclipseInventoryComponent::TakeRune(int32 RuneId)
{
	if (!GetOwner()->HasAuthority())
	{
		return false;
	}
	const int32 I = RuneStash.IndexOfByPredicate([RuneId](const FEclipseRuneStack& S) { return S.RuneId == RuneId; });
	if (I == INDEX_NONE || RuneStash[I].Count <= 0)
	{
		return false;
	}
	if (--RuneStash[I].Count == 0)
	{
		RuneStash.RemoveAtSwap(I);
	}
	return true;
}

void UEclipseInventoryComponent::GiveRune(int32 RuneId, int32 Count)
{
	if (!GetOwner()->HasAuthority() || Count <= 0)
	{
		return;
	}
	if (FEclipseRuneStack* S = RuneStash.FindByPredicate([RuneId](const FEclipseRuneStack& X) { return X.RuneId == RuneId; }))
	{
		S->Count += Count;
	}
	else
	{
		FEclipseRuneStack& New = RuneStash.AddDefaulted_GetRef();
		New.RuneId = RuneId;
		New.Count = Count;
	}
}

// =============================================================================
// RPCs - validate everything; the client only supplies ids.
// =============================================================================

void UEclipseInventoryComponent::Server_EquipItem_Implementation(int64 ItemGuid)
{
	EquipItem(ItemGuid);
}

void UEclipseInventoryComponent::Server_UnequipItem_Implementation(int64 ItemGuid)
{
	UnequipItem(ItemGuid);
}

void UEclipseInventoryComponent::Server_SocketRune_Implementation(int64 ItemGuid, uint8 PairIndex, int32 RuneId)
{
	const UEclipseItemDatabase* Db = UEclipseItemDatabase::Get();
	FEclipseItemData* Item = FindItemMutable(ItemGuid);
	if (!Db || !Item)
	{
		return;
	}

	Eclipse::ItemInstance Core = EclipseItems::ToCore(*Item);
	// Validate the socket BEFORE consuming the rune so a failed insert never eats it.
	if (PairIndex >= Core.Sockets.size() || !Db->GetRuneDatabase().Find(static_cast<Eclipse::u32>(RuneId)))
	{
		return;
	}
	const Eclipse::RuneDefinition* Def = Db->GetRuneDatabase().Find(static_cast<Eclipse::u32>(RuneId));
	const Eclipse::SocketPair& Pair = Core.Sockets[PairIndex];
	const bool bOccupied = (Def->Type == Eclipse::ERuneType::Ritual ? Pair.RitualRuneId : Pair.EffectRuneId) != Eclipse::InvalidId;
	if (bOccupied || !TakeRune(RuneId))
	{
		return;
	}

	if (Eclipse::InsertRune(Core, PairIndex, static_cast<Eclipse::u32>(RuneId), Db->GetRuneDatabase()) == Eclipse::ESocketError::None)
	{
		EclipseItems::FromCore(Core, *Item);
		MarkItemChanged(ItemGuid); // -> OnEquipmentChanged -> runeword rebuild if equipped.
	}
	else
	{
		GiveRune(RuneId); // Defensive refund; unreachable given the pre-checks above.
	}
}

void UEclipseInventoryComponent::Server_UnsocketRune_Implementation(int64 ItemGuid, uint8 PairIndex, bool bRitualHalf)
{
	FEclipseItemData* Item = FindItemMutable(ItemGuid);
	if (!Item)
	{
		return;
	}
	Eclipse::ItemInstance Core = EclipseItems::ToCore(*Item);
	Eclipse::u32 Removed = Eclipse::InvalidId;
	if (Eclipse::RemoveRune(Core, PairIndex, bRitualHalf ? Eclipse::ERuneType::Ritual : Eclipse::ERuneType::Effect, Removed)
		== Eclipse::ESocketError::None)
	{
		EclipseItems::FromCore(Core, *Item);
		GiveRune(static_cast<int32>(Removed));
		MarkItemChanged(ItemGuid);
	}
}
