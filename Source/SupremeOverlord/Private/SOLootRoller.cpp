#include "SOLootRoller.h"

#include "SOArmorData.h"
#include "SOItemData.h"
#include "SOItemSetData.h"
#include "SOWeaponData.h"
#include "UObject/UObjectGlobals.h"

USOItemData* USOLootRoller::RollItemInstance(const USOItemData* Template, UObject* Outer, int32 ItemLevel)
{
	if (!Template)
	{
		return nullptr;
	}

	UObject* DupeOuter = Outer ? Outer : GetTransientPackage();
	USOItemData* NewItem = DuplicateObject<USOItemData>(Template, DupeOuter);
	if (!NewItem)
	{
		return nullptr;
	}
	NewItem->SourceTemplate = const_cast<USOItemData*>(Template);

	// Hand-authored legendaries/uniques are final as designed — no rarity reroll, no affixes.
	if (Template->bIsLegendaryUnique)
	{
		return NewItem;
	}

	// Set items are final too — their power comes from base stats + the assembled set
	// bonus (USOEquipmentComponent), not random affixes. Force the Set rarity tint.
	if (Template->ItemSet)
	{
		NewItem->Rarity = ESOItemRarity::Set;
		return NewItem;
	}

	NewItem->ItemLevel      = FMath::Max(1, ItemLevel);
	NewItem->Rarity         = RollRarity();
	NewItem->RolledAffixes.Reset();

	TArray<ESOAffixStat> Pool;
	if (Cast<USOWeaponData>(NewItem))
	{
		Pool = { ESOAffixStat::PrimaryDamage, ESOAffixStat::ShadowBoltDamage, ESOAffixStat::AttackSpeed, ESOAffixStat::CritChance };
	}
	else if (Cast<USOArmorData>(NewItem))
	{
		Pool = { ESOAffixStat::MaxHealth, ESOAffixStat::MaxMana, ESOAffixStat::MovementSpeed, ESOAffixStat::DamageReduction };
	}

	const int32 NumAffixes = FMath::Min(GetAffixCountForRarity(NewItem->Rarity), Pool.Num());

	for (int32 i = 0; i < NumAffixes; ++i)
	{
		const int32 PoolIndex = FMath::RandRange(0, Pool.Num() - 1);
		const ESOAffixStat Stat = Pool[PoolIndex];
		Pool.RemoveAt(PoolIndex); // no duplicate affix stat on the same item

		FSOItemAffix Affix;
		Affix.Stat        = Stat;
		Affix.Value       = RollAffixMagnitude(Stat, NewItem->ItemLevel);
		Affix.Description = BuildAffixDescription(Stat, Affix.Value);

		ApplyAffix(NewItem, Affix);
		NewItem->RolledAffixes.Add(Affix);
	}

	return NewItem;
}

USOItemData* USOLootRoller::ReconstructItemInstance(const USOItemData* Template, UObject* Outer, ESOItemRarity Rarity,
                                                      int32 ItemLevel, const TArray<FSOItemAffix>& Affixes)
{
	if (!Template)
	{
		return nullptr;
	}

	UObject* DupeOuter = Outer ? Outer : GetTransientPackage();
	USOItemData* NewItem = DuplicateObject<USOItemData>(Template, DupeOuter);
	if (!NewItem)
	{
		return nullptr;
	}

	NewItem->SourceTemplate = const_cast<USOItemData*>(Template);
	NewItem->Rarity         = Rarity;
	NewItem->ItemLevel      = FMath::Max(1, ItemLevel);
	NewItem->RolledAffixes.Reset();

	for (const FSOItemAffix& Affix : Affixes)
	{
		ApplyAffix(NewItem, Affix);
		NewItem->RolledAffixes.Add(Affix);
	}

	return NewItem;
}

ESOItemRarity USOLootRoller::RollRarity()
{
	const float Roll = FMath::FRand();
	if (Roll < 0.03f) return ESOItemRarity::Legendary; // 3%
	if (Roll < 0.15f) return ESOItemRarity::Rare;       // 12%
	if (Roll < 0.45f) return ESOItemRarity::Magic;      // 30%
	return ESOItemRarity::Common;                        // 55%
}

int32 USOLootRoller::GetAffixCountForRarity(ESOItemRarity Rarity)
{
	switch (Rarity)
	{
	case ESOItemRarity::Legendary: return 4;
	case ESOItemRarity::Rare:      return 2;
	case ESOItemRarity::Magic:     return 1;
	default:                       return 0;
	}
}

float USOLootRoller::RollAffixMagnitude(ESOAffixStat Stat, int32 ItemLevel)
{
	const float Level = static_cast<float>(FMath::Max(1, ItemLevel));
	switch (Stat)
	{
	case ESOAffixStat::PrimaryDamage:    return FMath::FRandRange(2.0f, 6.0f) * Level;
	case ESOAffixStat::ShadowBoltDamage: return FMath::FRandRange(2.0f, 6.0f) * Level;
	case ESOAffixStat::AttackSpeed:      return FMath::FRandRange(0.03f, 0.10f);
	case ESOAffixStat::MaxHealth:        return FMath::FRandRange(5.0f, 15.0f) * Level;
	case ESOAffixStat::MaxMana:          return FMath::FRandRange(3.0f, 10.0f) * Level;
	case ESOAffixStat::MovementSpeed:    return FMath::FRandRange(0.02f, 0.08f);
	case ESOAffixStat::DamageReduction:  return FMath::FRandRange(0.02f, 0.06f);
	case ESOAffixStat::CritChance:       return FMath::FRandRange(0.02f, 0.06f);
	default:                             return 0.0f;
	}
}

FString USOLootRoller::BuildAffixDescription(ESOAffixStat Stat, float Value)
{
	switch (Stat)
	{
	case ESOAffixStat::PrimaryDamage:    return FString::Printf(TEXT("+%.0f Primary Damage"), Value);
	case ESOAffixStat::ShadowBoltDamage: return FString::Printf(TEXT("+%.0f Shadow Bolt Damage"), Value);
	case ESOAffixStat::AttackSpeed:      return FString::Printf(TEXT("+%.0f%% Attack Speed"), Value * 100.0f);
	case ESOAffixStat::MaxHealth:        return FString::Printf(TEXT("+%.0f Maximum Health"), Value);
	case ESOAffixStat::MaxMana:          return FString::Printf(TEXT("+%.0f Maximum Mana"), Value);
	case ESOAffixStat::MovementSpeed:    return FString::Printf(TEXT("+%.0f%% Movement Speed"), Value * 100.0f);
	case ESOAffixStat::DamageReduction:  return FString::Printf(TEXT("+%.0f%% Damage Reduction"), Value * 100.0f);
	case ESOAffixStat::CritChance:       return FString::Printf(TEXT("+%.0f%% Critical Hit Chance"), Value * 100.0f);
	default:                             return FString();
	}
}

void USOLootRoller::ApplyAffix(USOItemData* Item, const FSOItemAffix& Affix)
{
	if (USOWeaponData* Weapon = Cast<USOWeaponData>(Item))
	{
		switch (Affix.Stat)
		{
		case ESOAffixStat::PrimaryDamage:    Weapon->PrimaryDamageBonus    += Affix.Value; return;
		case ESOAffixStat::ShadowBoltDamage: Weapon->ShadowBoltDamageBonus += Affix.Value; return;
		case ESOAffixStat::AttackSpeed:
			Weapon->PrimaryAttackCooldownMultiplier = FMath::Max(0.1f, Weapon->PrimaryAttackCooldownMultiplier - Affix.Value);
			return;
		case ESOAffixStat::CritChance:
			Weapon->CritChanceBonus = FMath::Clamp(Weapon->CritChanceBonus + Affix.Value, 0.0f, 1.0f);
			return;
		default: return;
		}
	}

	if (USOArmorData* Armor = Cast<USOArmorData>(Item))
	{
		switch (Affix.Stat)
		{
		case ESOAffixStat::MaxHealth: Armor->MaxHealthBonus += Affix.Value; return;
		case ESOAffixStat::MaxMana:   Armor->MaxManaBonus   += Affix.Value; return;
		case ESOAffixStat::MovementSpeed:
			Armor->MovementSpeedMultiplier = FMath::Clamp(Armor->MovementSpeedMultiplier + Affix.Value, 0.25f, 2.0f);
			return;
		case ESOAffixStat::DamageReduction:
			Armor->DamageReductionPct = FMath::Clamp(Armor->DamageReductionPct + Affix.Value, 0.0f, 0.9f);
			return;
		default: return;
		}
	}
}
