#include "SOSummonComponent.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "SOCharacter.h"
#include "SOEnemyCharacter.h"
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
	if (NecromancyCooldownRemaining > 0.0f)
	{
		NecromancyCooldownRemaining -= DeltaTime;
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

bool USOSummonComponent::ResurrectAtLocation(FVector CursorLocation, ASOCharacter* Caster)
{
	if (!MinionClass || !Caster || NecromancyCooldownRemaining > 0.0f)
	{
		return false;
	}
	if (GetActiveCount() >= MaxMinions)
	{
		return false;
	}
	if (ManaCostPerResurrect > 0.0f && Caster->ManaComponent)
	{
		if (!Caster->ManaComponent->UseMana(ManaCostPerResurrect))
		{
			return false;
		}
	}

	// Find the nearest dead ASOEnemyCharacter within ResurrectRange of the cursor.
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASOEnemyCharacter::StaticClass(), AllEnemies);

	ASOEnemyCharacter* Target   = nullptr;
	float BestDistSq = ResurrectRange * ResurrectRange;

	for (AActor* A : AllEnemies)
	{
		ASOEnemyCharacter* Enemy = Cast<ASOEnemyCharacter>(A);
		if (!Enemy || Enemy->IsAlive() || !Enemy->bCanBeResurrected)
		{
			continue;
		}
		const float DistSq = FVector::DistSquared(CursorLocation, Enemy->GetActorLocation());
		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			Target     = Enemy;
		}
	}

	if (!Target)
	{
		return false;
	}

	const FVector SpawnLocation = Target->GetActorLocation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ASOMinion* Minion = GetWorld()->SpawnActor<ASOMinion>(MinionClass, SpawnLocation, FRotator::ZeroRotator, Params);
	if (!Minion)
	{
		return false;
	}

	// Override actor lifespan so the resurrected minion expires on its own.
	if (ResurrectedMinionLifetime > 0.0f)
	{
		Minion->SetLifeSpan(ResurrectedMinionLifetime);
	}

	Minion->OwnerSummonComponent = this;
	ActiveMinions.Add(Minion);
	NecromancyCooldownRemaining = NecromancyCooldown;

	OnMinionSummoned.Broadcast(Minion);
	return true;
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
