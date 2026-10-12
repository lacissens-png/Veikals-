// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// Items.h
// -----------------------------------------------------------------------------
// Authoritative item model + affix pool + item generation.
//
// ItemInstance is the canonical server-side representation. The UE layer
// mirrors it 1:1 in FEclipseItemData (a USTRUCT) for replication / UI and
// converts at the boundary (EclipseItemConversion.h). Mutation of an item's
// rolled values happens ONLY through functions in this core, on the server.
// -----------------------------------------------------------------------------
#pragma once

#include "EclipseCore/EclipseCoreTypes.h"
#include "EclipseCore/Random.h"

#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>

namespace Eclipse
{
	enum class EItemRarity : u8
	{
		Common,
		Magic,
		Rare,
		Legendary,
		AncestralEldritch,
		Count
	};

	enum class EItemSlot : u8
	{
		MainHand,
		OffHand,
		TwoHand,
		Helm,
		Chest,
		Gloves,
		Legs,
		Boots,
		Amulet,
		Ring,
		Count
	};

	constexpr u32 SlotBit(EItemSlot S) { return 1u << static_cast<u32>(S); }
	constexpr u32 AllSlotsMask = (1u << Idx(EItemSlot::Count)) - 1u;
	constexpr u32 WeaponSlotsMask = SlotBit(EItemSlot::MainHand) | SlotBit(EItemSlot::OffHand) | SlotBit(EItemSlot::TwoHand);
	constexpr u32 ArmorSlotsMask = SlotBit(EItemSlot::Helm) | SlotBit(EItemSlot::Chest) | SlotBit(EItemSlot::Gloves)
		| SlotBit(EItemSlot::Legs) | SlotBit(EItemSlot::Boots);
	constexpr u32 JewelrySlotsMask = SlotBit(EItemSlot::Amulet) | SlotBit(EItemSlot::Ring);

	/** Stat families. An item may never carry two affixes of the same stat. */
	enum class EAffixStat : u16
	{
		Strength,
		Intelligence,
		Dexterity,
		Willpower,
		MaxLife,
		Armor,
		CritChance,
		CritDamage,
		AttackSpeed,
		CooldownReduction,
		ResourceGeneration,
		DamageToVulnerable,
		DamageToStaggered,
		CrowdControlDuration,
		MovementSpeed,
		AllResistance,
		UltimateDamage,
		Count
	};
	constexpr std::size_t AffixStatCount = Idx(EAffixStat::Count);
	using StatMask = std::bitset<AffixStatCount>;

	struct AffixDefinition
	{
		u32 Id = InvalidId;
		EAffixStat Stat = EAffixStat::Strength;
		float MinValue = 0.f;
		float MaxValue = 0.f;
		u32 SlotMask = AllSlotsMask;
		u32 Weight = 100;        // Relative drop weight.
		u16 MinItemLevel = 1;
		std::string DebugName;
	};

	struct AffixRoll
	{
		u32 DefinitionId = InvalidId;
		float Value = 0.f;
		float Quality = 0.f;     // 0..1 position inside [Min, Max] (UI "roll %").
		bool bGreater = false;   // Ancestral "greater affix": value x GreaterAffixMultiplier.

		bool operator==(const AffixRoll& O) const
		{
			return DefinitionId == O.DefinitionId && Value == O.Value && Quality == O.Quality && bGreater == O.bGreater;
		}
		bool operator!=(const AffixRoll& O) const { return !(*this == O); }
	};

	/** Crafting currency. Plain counters so the wallet can be validated atomically. */
	struct MaterialBundle
	{
		u32 SoulDust = 0;
		u32 ObsidianOre = 0;
		u32 PrimordialBlood = 0;

		bool Covers(const MaterialBundle& Cost) const
		{
			return SoulDust >= Cost.SoulDust && ObsidianOre >= Cost.ObsidianOre && PrimordialBlood >= Cost.PrimordialBlood;
		}
		/** Precondition: Covers(Cost). */
		void Subtract(const MaterialBundle& Cost)
		{
			SoulDust -= Cost.SoulDust;
			ObsidianOre -= Cost.ObsidianOre;
			PrimordialBlood -= Cost.PrimordialBlood;
		}
		bool operator==(const MaterialBundle& O) const
		{
			return SoulDust == O.SoulDust && ObsidianOre == O.ObsidianOre && PrimordialBlood == O.PrimordialBlood;
		}
	};

	/** A paired socket: one Ritual (condition) + one Effect (action). 0 = empty. */
	struct SocketPair
	{
		u32 RitualRuneId = InvalidId;
		u32 EffectRuneId = InvalidId;

		bool IsComplete() const { return RitualRuneId != InvalidId && EffectRuneId != InvalidId; }
	};

	struct AspectRoll
	{
		u32 AspectId = InvalidId;
		float Value = 0.f;

		bool IsValid() const { return AspectId != InvalidId; }
	};

	struct ItemInstance
	{
		static constexpr int MaxExplicitAffixes = 4;
		static constexpr int MaxReforgeSelections = 2;
		static constexpr int MaxSocketPairs = 2;

		u64 Guid = 0;
		u32 BaseId = InvalidId;
		EItemSlot Slot = EItemSlot::MainHand;
		EItemRarity Rarity = EItemRarity::Common;
		u16 ItemLevel = 1;
		u64 Seed = 0;

		std::vector<AffixRoll> ExplicitAffixes;
		AspectRoll Aspect;

		/**
		 * Bit i set => explicit affix i was chosen as "Reforgable".
		 * Write-once: after the first SelectReforgeAffixes() the mask is permanent,
		 * and every affix NOT in the mask is immutable for the item's lifetime.
		 */
		u8 ReforgeSelectionMask = 0;
		u32 ReforgeCount = 0;

		std::vector<SocketPair> Sockets;

		bool IsAffixReforgable(int Index) const
		{
			return Index >= 0 && Index < static_cast<int>(ExplicitAffixes.size())
				&& (ReforgeSelectionMask & (1u << Index)) != 0;
		}
	};

	ECLIPSECORE_API int GetExplicitAffixCount(EItemRarity Rarity);
	ECLIPSECORE_API int GetMaxSocketPairs(EItemSlot Slot);

	struct RollTuning
	{
		/** Minimum roll quality per rarity: higher tiers never roll bottom-of-range. */
		float QualityFloor[Idx(EItemRarity::Count)] = { 0.f, 0.f, 0.f, 0.2f, 0.5f };
		float GreaterAffixChance = 0.25f;     // Ancestral only, per affix.
		float GreaterAffixMultiplier = 1.5f;
	};

	/**
	 * Read-only after load. Thread-safe for concurrent const access.
	 * Built from UEclipseItemDatabase rows at startup on the server.
	 */
	class ECLIPSECORE_API AffixPool
	{
	public:
		void Add(const AffixDefinition& Def);
		const AffixDefinition* Find(u32 Id) const;
		std::size_t Num() const { return Defs.size(); }

		/** Weighted pick among defs valid for Slot/ItemLevel whose stat is NOT in Excluded. */
		const AffixDefinition* PickWeighted(EItemSlot Slot, u16 ItemLevel, const StatMask& Excluded, Pcg32& Rng) const;

		/**
		 * Rolls a value for Def. QualityFloorBonus raises the floor (Primordial Blood catalyst).
		 * bAllowGreater lets Ancestral items roll greater affixes.
		 */
		AffixRoll RollValue(const AffixDefinition& Def, EItemRarity Rarity, Pcg32& Rng, float QualityFloorBonus = 0.f) const;

		RollTuning Tuning;

	private:
		std::vector<AffixDefinition> Defs;
		std::unordered_map<u32, std::size_t> IndexById;
	};

	/** Stat mask of all explicit affixes on Item, optionally skipping one index. */
	ECLIPSECORE_API StatMask CollectStats(const ItemInstance& Item, const AffixPool& Pool, int SkipIndex = -1);

	struct ItemGenerationParams
	{
		u64 Guid = 0;
		u32 BaseId = InvalidId;
		EItemSlot Slot = EItemSlot::MainHand;
		EItemRarity Rarity = EItemRarity::Rare;
		u16 ItemLevel = 1;
		u8 SocketPairs = 0;
	};

	/** Server-only: rolls a fresh item. Seed is captured so later rerolls derive from it. */
	ECLIPSECORE_API ItemInstance GenerateItem(const ItemGenerationParams& Params, const AffixPool& Pool, Pcg32& Rng);
}
