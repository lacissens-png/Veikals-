#include "SOBlinkComponent.h"

#include "Engine/World.h"
#include "NavigationSystem.h"
#include "SOCharacter.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"
#include "TimerManager.h"

USOBlinkComponent::USOBlinkComponent()
{
	PrimaryComponentTick.bCanEverTick          = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval          = 0.05f;
}

void USOBlinkComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining = FMath::Max(0.0f, CooldownRemaining - DeltaTime);
	}
}

void USOBlinkComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(InvulnHandle);
	}
	Super::EndPlay(Reason);
}

bool USOBlinkComponent::CanBlink() const
{
	return CooldownRemaining <= 0.0f;
}

bool USOBlinkComponent::Blink(FVector CursorLocation, ASOCharacter* Caster)
{
	if (!CanBlink() || !Caster || !Caster->IsAlive())
	{
		return false;
	}

	if (ManaCost > 0.0f && (!Caster->ManaComponent || !Caster->ManaComponent->HasEnough(ManaCost)))
	{
		return false;
	}

	const FVector Start = Caster->GetActorLocation();

	// Blink is a ground dash — keep the vertical component of the start position.
	FVector Offset = CursorLocation - Start;
	Offset.Z = 0.0f;

	if (Offset.IsNearlyZero())
	{
		Offset = Caster->GetActorForwardVector() * BlinkRange;
		Offset.Z = 0.0f;
	}

	const FVector Direction = Offset.GetSafeNormal();
	const float   Distance  = FMath::Min(Offset.Size(), BlinkRange);

	FVector Destination = Start + Direction * Distance;
	Destination.Z = Start.Z + DestinationHeightOffset;

	if (const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation Projected;
		if (NavSys->ProjectPointToNavigation(Destination, Projected, FVector(NavProjectionExtent)))
		{
			Destination = Projected.Location;
			Destination.Z += DestinationHeightOffset;
		}
	}

	const FRotator KeepRotation = Caster->GetActorRotation();
	if (!Caster->TeleportTo(Destination, KeepRotation, /*bIsATest=*/ false, /*bNoCheck=*/ false))
	{
		return false;
	}

	if (ManaCost > 0.0f)
	{
		Caster->ManaComponent->Consume(ManaCost);
	}

	CooldownRemaining = Cooldown;

	if (InvulnerabilityDuration > 0.0f && Caster->HealthComponent)
	{
		// A blink mid-iframe would otherwise leave the flag stuck on.
		GetWorld()->GetTimerManager().ClearTimer(InvulnHandle);

		InvulnTarget = Caster->HealthComponent;
		Caster->HealthComponent->bInvulnerable = true;

		GetWorld()->GetTimerManager().SetTimer(
			InvulnHandle,
			this, &USOBlinkComponent::ClearInvulnerability,
			InvulnerabilityDuration,
			/*bLoop=*/ false);
	}

	const FVector Landed = Caster->GetActorLocation();
	OnBlinked.Broadcast(Start, Landed);
	OnBlinkedBP(Start, Landed);

	return true;
}

void USOBlinkComponent::ClearInvulnerability()
{
	if (USOHealthComponent* Health = InvulnTarget.Get())
	{
		Health->bInvulnerable = false;
	}
	InvulnTarget.Reset();
}
