// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Items/EclipseForgeComponent.h"

#include "Characters/EclipsePlayerState.h"
#include "EclipseCore/Random.h"
#include "Items/EclipseInventoryComponent.h"
#include "Items/EclipseItemDatabase.h"
#include "Net/UnrealNetwork.h"

UEclipseForgeComponent::UEclipseForgeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UEclipseForgeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UEclipseForgeComponent, Codex, COND_OwnerOnly);
}

void UEclipseForgeComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority())
	{
		// Per-player secret salt. Never replicated, so even a client that somehow
		// knew an item's seed could not predict its reroll candidates.
		const FGuid G = FGuid::NewGuid();
		ServerSalt = (static_cast<Eclipse::u64>(G.A) << 32 | G.B) ^ (static_cast<Eclipse::u64>(G.C) << 32 | G.D);
	}
}

// =============================================================================
// Helpers
// =============================================================================

UEclipseInventoryComponent* UEclipseForgeComponent::GetInventory() const
{
	return GetOwner()->FindComponentByClass<UEclipseInventoryComponent>();
}

bool UEclipseForgeComponent::PassesRateLimit()
{
	const double Now = GetWorld()->GetTimeSeconds();
	if (LastOpTime >= 0.0 && Now - LastOpTime < MinSecondsBetweenOps)
	{
		return false;
	}
	LastOpTime = Now;
	return true;
}

void UEclipseForgeComponent::Reply(EEclipseForgeOp Op, EEclipseForgeError Error)
{
	Client_ForgeResult(Op, Error);
}

Eclipse::u64 UEclipseForgeComponent::MakeRollSeed(const Eclipse::ItemInstance& Item) const
{
	// Deterministic per (item, reroll #) for auditability; salted for unpredictability.
	return Eclipse::MixSeed(Item.Seed ^ ServerSalt, Item.ReforgeCount);
}

uint8 UEclipseForgeComponent::GetPlayerClassId() const
{
	const AEclipsePlayerState* PS = Cast<AEclipsePlayerState>(GetOwner());
	return PS ? PS->GetClassId() : Eclipse::AnyClass;
}

void UEclipseForgeComponent::SyncCodexFromCore()
{
	Codex.Reset();
	for (const Eclipse::CodexEntry& E : CoreCodex.GetEntries())
	{
		FEclipseCodexEntry& Out = Codex.AddDefaulted_GetRef();
		Out.AspectId = static_cast<int32>(E.AspectId);
		Out.BestValue = E.BestValue;
		Out.TimesExtracted = static_cast<int32>(E.TimesExtracted);
	}
	OnCodexChanged.Broadcast();
}

void UEclipseForgeComponent::RestoreCodex(const TArray<FEclipseCodexEntry>& Saved)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	std::vector<Eclipse::CodexEntry> Entries;
	Entries.reserve(Saved.Num());
	for (const FEclipseCodexEntry& E : Saved)
	{
		Entries.push_back({ static_cast<Eclipse::u32>(E.AspectId), E.BestValue, static_cast<Eclipse::u32>(FMath::Max(0, E.TimesExtracted)) });
	}
	CoreCodex.Restore(Entries);
	SyncCodexFromCore();
}

void UEclipseForgeComponent::OnRep_Codex()
{
	OnCodexChanged.Broadcast();
}

EEclipseForgeError UEclipseForgeComponent::Map(Eclipse::EReforgeError E)
{
	using R = Eclipse::EReforgeError;
	switch (E)
	{
	case R::None:                   return EEclipseForgeError::None;
	case R::ItemNotReforgeable:     return EEclipseForgeError::ItemNotReforgeable;
	case R::InvalidAffixIndex:      return EEclipseForgeError::InvalidAffixIndex;
	case R::TooManySelections:      return EEclipseForgeError::TooManySelections;
	case R::DuplicateSelection:     return EEclipseForgeError::DuplicateSelection;
	case R::SelectionAlreadyLocked: return EEclipseForgeError::SelectionAlreadyLocked;
	case R::NoSelection:            return EEclipseForgeError::NoSelection;
	case R::AffixNotSelected:       return EEclipseForgeError::AffixNotSelected;
	case R::InsufficientMaterials:  return EEclipseForgeError::InsufficientMaterials;
	case R::PoolExhausted:          return EEclipseForgeError::PoolExhausted;
	case R::NoPendingSession:       return EEclipseForgeError::NoPendingSession;
	case R::SessionMismatch:        return EEclipseForgeError::SessionMismatch;
	case R::InvalidChoice:          return EEclipseForgeError::InvalidChoice;
	}
	return EEclipseForgeError::ItemNotFound;
}

EEclipseForgeError UEclipseForgeComponent::Map(Eclipse::ECodexError E)
{
	using C = Eclipse::ECodexError;
	switch (E)
	{
	case C::None:                  return EEclipseForgeError::None;
	case C::SourceNotLegendary:    return EEclipseForgeError::SourceNotLegendary;
	case C::SourceHasNoAspect:     return EEclipseForgeError::SourceHasNoAspect;
	case C::UnknownAspect:         return EEclipseForgeError::UnknownAspect;
	case C::NotInCodex:            return EEclipseForgeError::NotInCodex;
	case C::InvalidTargetRarity:   return EEclipseForgeError::InvalidTargetRarity;
	case C::SlotIncompatible:      return EEclipseForgeError::SlotIncompatible;
	case C::ClassRestricted:       return EEclipseForgeError::ClassRestricted;
	case C::InsufficientMaterials: return EEclipseForgeError::InsufficientMaterials;
	}
	return EEclipseForgeError::ItemNotFound;
}

// =============================================================================
// Reforging
// =============================================================================

void UEclipseForgeComponent::Server_SelectReforgeAffixes_Implementation(int64 ItemGuid, const TArray<uint8>& AffixIndices)
{
	constexpr EEclipseForgeOp Op = EEclipseForgeOp::SelectAffixes;
	if (!PassesRateLimit()) { return Reply(Op, EEclipseForgeError::RateLimited); }

	UEclipseInventoryComponent* Inventory = GetInventory();
	FEclipseItemData* Item = Inventory ? Inventory->FindItemMutable(ItemGuid) : nullptr;
	if (!Item) { return Reply(Op, EEclipseForgeError::ItemNotFound); }

	// Reject oversized arrays before copying (RPC payload is attacker-controlled).
	if (AffixIndices.Num() > Eclipse::ItemInstance::MaxReforgeSelections)
	{
		return Reply(Op, EEclipseForgeError::TooManySelections);
	}

	Eclipse::ItemInstance Core = EclipseItems::ToCore(*Item);
	const Eclipse::EReforgeError Err = Eclipse::SelectReforgeAffixes(Core, std::vector<Eclipse::u8>(AffixIndices.GetData(), AffixIndices.GetData() + AffixIndices.Num()));
	if (Err == Eclipse::EReforgeError::None)
	{
		EclipseItems::FromCore(Core, *Item);
		Inventory->MarkItemChanged(ItemGuid);
	}
	Reply(Op, Map(Err));
}

void UEclipseForgeComponent::Server_RequestReforge_Implementation(int64 ItemGuid, uint8 AffixIndex, uint8 BloodCatalyst)
{
	constexpr EEclipseForgeOp Op = EEclipseForgeOp::Reforge;
	if (!PassesRateLimit()) { return Reply(Op, EEclipseForgeError::RateLimited); }

	const UEclipseItemDatabase* Db = UEclipseItemDatabase::Get();
	if (!Db) { return Reply(Op, EEclipseForgeError::NoDatabase); }

	UEclipseInventoryComponent* Inventory = GetInventory();
	FEclipseItemData* Item = Inventory ? Inventory->FindItemMutable(ItemGuid) : nullptr;
	if (!Item) { return Reply(Op, EEclipseForgeError::ItemNotFound); }

	// Opening a new session silently abandons any previous unpaid-for choice
	// (materials for that one were already spent; D4-style "walk away" semantics).
	PendingSession = Eclipse::ReforgeSession();

	Eclipse::ItemInstance Core = EclipseItems::ToCore(*Item);
	Eclipse::MaterialBundle Wallet = EclipseItems::ToCore(Inventory->GetWallet());
	Eclipse::Pcg32 Rng(MakeRollSeed(Core));

	const Eclipse::ReforgeResult Result = Eclipse::ReforgeAffixes(Core, AffixIndex, Wallet, BloodCatalyst, Db->GetAffixPool(), Rng);
	if (!Result.Succeeded())
	{
		return Reply(Op, Map(Result.Error));
	}

	// Commit side effects atomically: wallet, ReforgeCount, session.
	Inventory->SetWallet(EclipseItems::FromCore(Wallet));
	EclipseItems::FromCore(Core, *Item); // Only ReforgeCount changed; affixes untouched until commit.
	Inventory->MarkItemChanged(ItemGuid);
	PendingSession = Result.Session;

	FEclipseReforgeOffer Offer;
	Offer.ItemGuid = ItemGuid;
	Offer.AffixIndex = AffixIndex;
	Offer.Original = Item->Affixes[AffixIndex];
	Offer.Spent = EclipseItems::FromCore(Result.Spent);
	for (int32 C = 0; C < Result.Session.NumChoices; ++C)
	{
		const Eclipse::AffixRoll& R = Result.Session.Choices[C];
		FEclipseAffixRoll& Out = Offer.Choices.AddDefaulted_GetRef();
		Out.DefinitionId = static_cast<int32>(R.DefinitionId);
		Out.Value = R.Value;
		Out.Quality = R.Quality;
		Out.bGreater = R.bGreater;
	}
	Client_ReforgeOffer(Offer);
	Reply(Op, EEclipseForgeError::None);
}

void UEclipseForgeComponent::Server_CommitReforge_Implementation(int64 ItemGuid, int32 ChoiceIndex)
{
	constexpr EEclipseForgeOp Op = EEclipseForgeOp::CommitReforge;
	// No rate limit on commit: it can't generate value, and blocking it would strand a paid session.

	UEclipseInventoryComponent* Inventory = GetInventory();
	FEclipseItemData* Item = Inventory ? Inventory->FindItemMutable(ItemGuid) : nullptr;
	if (!Item) { return Reply(Op, EEclipseForgeError::ItemNotFound); }

	Eclipse::ItemInstance Core = EclipseItems::ToCore(*Item);
	const Eclipse::EReforgeError Err = Eclipse::CommitReforgeChoice(Core, PendingSession, ChoiceIndex);
	if (Err == Eclipse::EReforgeError::None)
	{
		EclipseItems::FromCore(Core, *Item);
		Inventory->MarkItemChanged(ItemGuid);
	}
	Reply(Op, Map(Err));
}

// =============================================================================
// Codex of Power
// =============================================================================

void UEclipseForgeComponent::Server_ExtractAspect_Implementation(int64 ItemGuid)
{
	constexpr EEclipseForgeOp Op = EEclipseForgeOp::ExtractAspect;
	if (!PassesRateLimit()) { return Reply(Op, EEclipseForgeError::RateLimited); }

	const UEclipseItemDatabase* Db = UEclipseItemDatabase::Get();
	if (!Db) { return Reply(Op, EEclipseForgeError::NoDatabase); }

	UEclipseInventoryComponent* Inventory = GetInventory();
	const FEclipseItemData* Item = Inventory ? Inventory->FindItem(ItemGuid) : nullptr;
	if (!Item) { return Reply(Op, EEclipseForgeError::ItemNotFound); }
	if (Item->bEquipped) { return Reply(Op, EEclipseForgeError::ItemEquipped); }

	bool bImproved = false;
	const Eclipse::ECodexError Err = CoreCodex.ExtractAspect(EclipseItems::ToCore(*Item), Db->GetAspectRegistry(), bImproved);
	if (Err == Eclipse::ECodexError::None)
	{
		// Extraction consumes the source item - same server frame, no window for dupes.
		Inventory->RemoveItem(ItemGuid);
		SyncCodexFromCore();
	}
	Reply(Op, Map(Err));
}

void UEclipseForgeComponent::Server_ImbueAspect_Implementation(int64 ItemGuid, int32 AspectId)
{
	constexpr EEclipseForgeOp Op = EEclipseForgeOp::ImbueAspect;
	if (!PassesRateLimit()) { return Reply(Op, EEclipseForgeError::RateLimited); }

	const UEclipseItemDatabase* Db = UEclipseItemDatabase::Get();
	if (!Db) { return Reply(Op, EEclipseForgeError::NoDatabase); }

	UEclipseInventoryComponent* Inventory = GetInventory();
	FEclipseItemData* Item = Inventory ? Inventory->FindItemMutable(ItemGuid) : nullptr;
	if (!Item) { return Reply(Op, EEclipseForgeError::ItemNotFound); }

	Eclipse::ItemInstance Core = EclipseItems::ToCore(*Item);
	Eclipse::MaterialBundle Wallet = EclipseItems::ToCore(Inventory->GetWallet());
	const Eclipse::ECodexError Err = CoreCodex.ImbueAspect(Core, static_cast<Eclipse::u32>(AspectId), GetPlayerClassId(),
		Db->GetAspectRegistry(), Wallet);
	if (Err == Eclipse::ECodexError::None)
	{
		Inventory->SetWallet(EclipseItems::FromCore(Wallet));
		EclipseItems::FromCore(Core, *Item);
		Inventory->MarkItemChanged(ItemGuid);
	}
	Reply(Op, Map(Err));
}

// =============================================================================
// Client
// =============================================================================

void UEclipseForgeComponent::Client_ReforgeOffer_Implementation(const FEclipseReforgeOffer& Offer)
{
	OnReforgeOffer.Broadcast(Offer);
}

void UEclipseForgeComponent::Client_ForgeResult_Implementation(EEclipseForgeOp Op, EEclipseForgeError Error)
{
	OnForgeResult.Broadcast(Op, Error);
}
