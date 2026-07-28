#include "SOItemData.h"

FLinearColor USOItemData::GetRarityColor() const
{
	switch (Rarity)
	{
	case ESOItemRarity::Magic:     return FLinearColor(0.28f, 0.55f, 1.00f, 1.0f);
	case ESOItemRarity::Rare:      return FLinearColor(1.00f, 0.90f, 0.20f, 1.0f);
	case ESOItemRarity::Unique:    return FLinearColor(0.72f, 0.50f, 0.20f, 1.0f);
	case ESOItemRarity::Legendary: return FLinearColor(1.00f, 0.42f, 0.00f, 1.0f);
	case ESOItemRarity::Set:       return FLinearColor(0.10f, 0.85f, 0.30f, 1.0f);
	case ESOItemRarity::Common:
	default:                       return FLinearColor(0.85f, 0.85f, 0.85f, 1.0f);
	}
}

TArray<FString> USOItemData::GetAffixDescriptions() const
{
	TArray<FString> Lines;
	Lines.Reserve(RolledAffixes.Num());
	for (const FSOItemAffix& Affix : RolledAffixes)
	{
		Lines.Add(Affix.Description);
	}
	return Lines;
}
