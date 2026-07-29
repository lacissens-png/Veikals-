#include "SOGemData.h"

float USOGemData::GetEffectiveValue() const
{
	switch (Tier)
	{
	case ESOGemTier::Chipped:  return Value * 0.5f;
	case ESOGemTier::Flawed:   return Value * 0.75f;
	case ESOGemTier::Regular:  return Value;
	case ESOGemTier::Flawless: return Value * 1.5f;
	case ESOGemTier::Perfect:  return Value * 2.0f;
	default:                   return Value;
	}
}
