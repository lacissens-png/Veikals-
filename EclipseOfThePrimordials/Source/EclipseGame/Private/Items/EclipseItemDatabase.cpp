// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Items/EclipseItemDatabase.h"

#include "EclipseGameSettings.h"

UEclipseItemDatabase* UEclipseItemDatabase::Get()
{
	// Loaded once and kept alive by the asset manager / soft-pointer cache.
	return UEclipseGameSettings::Get()->ItemDatabase.LoadSynchronous();
}

void UEclipseItemDatabase::PostLoad()
{
	Super::PostLoad();
	BuildCoreTables();
}

#if WITH_EDITOR
void UEclipseItemDatabase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	BuildCoreTables(); // Live-tune in PIE.
}
#endif

Eclipse::u32 UEclipseItemDatabase::SlotsToMask(const TArray<EEclipseItemSlot>& Slots)
{
	if (Slots.Num() == 0)
	{
		return Eclipse::AllSlotsMask;
	}
	Eclipse::u32 Mask = 0;
	for (const EEclipseItemSlot S : Slots)
	{
		Mask |= Eclipse::SlotBit(static_cast<Eclipse::EItemSlot>(S));
	}
	return Mask;
}

void UEclipseItemDatabase::BuildCoreTables()
{
	CoreAffixPool = Eclipse::AffixPool();
	CoreAspects = Eclipse::AspectRegistry();
	CoreRunes = Eclipse::RuneDatabase();

	for (const FEclipseAffixRow& Row : Affixes)
	{
		Eclipse::AffixDefinition D;
		D.Id = static_cast<Eclipse::u32>(Row.Id);
		D.Stat = static_cast<Eclipse::EAffixStat>(Row.Stat);
		D.MinValue = Row.MinValue;
		D.MaxValue = Row.MaxValue;
		D.SlotMask = SlotsToMask(Row.AllowedSlots);
		D.Weight = static_cast<Eclipse::u32>(FMath::Max(1, Row.Weight));
		D.MinItemLevel = static_cast<Eclipse::u16>(FMath::Clamp(Row.MinItemLevel, 1, 65535));
		CoreAffixPool.Add(D);
	}

	for (const FEclipseAspectRow& Row : Aspects)
	{
		Eclipse::AspectDefinition D;
		D.Id = static_cast<Eclipse::u32>(Row.Id);
		D.SlotMask = SlotsToMask(Row.AllowedSlots);
		D.MinValue = Row.MinValue;
		D.MaxValue = Row.MaxValue;
		D.ClassRestriction = Row.ClassRestriction < 0 ? Eclipse::AnyClass : static_cast<Eclipse::u8>(Row.ClassRestriction);
		CoreAspects.Add(D);
	}

	for (const FEclipseRuneRow& Row : Runes)
	{
		Eclipse::RuneDefinition D;
		D.Id = static_cast<Eclipse::u32>(Row.Id);
		D.Type = static_cast<Eclipse::ERuneType>(Row.Type);
		D.Condition = static_cast<Eclipse::ERitualCondition>(Row.Condition);
		D.Threshold = Row.Threshold;
		D.OfferingGenerated = static_cast<Eclipse::u32>(FMath::Max(0, Row.OfferingGenerated));
		D.Action = static_cast<Eclipse::ERuneEffectAction>(Row.Action);
		D.Param = Row.Param;
		D.OfferingCost = static_cast<Eclipse::u32>(FMath::Max(0, Row.OfferingCost));
		D.Cooldown = Row.Cooldown;
		CoreRunes.Add(D);
	}
}
