// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "EclipseCore/Runewords.h"

#include <algorithm>

namespace Eclipse
{
	// -------------------------------------------------------------------------
	// RuneDatabase
	// -------------------------------------------------------------------------

	void RuneDatabase::Add(const RuneDefinition& Def)
	{
		if (Def.Id == InvalidId)
		{
			return;
		}
		auto It = std::lower_bound(Defs.begin(), Defs.end(), Def.Id,
			[](const RuneDefinition& D, u32 Key) { return D.Id < Key; });
		if (It != Defs.end() && It->Id == Def.Id)
		{
			return;
		}
		Defs.insert(It, Def);
	}

	const RuneDefinition* RuneDatabase::Find(u32 Id) const
	{
		auto It = std::lower_bound(Defs.begin(), Defs.end(), Id,
			[](const RuneDefinition& D, u32 Key) { return D.Id < Key; });
		return (It != Defs.end() && It->Id == Id) ? &*It : nullptr;
	}

	// -------------------------------------------------------------------------
	// Socketing
	// -------------------------------------------------------------------------

	ESocketError InsertRune(ItemInstance& Item, u8 PairIndex, u32 RuneId, const RuneDatabase& Db)
	{
		if (PairIndex >= Item.Sockets.size())
		{
			return ESocketError::InvalidPairIndex;
		}
		const RuneDefinition* Def = Db.Find(RuneId);
		if (!Def)
		{
			return ESocketError::UnknownRune;
		}
		SocketPair& Pair = Item.Sockets[PairIndex];
		u32& Slot = (Def->Type == ERuneType::Ritual) ? Pair.RitualRuneId : Pair.EffectRuneId;
		if (Slot != InvalidId)
		{
			return ESocketError::SlotOccupied;
		}
		Slot = RuneId;
		return ESocketError::None;
	}

	ESocketError RemoveRune(ItemInstance& Item, u8 PairIndex, ERuneType Type, u32& OutRuneId)
	{
		OutRuneId = InvalidId;
		if (PairIndex >= Item.Sockets.size())
		{
			return ESocketError::InvalidPairIndex;
		}
		SocketPair& Pair = Item.Sockets[PairIndex];
		u32& Slot = (Type == ERuneType::Ritual) ? Pair.RitualRuneId : Pair.EffectRuneId;
		if (Slot == InvalidId)
		{
			return ESocketError::SlotEmpty;
		}
		OutRuneId = Slot;
		Slot = InvalidId;
		return ESocketError::None;
	}

	ERunewordValidity ValidateRunewordPair(const SocketPair& Pair, const RuneDatabase& Db,
		const RuneDefinition*& OutRitual, const RuneDefinition*& OutEffect)
	{
		OutRitual = nullptr;
		OutEffect = nullptr;
		if (!Pair.IsComplete())
		{
			return ERunewordValidity::Incomplete;
		}
		const RuneDefinition* R = Db.Find(Pair.RitualRuneId);
		const RuneDefinition* E = Db.Find(Pair.EffectRuneId);
		if (!R || !E)
		{
			return ERunewordValidity::UnknownRune;
		}
		// Save data may predate a rune's type change; validate defensively.
		if (R->Type != ERuneType::Ritual)
		{
			return ERunewordValidity::RitualSlotHoldsEffect;
		}
		if (E->Type != ERuneType::Effect)
		{
			return ERunewordValidity::EffectSlotHoldsRitual;
		}
		OutRitual = R;
		OutEffect = E;
		return ERunewordValidity::Valid;
	}

	// -------------------------------------------------------------------------
	// RunewordEvaluator
	// -------------------------------------------------------------------------

	void RunewordEvaluator::Rebuild(const std::vector<const ItemInstance*>& Equipped, const RuneDatabase& Db)
	{
		std::vector<RunewordBinding> Previous;
		Previous.swap(Bindings);
		for (auto& List : ByCondition)
		{
			List.clear();
		}

		for (const ItemInstance* Item : Equipped)
		{
			if (!Item)
			{
				continue;
			}
			for (std::size_t P = 0; P < Item->Sockets.size(); ++P)
			{
				const RuneDefinition* Ritual = nullptr;
				const RuneDefinition* Effect = nullptr;
				if (ValidateRunewordPair(Item->Sockets[P], Db, Ritual, Effect) != ERunewordValidity::Valid)
				{
					continue;
				}

				RunewordBinding B;
				B.ItemGuid = Item->Guid;
				B.PairIndex = static_cast<u8>(P);
				B.Ritual = *Ritual;
				B.Effect = *Effect;

				// Carry over runtime state from an identical previous binding.
				for (const RunewordBinding& Old : Previous)
				{
					if (Old.ItemGuid == B.ItemGuid && Old.PairIndex == B.PairIndex
						&& Old.Ritual.Id == B.Ritual.Id && Old.Effect.Id == B.Effect.Id)
					{
						B.OfferingBank = Old.OfferingBank;
						B.CooldownRemaining = Old.CooldownRemaining;
						break;
					}
				}

				ByCondition[Idx(B.Ritual.Condition)].push_back(static_cast<u16>(Bindings.size()));
				Bindings.push_back(B);
			}
		}
	}

	int RunewordEvaluator::EvaluateRuneword(const RitualEvent& Event, std::vector<RuneEffectInvocation>& Out)
	{
		if (Idx(Event.Condition) >= RitualConditionCount)
		{
			return 0;
		}

		int Fired = 0;
		for (const u16 BindingIndex : ByCondition[Idx(Event.Condition)])
		{
			RunewordBinding& B = Bindings[BindingIndex];
			if (Event.Magnitude < B.Ritual.Threshold)
			{
				continue;
			}

			// Bank is capped at 2x the cost so players can't hoard dozens of procs.
			const u32 Cap = Max<u32>(B.Effect.OfferingCost * 2u, B.Ritual.OfferingGenerated);
			B.OfferingBank = Min(B.OfferingBank + B.Ritual.OfferingGenerated, Cap);

			if (B.CooldownRemaining > 0.f || B.OfferingBank < B.Effect.OfferingCost)
			{
				continue;
			}

			B.OfferingBank -= B.Effect.OfferingCost;
			B.CooldownRemaining = B.Effect.Cooldown;
			Out.push_back({ B.Effect.Action, B.Effect.Param, B.ItemGuid, B.PairIndex });
			++Fired;
		}
		return Fired;
	}

	void RunewordEvaluator::Tick(float Dt)
	{
		for (RunewordBinding& B : Bindings)
		{
			if (B.CooldownRemaining > 0.f)
			{
				B.CooldownRemaining = Max(0.f, B.CooldownRemaining - Dt);
			}
		}
	}
}
