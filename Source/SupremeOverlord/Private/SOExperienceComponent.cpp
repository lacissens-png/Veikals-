#include "SOExperienceComponent.h"

USOExperienceComponent::USOExperienceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOExperienceComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevel     = FMath::Clamp(StartingLevel, 1, MaxLevel);
	XPInCurrentLevel = 0;

	OnExperienceChanged.Broadcast(this, XPInCurrentLevel, GetXPToNextLevel(), CurrentLevel);
}

int32 USOExperienceComponent::ComputeXPForLevel(int32 Level) const
{
	if (Level <= 1)
	{
		return BaseXPForLevel2;
	}
	const float LevelF = static_cast<float>(Level);
	return FMath::Max(1, FMath::RoundToInt(BaseXPForLevel2 * FMath::Pow(LevelF, LevelCurveExponent)));
}

int32 USOExperienceComponent::GetXPToNextLevel() const
{
	if (IsMaxLevel())
	{
		return 0;
	}
	return ComputeXPForLevel(CurrentLevel);
}

float USOExperienceComponent::GetLevelProgressPercent() const
{
	if (IsMaxLevel())
	{
		return 1.0f;
	}
	const int32 Needed = GetXPToNextLevel();
	return Needed > 0 ? FMath::Clamp(static_cast<float>(XPInCurrentLevel) / Needed, 0.0f, 1.0f) : 0.0f;
}

void USOExperienceComponent::GainXP(int32 Amount)
{
	if (Amount <= 0 || IsMaxLevel())
	{
		return;
	}

	XPInCurrentLevel += Amount;

	while (!IsMaxLevel())
	{
		const int32 Threshold = ComputeXPForLevel(CurrentLevel);
		if (XPInCurrentLevel < Threshold)
		{
			break;
		}

		XPInCurrentLevel -= Threshold;

		const int32 PreviousLevel = CurrentLevel;
		++CurrentLevel;
		OnLevelUp.Broadcast(this, CurrentLevel, PreviousLevel);
	}

	if (IsMaxLevel())
	{
		XPInCurrentLevel = 0;
	}

	OnExperienceChanged.Broadcast(this, XPInCurrentLevel, GetXPToNextLevel(), CurrentLevel);
}
