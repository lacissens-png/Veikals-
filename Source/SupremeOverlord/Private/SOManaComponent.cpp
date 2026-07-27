#include "SOManaComponent.h"

USOManaComponent::USOManaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f; // 20Hz - way more than enough for a smooth bar.
}

void USOManaComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentMana = (StartingMana > 0.0f) ? FMath::Min(StartingMana, MaxMana) : MaxMana;
}

void USOManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RegenLockoutRemaining > 0.0f)
	{
		RegenLockoutRemaining = FMath::Max(0.0f, RegenLockoutRemaining - DeltaTime);
		return;
	}

	if (RegenPerSecond <= 0.0f || CurrentMana >= MaxMana)
	{
		return;
	}

	ApplyManaDelta(RegenPerSecond * DeltaTime);
}

bool USOManaComponent::Consume(float Cost)
{
	if (Cost <= 0.0f)
	{
		return true;
	}

	if (!HasEnough(Cost))
	{
		return false;
	}

	ApplyManaDelta(-Cost);
	RegenLockoutRemaining = RegenDelayAfterConsume;
	return true;
}

float USOManaComponent::Restore(float Amount)
{
	if (Amount <= 0.0f)
	{
		return 0.0f;
	}

	const float OldMana = CurrentMana;
	ApplyManaDelta(Amount);
	return CurrentMana - OldMana;
}

void USOManaComponent::RefillToMax()
{
	if (CurrentMana < MaxMana)
	{
		ApplyManaDelta(MaxMana - CurrentMana);
	}
}

void USOManaComponent::ApplyManaDelta(float Delta)
{
	const float OldMana = CurrentMana;
	CurrentMana = FMath::Clamp(OldMana + Delta, 0.0f, MaxMana);

	const float ActualDelta = CurrentMana - OldMana;
	if (FMath::IsNearlyZero(ActualDelta))
	{
		return;
	}

	OnManaChanged.Broadcast(this, OldMana, CurrentMana, ActualDelta);
}
