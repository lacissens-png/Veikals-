// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "EclipseCore/Reforge.h"

#include <cmath>

namespace Eclipse
{
	namespace
	{
		bool IsReforgeableRarity(EItemRarity R)
		{
			return R == EItemRarity::Rare || R == EItemRarity::Legendary || R == EItemRarity::AncestralEldritch;
		}

		u32 ScaleCost(u32 Base, float Scale)
		{
			return static_cast<u32>(std::ceil(static_cast<float>(Base) * Scale));
		}
	}

	EReforgeError SelectReforgeAffixes(ItemInstance& Item, const std::vector<u8>& Indices)
	{
		if (!IsReforgeableRarity(Item.Rarity))
		{
			return EReforgeError::ItemNotReforgeable;
		}
		if (Item.ReforgeSelectionMask != 0)
		{
			return EReforgeError::SelectionAlreadyLocked;
		}
		if (Indices.empty())
		{
			return EReforgeError::NoSelection;
		}
		if (Indices.size() > static_cast<std::size_t>(ItemInstance::MaxReforgeSelections))
		{
			return EReforgeError::TooManySelections;
		}

		u8 Mask = 0;
		for (const u8 Index : Indices)
		{
			if (Index >= Item.ExplicitAffixes.size())
			{
				return EReforgeError::InvalidAffixIndex;
			}
			const u8 Bit = static_cast<u8>(1u << Index);
			if (Mask & Bit)
			{
				return EReforgeError::DuplicateSelection;
			}
			Mask = static_cast<u8>(Mask | Bit);
		}

		// Commit only after full validation (all-or-nothing).
		Item.ReforgeSelectionMask = Mask;
		return EReforgeError::None;
	}

	MaterialBundle GetReforgeCost(const ItemInstance& Item, const ReforgeTuning& Tuning)
	{
		MaterialBundle Base;
		switch (Item.Rarity)
		{
		case EItemRarity::Rare:              Base = { 40, 0, 0 }; break;
		case EItemRarity::Legendary:         Base = { 80, 10, 0 }; break;
		case EItemRarity::AncestralEldritch: Base = { 120, 20, 2 }; break;
		default:                             return {};
		}

		const float Scale = 1.f + Tuning.CostGrowthPerReforge * static_cast<float>(Item.ReforgeCount);
		return { ScaleCost(Base.SoulDust, Scale), ScaleCost(Base.ObsidianOre, Scale), ScaleCost(Base.PrimordialBlood, Scale) };
	}

	ReforgeResult ReforgeAffixes(ItemInstance& Item, u8 AffixIndex, MaterialBundle& Wallet, u32 BloodCatalyst,
		const AffixPool& Pool, Pcg32& Rng, const ReforgeTuning& Tuning)
	{
		ReforgeResult Result;

		// ---- Validation (no side effects) -----------------------------------
		if (!IsReforgeableRarity(Item.Rarity))
		{
			Result.Error = EReforgeError::ItemNotReforgeable;
			return Result;
		}
		if (AffixIndex >= Item.ExplicitAffixes.size())
		{
			Result.Error = EReforgeError::InvalidAffixIndex;
			return Result;
		}
		if (Item.ReforgeSelectionMask == 0)
		{
			Result.Error = EReforgeError::NoSelection;
			return Result;
		}
		if (!Item.IsAffixReforgable(AffixIndex))
		{
			// Locked affix: the core guarantee that non-selected lines never change.
			Result.Error = EReforgeError::AffixNotSelected;
			return Result;
		}

		const u32 Catalyst = Min(BloodCatalyst, Tuning.MaxBloodCatalyst);
		MaterialBundle Cost = GetReforgeCost(Item, Tuning);
		Cost.PrimordialBlood += Catalyst;
		if (!Wallet.Covers(Cost))
		{
			Result.Error = EReforgeError::InsufficientMaterials;
			return Result;
		}

		// ---- Roll candidates --------------------------------------------------
		// Exclude stats owned by the OTHER affixes so the item can never end up with
		// duplicate stats. The current slot's own stat stays legal (re-rolling the
		// same stat for a better value is a valid outcome).
		StatMask Excluded = CollectStats(Item, Pool, AffixIndex);
		const float FloorBonus = Tuning.QualityFloorPerBlood * static_cast<float>(Catalyst);

		ReforgeSession Session;
		Session.ItemGuid = Item.Guid;
		Session.AffixIndex = AffixIndex;
		Session.Original = Item.ExplicitAffixes[AffixIndex];

		for (int C = 0; C < ReforgeChoiceCount; ++C)
		{
			const AffixDefinition* Def = Pool.PickWeighted(Item.Slot, Item.ItemLevel, Excluded, Rng);
			if (!Def)
			{
				break; // Fewer than 3 legal stats remain; offer what exists.
			}
			Excluded.set(Idx(Def->Stat)); // Candidates are mutually distinct.
			Session.Choices[Session.NumChoices++] = Pool.RollValue(*Def, Item.Rarity, Rng, FloorBonus);
		}

		if (Session.NumChoices == 0)
		{
			Result.Error = EReforgeError::PoolExhausted; // Nothing charged.
			return Result;
		}

		// ---- Commit cost (single point of mutation for the wallet) ------------
		Wallet.Subtract(Cost);
		Item.ReforgeCount += 1;

		Session.bActive = true;
		Result.Session = Session;
		Result.Spent = Cost;
		return Result;
	}

	EReforgeError CommitReforgeChoice(ItemInstance& Item, ReforgeSession& Session, int ChoiceIndex)
	{
		if (!Session.bActive)
		{
			return EReforgeError::NoPendingSession;
		}
		if (Session.ItemGuid != Item.Guid || !Item.IsAffixReforgable(Session.AffixIndex)
			|| Item.ExplicitAffixes[Session.AffixIndex] != Session.Original)
		{
			// The item was swapped or modified after the session was opened.
			Session.bActive = false;
			return EReforgeError::SessionMismatch;
		}
		if (ChoiceIndex != KeepOriginalChoice && (ChoiceIndex < 0 || ChoiceIndex >= Session.NumChoices))
		{
			// Keep the session open: an out-of-range index is a client error, not a forfeit.
			return EReforgeError::InvalidChoice;
		}

		if (ChoiceIndex != KeepOriginalChoice)
		{
			Item.ExplicitAffixes[Session.AffixIndex] = Session.Choices[ChoiceIndex];
		}
		Session.bActive = false;
		return EReforgeError::None;
	}
}
