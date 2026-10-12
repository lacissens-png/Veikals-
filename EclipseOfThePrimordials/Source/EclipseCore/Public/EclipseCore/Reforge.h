// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// Reforge.h
// -----------------------------------------------------------------------------
// Dual-Affix Reforging.
//
// PLAYER FLOW (all server-authoritative; see UEclipseForgeComponent):
//   1. SelectReforgeAffixes(Item, {i, j})
//        Player designates up to TWO explicit affixes as "Reforgable".
//        This choice is WRITE-ONCE: it is stamped into Item.ReforgeSelectionMask
//        and every non-selected affix becomes permanently immutable.
//   2. ReforgeAffixes(Item, SlotIndex, Wallet, Catalyst, Pool, Rng)
//        Validates, charges Soul Dust / Obsidian Ore / Primordial Blood, and
//        returns a ReforgeSession holding 3 randomized candidate rolls drawn
//        from the affix pool. THE ITEM'S AFFIXES ARE NOT MODIFIED YET.
//   3. CommitReforgeChoice(Item, Session, ChoiceIndex | KeepOriginal)
//        Applies the chosen candidate (or keeps the original). The session is
//        stored server-side only; the client sends an index, never a roll, so a
//        modified client cannot inject arbitrary affix values.
//
// INVARIANTS (covered by Tests/CoreTests.cpp):
//   * Non-selected affixes are bit-identical before/after any reforge.
//   * Candidate stats never duplicate a stat already on the item's OTHER affixes.
//   * The 3 candidates are distinct stats from each other.
//   * Materials are charged exactly once, only after every validation passes.
//   * Same item + same seed => same candidates (deterministic / auditable).
// -----------------------------------------------------------------------------
#pragma once

#include "EclipseCore/Items.h"

#include <array>
#include <vector>

namespace Eclipse
{
	enum class EReforgeError : u8
	{
		None,
		ItemNotReforgeable,     // Common / Magic.
		InvalidAffixIndex,
		TooManySelections,      // > 2 indices.
		DuplicateSelection,
		SelectionAlreadyLocked, // Write-once mask already set.
		NoSelection,            // Tried to reroll before selecting.
		AffixNotSelected,       // Tried to reroll a locked (non-selected) affix.
		InsufficientMaterials,
		PoolExhausted,          // No legal candidate exists.
		NoPendingSession,
		SessionMismatch,        // Session is for another item/slot, or item changed since.
		InvalidChoice
	};

	constexpr int ReforgeChoiceCount = 3;
	constexpr int KeepOriginalChoice = -1;

	struct ReforgeSession
	{
		bool bActive = false;
		u64 ItemGuid = 0;
		u8 AffixIndex = 0;
		AffixRoll Original;
		std::array<AffixRoll, ReforgeChoiceCount> Choices{};
		u8 NumChoices = 0;
	};

	struct ReforgeResult
	{
		EReforgeError Error = EReforgeError::None;
		ReforgeSession Session;
		MaterialBundle Spent;

		bool Succeeded() const { return Error == EReforgeError::None; }
	};

	struct ReforgeTuning
	{
		/** Cost escalates by this fraction per previous reroll on the same item. */
		float CostGrowthPerReforge = 0.25f;
		/** Each unit of Primordial Blood catalyst raises the candidates' quality floor. */
		float QualityFloorPerBlood = 0.10f;
		u32 MaxBloodCatalyst = 3;
	};

	/** Step 1. Indices are explicit-affix indices on the item. */
	ECLIPSECORE_API EReforgeError SelectReforgeAffixes(ItemInstance& Item, const std::vector<u8>& Indices);

	/** Material price for the NEXT reroll on Item (excludes optional catalyst). */
	ECLIPSECORE_API MaterialBundle GetReforgeCost(const ItemInstance& Item, const ReforgeTuning& Tuning = {});

	/**
	 * Step 2. Rolls 3 candidates for a selected affix slot.
	 *
	 * @param Wallet         Player materials; charged only on success.
	 * @param BloodCatalyst  Optional extra Primordial Blood (0..MaxBloodCatalyst) to raise quality floor.
	 */
	ECLIPSECORE_API ReforgeResult ReforgeAffixes(ItemInstance& Item, u8 AffixIndex, MaterialBundle& Wallet,
		u32 BloodCatalyst, const AffixPool& Pool, Pcg32& Rng, const ReforgeTuning& Tuning = {});

	/** Step 3. ChoiceIndex in [0, NumChoices) or KeepOriginalChoice. Consumes the session. */
	ECLIPSECORE_API EReforgeError CommitReforgeChoice(ItemInstance& Item, ReforgeSession& Session, int ChoiceIndex);
}
