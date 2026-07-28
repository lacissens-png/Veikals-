#include "SOSummonComponent.h"

#include "Engine/World.h"
#include "NavigationSystem.h"
#include "SOCharacter.h"
#include "SOManaComponent.h"
#include "SOMinion.h"

USOSummonComponent::USOSummonComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}

void USOSummonComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SummonCooldownRemaining > 0.0f)
	{
		SummonCooldownRemaining -= DeltaTime;
	}
}

bool USOSummonComponent::SummonMinion(FVector TargetLocation, ASOCharacter* Caster)
{
	if (!MinionClass || !Caster)
	{
		return false;
	}
	if (SummonCooldownRemaining > 0.0f)
	{
		return false;
	}
	if (GetActiveCount() >= MaxMinions)
	{
		return false;
	}
	if (ManaCostPerSummon > 0.0f && Caster->ManaComponent)
	{
		if (!Caster->ManaComponent->UseMana(ManaCostPerSummon))
		{
			return false;
		}
	}

	// Try to snap spawn point to the nav mesh
	FVector SpawnLocation = TargetLocation;
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation NavLoc;
		if (NavSys->GetRandomPointInNavigableRadius(TargetLocation, 100.0f, NavLoc))
		{
			SpawnLocation = NavLoc.Location;
		}
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ASOMinion* Minion = GetWorld()->SpawnActor<ASOMinion>(MinionClass, SpawnLocation, FRotator::ZeroRotator, Params);
	if (!Minion)
	{
		return false;
	}

	Minion->OwnerSummonComponent = this;
	ActiveMinions.Add(Minion);
	SummonCooldownRemaining = SummonCooldown;

	OnMinionSummoned.Broadcast(Minion);
	return true;
}

void USOSummonComponent::DismissAll()
{
	for (TWeakObjectPtr<ASOMinion>& Weak : ActiveMinions)
	{
		if (Weak.IsValid())
		{
			Weak->Destroy();
		}
	}
	ActiveMinions.Empty();
}

int32 USOSummonComponent::GetActiveCount()
{
	// Prune destroyed/expired minions
	ActiveMinions.RemoveAll([](const TWeakObjectPtr<ASOMinion>& W)
	{
		if (!W.IsValid())
		{
			return true;
		}
		return !W->IsAlive();
	});
	return ActiveMinions.Num();
}
