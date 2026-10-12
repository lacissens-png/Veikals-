// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "EclipseCore/Items.h"

namespace Eclipse
{
	int GetExplicitAffixCount(EItemRarity Rarity)
	{
		switch (Rarity)
		{
		case EItemRarity::Common:            return 0;
		case EItemRarity::Magic:             return 2;
		case EItemRarity::Rare:              return 3;
		case EItemRarity::Legendary:         return 4;
		case EItemRarity::AncestralEldritch: return 4;
		default:                             return 0;
		}
	}

	int GetMaxSocketPairs(EItemSlot Slot)
	{
		if (Slot == EItemSlot::TwoHand)
		{
			return 2;
		}
		if ((SlotBit(Slot) & (WeaponSlotsMask | ArmorSlotsMask)) != 0)
		{
			return 1;
		}
		return 0; // Jewelry cannot be socketed.
	}

	void AffixPool::Add(const AffixDefinition& Def)
	{
		if (Def.Id == InvalidId || IndexById.count(Def.Id) != 0)
		{
			return; // Ids are unique; duplicates are a data error and ignored.
		}
		IndexById[Def.Id] = Defs.size();
		Defs.push_back(Def);
	}

	const AffixDefinition* AffixPool::Find(u32 Id) const
	{
		const auto It = IndexById.find(Id);
		return It == IndexById.end() ? nullptr : &Defs[It->second];
	}

	const AffixDefinition* AffixPool::PickWeighted(EItemSlot Slot, u16 ItemLevel, const StatMask& Excluded, Pcg32& Rng) const
	{
		// Two-pass weighted selection over a small table (tens to low hundreds of
		// rows). Avoids building a temporary candidate list.
		u64 TotalWeight = 0;
		for (const AffixDefinition& D : Defs)
		{
			if ((D.SlotMask & SlotBit(Slot)) && ItemLevel >= D.MinItemLevel && !Excluded.test(Idx(D.Stat)))
			{
				TotalWeight += D.Weight;
			}
		}
		if (TotalWeight == 0)
		{
			return nullptr;
		}

		// Weight totals fit in 32 bits for any sane data table.
		u32 Pick = Rng.NextBounded(static_cast<u32>(TotalWeight));
		for (const AffixDefinition& D : Defs)
		{
			if ((D.SlotMask & SlotBit(Slot)) && ItemLevel >= D.MinItemLevel && !Excluded.test(Idx(D.Stat)))
			{
				if (Pick < D.Weight)
				{
					return &D;
				}
				Pick -= D.Weight;
			}
		}
		return nullptr;
	}

	AffixRoll AffixPool::RollValue(const AffixDefinition& Def, EItemRarity Rarity, Pcg32& Rng, float QualityFloorBonus) const
	{
		const float Floor = Clamp(Tuning.QualityFloor[Idx(Rarity)] + QualityFloorBonus, 0.f, 0.95f);
		AffixRoll Roll;
		Roll.DefinitionId = Def.Id;
		Roll.Quality = Floor + (1.f - Floor) * Rng.NextFloat01();
		Roll.Value = Def.MinValue + (Def.MaxValue - Def.MinValue) * Roll.Quality;
		if (Rarity == EItemRarity::AncestralEldritch && Rng.Chance(Tuning.GreaterAffixChance))
		{
			Roll.bGreater = true;
			Roll.Value *= Tuning.GreaterAffixMultiplier;
		}
		return Roll;
	}

	StatMask CollectStats(const ItemInstance& Item, const AffixPool& Pool, int SkipIndex)
	{
		StatMask Mask;
		for (int I = 0; I < static_cast<int>(Item.ExplicitAffixes.size()); ++I)
		{
			if (I == SkipIndex)
			{
				continue;
			}
			if (const AffixDefinition* D = Pool.Find(Item.ExplicitAffixes[I].DefinitionId))
			{
				Mask.set(Idx(D->Stat));
			}
		}
		return Mask;
	}

	ItemInstance GenerateItem(const ItemGenerationParams& Params, const AffixPool& Pool, Pcg32& Rng)
	{
		ItemInstance Item;
		Item.Guid = Params.Guid;
		Item.BaseId = Params.BaseId;
		Item.Slot = Params.Slot;
		Item.Rarity = Params.Rarity;
		Item.ItemLevel = Params.ItemLevel;
		Item.Seed = (static_cast<u64>(Rng.Next()) << 32) | Rng.Next();

		const int AffixCount = GetExplicitAffixCount(Params.Rarity);
		Item.ExplicitAffixes.reserve(AffixCount);

		StatMask Used;
		for (int I = 0; I < AffixCount; ++I)
		{
			const AffixDefinition* Def = Pool.PickWeighted(Params.Slot, Params.ItemLevel, Used, Rng);
			if (!Def)
			{
				break; // Pool exhausted for this slot; item simply has fewer affixes.
			}
			Used.set(Idx(Def->Stat));
			Item.ExplicitAffixes.push_back(Pool.RollValue(*Def, Params.Rarity, Rng));
		}

		// Ancestral guarantee: at least one greater affix.
		if (Params.Rarity == EItemRarity::AncestralEldritch && !Item.ExplicitAffixes.empty())
		{
			bool bAnyGreater = false;
			for (const AffixRoll& R : Item.ExplicitAffixes)
			{
				bAnyGreater |= R.bGreater;
			}
			if (!bAnyGreater)
			{
				AffixRoll& R = Item.ExplicitAffixes[Rng.NextBounded(static_cast<u32>(Item.ExplicitAffixes.size()))];
				R.bGreater = true;
				R.Value *= Pool.Tuning.GreaterAffixMultiplier;
			}
		}

		const int Sockets = Min<int>(Params.SocketPairs, GetMaxSocketPairs(Params.Slot));
		Item.Sockets.resize(static_cast<std::size_t>(Max(0, Sockets)));
		return Item;
	}
}
