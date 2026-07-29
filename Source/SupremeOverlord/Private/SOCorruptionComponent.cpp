#include "SOCorruptionComponent.h"

USOCorruptionComponent::USOCorruptionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}

void USOCorruptionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bOverlordModeActive)
	{
		OverlordModeTimeRemaining -= DeltaTime;
		if (OverlordModeTimeRemaining <= 0.0f)
		{
			bOverlordModeActive         = false;
			OverlordModeTimeRemaining   = 0.0f;
			CurrentCorruption           = 0.0f;
			OnOverlordModeEnded.Broadcast();
			OnOverlordModeEndedBP();
			OnCorruptionChanged.Broadcast(0.0f);
		}
	}
	else if (DecayRate > 0.0f && CurrentCorruption > 0.0f)
	{
		const float OldFraction = GetCorruptionFraction();
		CurrentCorruption = FMath::Max(0.0f, CurrentCorruption - DecayRate * DeltaTime);
		const float NewFraction = GetCorruptionFraction();
		if (!FMath::IsNearlyEqual(OldFraction, NewFraction, 0.01f))
		{
			OnCorruptionChanged.Broadcast(NewFraction);
		}
	}
}

void USOCorruptionComponent::AddCorruption(float Amount)
{
	if (Amount <= 0.0f || bOverlordModeActive)
	{
		return;
	}

	const float Old = CurrentCorruption;
	CurrentCorruption = FMath::Min(MaxCorruption, CurrentCorruption + Amount);
	if (!FMath::IsNearlyEqual(Old, CurrentCorruption, 0.01f))
	{
		OnCorruptionChanged.Broadcast(GetCorruptionFraction());
	}
}

void USOCorruptionComponent::SetCorruption(float Amount)
{
	bOverlordModeActive       = false;
	OverlordModeTimeRemaining = 0.0f;
	CurrentCorruption         = FMath::Clamp(Amount, 0.0f, MaxCorruption);
	OnCorruptionChanged.Broadcast(GetCorruptionFraction());
}

void USOCorruptionComponent::ActivateOverlordMode()
{
	if (bOverlordModeActive || CurrentCorruption < MaxCorruption)
	{
		return;
	}

	bOverlordModeActive         = true;
	OverlordModeTimeRemaining   = OverlordModeDuration;
	OnOverlordModeStarted.Broadcast();
	OnOverlordModeStartedBP();
}

float USOCorruptionComponent::GetCorruptionFraction() const
{
	return FMath::Clamp(CurrentCorruption / FMath::Max(MaxCorruption, 1.0f), 0.0f, 1.0f);
}

float USOCorruptionComponent::GetOutgoingDamageMultiplier() const
{
	return bOverlordModeActive ? OverlordDamageMultiplier : 1.0f;
}
