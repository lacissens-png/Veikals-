// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// Aspects.h
// -----------------------------------------------------------------------------
// Legendary Aspects and the "Codex of Power".
//
//   Extract:  Legendary / Ancestral item WITH an aspect  ──►  Codex entry
//             (the source item is consumed by the caller on success).
//             The Codex keeps the BEST value ever extracted per aspect.
//
//   Imbue:    Codex entry ──► Rare base     => becomes Legendary
//                         ──► Ancestral base => stays Ancestral, gains/replaces aspect
//             Legendary / Common / Magic targets are rejected.
// -----------------------------------------------------------------------------
#pragma once

#include "EclipseCore/Items.h"

#include <string>
#include <vector>

namespace Eclipse
{
	constexpr u8 AnyClass = 0xFF;

	struct AspectDefinition
	{
		u32 Id = InvalidId;
		u32 SlotMask = AllSlotsMask;
		float MinValue = 0.f;
		float MaxValue = 0.f;
		u8 ClassRestriction = AnyClass; // EClassId value, or AnyClass.
		std::string DebugName;
	};

	class ECLIPSECORE_API AspectRegistry
	{
	public:
		void Add(const AspectDefinition& Def);
		const AspectDefinition* Find(u32 Id) const;

	private:
		std::vector<AspectDefinition> Defs; // Sorted by Id for binary search.
	};

	enum class ECodexError : u8
	{
		None,
		SourceNotLegendary,
		SourceHasNoAspect,
		UnknownAspect,
		NotInCodex,
		InvalidTargetRarity,
		SlotIncompatible,
		ClassRestricted,
		InsufficientMaterials
	};

	struct CodexEntry
	{
		u32 AspectId = InvalidId;
		float BestValue = 0.f;
		u32 TimesExtracted = 0;
	};

	class ECLIPSECORE_API CodexOfPower
	{
	public:
		/**
		 * Records Source's aspect. Does NOT destroy the item; on None the caller
		 * (server inventory) must remove it in the same transaction.
		 * @param bOutImproved  true if this extraction raised the stored best value.
		 */
		ECodexError ExtractAspect(const ItemInstance& Source, const AspectRegistry& Registry, bool& bOutImproved);

		/** Applies a stored aspect to Target at the Codex's best value. Charges Wallet on success. */
		ECodexError ImbueAspect(ItemInstance& Target, u32 AspectId, u8 PlayerClass, const AspectRegistry& Registry,
			MaterialBundle& Wallet) const;

		static MaterialBundle GetImbueCost(const ItemInstance& Target);

		const CodexEntry* Find(u32 AspectId) const;
		const std::vector<CodexEntry>& GetEntries() const { return Entries; }

		/** Load from save game. Entries are re-sorted; duplicates keep the best value. */
		void Restore(const std::vector<CodexEntry>& Saved);

	private:
		CodexEntry* FindMutable(u32 AspectId);

		std::vector<CodexEntry> Entries; // Sorted by AspectId; a few hundred max.
	};
}
