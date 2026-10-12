// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Items/EclipseItemTypes.h"

namespace EclipseItems
{
	Eclipse::ItemInstance ToCore(const FEclipseItemData& In)
	{
		Eclipse::ItemInstance Out;
		Out.Guid = static_cast<Eclipse::u64>(In.ItemGuid);
		Out.BaseId = static_cast<Eclipse::u32>(In.BaseId);
		Out.Slot = static_cast<Eclipse::EItemSlot>(In.Slot);
		Out.Rarity = static_cast<Eclipse::EItemRarity>(In.Rarity);
		Out.ItemLevel = static_cast<Eclipse::u16>(FMath::Clamp(In.ItemLevel, 1, 65535));
		Out.Seed = static_cast<Eclipse::u64>(In.Seed);

		Out.ExplicitAffixes.reserve(In.Affixes.Num());
		for (const FEclipseAffixRoll& A : In.Affixes)
		{
			Out.ExplicitAffixes.push_back({ static_cast<Eclipse::u32>(A.DefinitionId), A.Value, A.Quality, A.bGreater });
		}

		Out.Aspect = { static_cast<Eclipse::u32>(In.AspectId), In.AspectValue };
		Out.ReforgeSelectionMask = In.ReforgeSelectionMask;
		Out.ReforgeCount = static_cast<Eclipse::u32>(FMath::Max(0, In.ReforgeCount));

		Out.Sockets.reserve(In.Sockets.Num());
		for (const FEclipseSocketPair& S : In.Sockets)
		{
			Out.Sockets.push_back({ static_cast<Eclipse::u32>(S.RitualRuneId), static_cast<Eclipse::u32>(S.EffectRuneId) });
		}
		return Out;
	}

	void FromCore(const Eclipse::ItemInstance& In, FEclipseItemData& Out)
	{
		Out.ItemGuid = static_cast<int64>(In.Guid);
		Out.BaseId = static_cast<int32>(In.BaseId);
		Out.Slot = static_cast<EEclipseItemSlot>(In.Slot);
		Out.Rarity = static_cast<EEclipseItemRarity>(In.Rarity);
		Out.ItemLevel = In.ItemLevel;
		Out.Seed = static_cast<int64>(In.Seed);

		Out.Affixes.Reset(static_cast<int32>(In.ExplicitAffixes.size()));
		for (const Eclipse::AffixRoll& A : In.ExplicitAffixes)
		{
			FEclipseAffixRoll& R = Out.Affixes.AddDefaulted_GetRef();
			R.DefinitionId = static_cast<int32>(A.DefinitionId);
			R.Value = A.Value;
			R.Quality = A.Quality;
			R.bGreater = A.bGreater;
		}

		Out.AspectId = static_cast<int32>(In.Aspect.AspectId);
		Out.AspectValue = In.Aspect.Value;
		Out.ReforgeSelectionMask = In.ReforgeSelectionMask;
		Out.ReforgeCount = static_cast<int32>(In.ReforgeCount);

		Out.Sockets.Reset(static_cast<int32>(In.Sockets.size()));
		for (const Eclipse::SocketPair& S : In.Sockets)
		{
			FEclipseSocketPair& P = Out.Sockets.AddDefaulted_GetRef();
			P.RitualRuneId = static_cast<int32>(S.RitualRuneId);
			P.EffectRuneId = static_cast<int32>(S.EffectRuneId);
		}
	}

	Eclipse::MaterialBundle ToCore(const FEclipseMaterialWallet& In)
	{
		return { static_cast<Eclipse::u32>(FMath::Max(0, In.SoulDust)), static_cast<Eclipse::u32>(FMath::Max(0, In.ObsidianOre)),
			static_cast<Eclipse::u32>(FMath::Max(0, In.PrimordialBlood)) };
	}

	FEclipseMaterialWallet FromCore(const Eclipse::MaterialBundle& In)
	{
		FEclipseMaterialWallet Out;
		Out.SoulDust = static_cast<int32>(In.SoulDust);
		Out.ObsidianOre = static_cast<int32>(In.ObsidianOre);
		Out.PrimordialBlood = static_cast<int32>(In.PrimordialBlood);
		return Out;
	}
}
