#include "SODifficultySubsystem.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"

USODifficultySubsystem* USODifficultySubsystem::Get(const UWorld* World)
{
	if (!World)
	{
		return nullptr;
	}
	UGameInstance* GameInstance = World->GetGameInstance();
	return GameInstance ? GameInstance->GetSubsystem<USODifficultySubsystem>() : nullptr;
}

void USODifficultySubsystem::SetDifficultyTier(ESODifficultyTier NewTier)
{
	if (CurrentTier == NewTier)
	{
		return;
	}
	CurrentTier = NewTier;
	OnDifficultyChanged.Broadcast(CurrentTier);
}

void USODifficultySubsystem::CycleDifficulty()
{
	constexpr uint8 NumTiers = static_cast<uint8>(ESODifficultyTier::Torment) + 1;
	const uint8 NextTier = (static_cast<uint8>(CurrentTier) + 1) % NumTiers;
	SetDifficultyTier(static_cast<ESODifficultyTier>(NextTier));
}

FText USODifficultySubsystem::GetDifficultyDisplayName() const
{
	switch (CurrentTier)
	{
	case ESODifficultyTier::Hard:      return FText::FromString(TEXT("Hard"));
	case ESODifficultyTier::Nightmare: return FText::FromString(TEXT("Nightmare"));
	case ESODifficultyTier::Torment:   return FText::FromString(TEXT("Torment"));
	default:                           return FText::FromString(TEXT("Normal"));
	}
}

float USODifficultySubsystem::GetEnemyHealthMultiplier() const
{
	switch (CurrentTier)
	{
	case ESODifficultyTier::Hard:      return 1.8f;
	case ESODifficultyTier::Nightmare: return 3.0f;
	case ESODifficultyTier::Torment:   return 5.0f;
	default:                           return 1.0f;
	}
}

float USODifficultySubsystem::GetEnemyDamageMultiplier() const
{
	switch (CurrentTier)
	{
	case ESODifficultyTier::Hard:      return 1.3f;
	case ESODifficultyTier::Nightmare: return 1.8f;
	case ESODifficultyTier::Torment:   return 2.5f;
	default:                           return 1.0f;
	}
}

float USODifficultySubsystem::GetXPMultiplier() const
{
	switch (CurrentTier)
	{
	case ESODifficultyTier::Hard:      return 1.25f;
	case ESODifficultyTier::Nightmare: return 1.6f;
	case ESODifficultyTier::Torment:   return 2.2f;
	default:                           return 1.0f;
	}
}

float USODifficultySubsystem::GetGoldMultiplier() const
{
	switch (CurrentTier)
	{
	case ESODifficultyTier::Hard:      return 1.15f;
	case ESODifficultyTier::Nightmare: return 1.35f;
	case ESODifficultyTier::Torment:   return 1.6f;
	default:                           return 1.0f;
	}
}

float USODifficultySubsystem::GetItemDropChanceMultiplier() const
{
	switch (CurrentTier)
	{
	case ESODifficultyTier::Hard:      return 1.1f;
	case ESODifficultyTier::Nightmare: return 1.25f;
	case ESODifficultyTier::Torment:   return 1.5f;
	default:                           return 1.0f;
	}
}
