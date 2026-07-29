#include "SOEquipmentComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SOArmorData.h"
#include "SOCharacter.h"
#include "SOGemData.h"
#include "SOHealthComponent.h"
#include "SOItemSetData.h"
#include "SOLootRoller.h"
#include "SOManaComponent.h"
#include "SOWeaponData.h"

USOEquipmentComponent::USOEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USOEquipmentComponent::Equip(USOItemData* Item)
{
	if (!Item || Item->EquipSlot == ESOEquipSlot::None)
	{
		return false;
	}

	const ESOEquipSlot Slot = Item->EquipSlot;
	USOItemData* OldItem = EquippedItems.FindRef(Slot);
	if (OldItem == Item)
	{
		return true;
	}

	EquippedItems.Add(Slot, Item);
	OnSlotChanged.Broadcast(Slot, OldItem, Item);

	// Delegate weapon-slot changes back to the character's existing weapon logic
	// so effective PrimaryAttackDamage / ShadowBoltDamage picks it up unchanged.
	if (Slot == ESOEquipSlot::MainHand)
	{
		if (ASOCharacter* Owner = Cast<ASOCharacter>(GetOwner()))
		{
			Owner->EquipWeapon(Cast<USOWeaponData>(Item));
		}
	}

	RecomputeAggregateStats();
	return true;
}

USOItemData* USOEquipmentComponent::Unequip(ESOEquipSlot Slot)
{
	if (Slot == ESOEquipSlot::None)
	{
		return nullptr;
	}

	TObjectPtr<USOItemData> OldItem = nullptr;
	if (EquippedItems.RemoveAndCopyValue(Slot, OldItem))
	{
		OnSlotChanged.Broadcast(Slot, OldItem, nullptr);

		if (Slot == ESOEquipSlot::MainHand)
		{
			if (ASOCharacter* Owner = Cast<ASOCharacter>(GetOwner()))
			{
				Owner->UnequipWeapon();
			}
		}

		RecomputeAggregateStats();
	}
	return OldItem;
}

USOItemData* USOEquipmentComponent::GetItemInSlot(ESOEquipSlot Slot) const
{
	if (const TObjectPtr<USOItemData>* Found = EquippedItems.Find(Slot))
	{
		return Found->Get();
	}
	return nullptr;
}

void USOEquipmentComponent::RecomputeAggregateStats()
{
	ASOCharacter* Owner = Cast<ASOCharacter>(GetOwner());
	if (!Owner)
	{
		return;
	}

	// Sum every equipped armor from scratch, then diff against last-applied so the effect is idempotent.
	float NewHealthBonus     = 0.0f;
	float NewManaBonus       = 0.0f;
	float NewSpeedMultiplier = 1.0f;
	float NewDR              = 0.0f;

	for (const TPair<ESOEquipSlot, TObjectPtr<USOItemData>>& Pair : EquippedItems)
	{
		if (USOArmorData* Armor = Cast<USOArmorData>(Pair.Value.Get()))
		{
			NewHealthBonus     += Armor->MaxHealthBonus;
			NewManaBonus       += Armor->MaxManaBonus;
			NewSpeedMultiplier *= Armor->MovementSpeedMultiplier;
			NewDR               = FMath::Min(0.9f, NewDR + Armor->DamageReductionPct);
		}
	}

	// Count equipped pieces per set, then fold in every bonus tier whose threshold is met.
	TMap<USOItemSetData*, int32> SetCounts;
	for (const TPair<ESOEquipSlot, TObjectPtr<USOItemData>>& Pair : EquippedItems)
	{
		if (Pair.Value && Pair.Value->ItemSet)
		{
			SetCounts.FindOrAdd(Pair.Value->ItemSet)++;
		}
	}

	ActiveSetBonusDescriptions.Reset();

	for (const TPair<USOItemSetData*, int32>& SetPair : SetCounts)
	{
		USOItemSetData* SetData       = SetPair.Key;
		const int32     EquippedCount = SetPair.Value;

		for (const FSOSetBonusTier& Tier : SetData->Tiers)
		{
			if (EquippedCount < Tier.PiecesRequired)
			{
				continue;
			}

			switch (Tier.Bonus.Stat)
			{
			case ESOAffixStat::MaxHealth:       NewHealthBonus     += Tier.Bonus.Value;                    break;
			case ESOAffixStat::MaxMana:         NewManaBonus       += Tier.Bonus.Value;                    break;
			case ESOAffixStat::MovementSpeed:   NewSpeedMultiplier *= (1.0f + Tier.Bonus.Value);           break;
			case ESOAffixStat::DamageReduction: NewDR               = FMath::Min(0.9f, NewDR + Tier.Bonus.Value); break;
			default: break; // PrimaryDamage/ShadowBoltDamage/AttackSpeed aren't supported for set tiers.
			}

			ActiveSetBonusDescriptions.Add(FString::Printf(TEXT("%s (%dpc): %s"),
				*SetData->SetName.ToString(), Tier.PiecesRequired, *Tier.Bonus.Description));
		}
	}

	if (USOHealthComponent* HP = Owner->HealthComponent)
	{
		HP->MaxHealth += (NewHealthBonus - AppliedMaxHealthBonus);
	}
	if (USOManaComponent* MP = Owner->ManaComponent)
	{
		MP->MaxMana += (NewManaBonus - AppliedMaxManaBonus);
	}

	if (AppliedSpeedMultiplier > KINDA_SMALL_NUMBER)
	{
		Owner->MovementSpeed *= (NewSpeedMultiplier / AppliedSpeedMultiplier);
	}
	if (UCharacterMovementComponent* Move = Owner->GetCharacterMovement())
	{
		Move->MaxWalkSpeed = Owner->MovementSpeed;
	}

	// Damage reduction is exposed via the health component's IncomingDamageMultiplier
	// so all existing damage flows automatically honor it.
	if (USOHealthComponent* HP = Owner->HealthComponent)
	{
		HP->IncomingDamageMultiplier = FMath::Max(0.0f, 1.0f - NewDR);
	}

	AppliedMaxHealthBonus     = NewHealthBonus;
	AppliedMaxManaBonus       = NewManaBonus;
	AppliedSpeedMultiplier    = NewSpeedMultiplier;
	AppliedDamageReductionPct = NewDR;
}

bool USOEquipmentComponent::SocketGem(ESOEquipSlot Slot, USOGemData* Gem)
{
	if (!Gem)
	{
		return false;
	}

	USOItemData* Item = GetItemInSlot(Slot);
	if (!Item || Item->GetFreeSocketCount() <= 0)
	{
		return false;
	}

	FSOItemAffix Bonus;
	Bonus.Stat        = Gem->Stat;
	Bonus.Value       = Gem->Value;
	Bonus.Description = Gem->DisplayName.ToString();

	USOLootRoller::ApplyAffix(Item, Bonus);
	Item->SocketedGems.Add(Gem);

	// Re-sync aggregate stats so a gem socketed into armor (Health/Mana/Speed/DR)
	// takes effect immediately; a no-op for weapon-only stats already read live
	// via GetEffective*Damage().
	RecomputeAggregateStats();

	return true;
}
