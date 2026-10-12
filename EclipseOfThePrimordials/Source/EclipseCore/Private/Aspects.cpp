// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "EclipseCore/Aspects.h"

#include <algorithm>

namespace Eclipse
{
	namespace
	{
		template <typename T>
		auto LowerBoundById(std::vector<T>& V, u32 Id, u32 T::*Member)
		{
			return std::lower_bound(V.begin(), V.end(), Id, [Member](const T& E, u32 Key) { return E.*Member < Key; });
		}
	}

	// -------------------------------------------------------------------------
	// AspectRegistry
	// -------------------------------------------------------------------------

	void AspectRegistry::Add(const AspectDefinition& Def)
	{
		if (Def.Id == InvalidId)
		{
			return;
		}
		auto It = LowerBoundById(Defs, Def.Id, &AspectDefinition::Id);
		if (It != Defs.end() && It->Id == Def.Id)
		{
			return; // Duplicate id: data error, first wins.
		}
		Defs.insert(It, Def);
	}

	const AspectDefinition* AspectRegistry::Find(u32 Id) const
	{
		auto It = std::lower_bound(Defs.begin(), Defs.end(), Id,
			[](const AspectDefinition& D, u32 Key) { return D.Id < Key; });
		return (It != Defs.end() && It->Id == Id) ? &*It : nullptr;
	}

	// -------------------------------------------------------------------------
	// CodexOfPower
	// -------------------------------------------------------------------------

	const CodexEntry* CodexOfPower::Find(u32 AspectId) const
	{
		auto It = std::lower_bound(Entries.begin(), Entries.end(), AspectId,
			[](const CodexEntry& E, u32 Key) { return E.AspectId < Key; });
		return (It != Entries.end() && It->AspectId == AspectId) ? &*It : nullptr;
	}

	CodexEntry* CodexOfPower::FindMutable(u32 AspectId)
	{
		return const_cast<CodexEntry*>(static_cast<const CodexOfPower*>(this)->Find(AspectId));
	}

	ECodexError CodexOfPower::ExtractAspect(const ItemInstance& Source, const AspectRegistry& Registry, bool& bOutImproved)
	{
		bOutImproved = false;
		if (Source.Rarity != EItemRarity::Legendary && Source.Rarity != EItemRarity::AncestralEldritch)
		{
			return ECodexError::SourceNotLegendary;
		}
		if (!Source.Aspect.IsValid())
		{
			return ECodexError::SourceHasNoAspect;
		}
		const AspectDefinition* Def = Registry.Find(Source.Aspect.AspectId);
		if (!Def)
		{
			return ECodexError::UnknownAspect;
		}

		const float Value = Clamp(Source.Aspect.Value, Def->MinValue, Def->MaxValue);
		if (CodexEntry* Existing = FindMutable(Def->Id))
		{
			Existing->TimesExtracted += 1;
			if (Value > Existing->BestValue)
			{
				Existing->BestValue = Value;
				bOutImproved = true;
			}
			return ECodexError::None;
		}

		auto It = LowerBoundById(Entries, Def->Id, &CodexEntry::AspectId);
		Entries.insert(It, CodexEntry{ Def->Id, Value, 1 });
		bOutImproved = true;
		return ECodexError::None;
	}

	MaterialBundle CodexOfPower::GetImbueCost(const ItemInstance& Target)
	{
		return Target.Rarity == EItemRarity::AncestralEldritch ? MaterialBundle{ 150, 25, 3 } : MaterialBundle{ 75, 10, 0 };
	}

	ECodexError CodexOfPower::ImbueAspect(ItemInstance& Target, u32 AspectId, u8 PlayerClass, const AspectRegistry& Registry,
		MaterialBundle& Wallet) const
	{
		if (Target.Rarity != EItemRarity::Rare && Target.Rarity != EItemRarity::AncestralEldritch)
		{
			return ECodexError::InvalidTargetRarity;
		}
		const CodexEntry* Entry = Find(AspectId);
		if (!Entry)
		{
			return ECodexError::NotInCodex;
		}
		const AspectDefinition* Def = Registry.Find(AspectId);
		if (!Def)
		{
			return ECodexError::UnknownAspect;
		}
		if ((Def->SlotMask & SlotBit(Target.Slot)) == 0)
		{
			return ECodexError::SlotIncompatible;
		}
		if (Def->ClassRestriction != AnyClass && Def->ClassRestriction != PlayerClass)
		{
			return ECodexError::ClassRestricted;
		}
		const MaterialBundle Cost = GetImbueCost(Target);
		if (!Wallet.Covers(Cost))
		{
			return ECodexError::InsufficientMaterials;
		}

		Wallet.Subtract(Cost);
		Target.Aspect = AspectRoll{ AspectId, Entry->BestValue };
		if (Target.Rarity == EItemRarity::Rare)
		{
			Target.Rarity = EItemRarity::Legendary; // The upgrade.
		}
		return ECodexError::None;
	}

	void CodexOfPower::Restore(const std::vector<CodexEntry>& Saved)
	{
		Entries.clear();
		for (const CodexEntry& E : Saved)
		{
			if (E.AspectId == InvalidId)
			{
				continue;
			}
			if (CodexEntry* Existing = FindMutable(E.AspectId))
			{
				Existing->BestValue = Max(Existing->BestValue, E.BestValue);
				Existing->TimesExtracted += E.TimesExtracted;
				continue;
			}
			Entries.insert(LowerBoundById(Entries, E.AspectId, &CodexEntry::AspectId), E);
		}
	}
}
