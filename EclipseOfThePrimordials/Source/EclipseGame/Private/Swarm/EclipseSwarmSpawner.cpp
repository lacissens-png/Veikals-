// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Swarm/EclipseSwarmSpawner.h"

#include "Engine/World.h"
#include "Swarm/EclipseSwarmSubsystem.h"
#include "TimerManager.h"

AEclipseSwarmSpawner::AEclipseSwarmSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AEclipseSwarmSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority())
	{
		return;
	}
	if (bSpawnOnBeginPlay)
	{
		SpawnWave();
	}
	if (RepeatInterval > 0.f)
	{
		GetWorldTimerManager().SetTimer(RepeatTimer, this, &AEclipseSwarmSpawner::SpawnWave, RepeatInterval, true);
	}
}

void AEclipseSwarmSpawner::SpawnWave()
{
	UEclipseSwarmSubsystem* Swarm = GetWorld()->GetSubsystem<UEclipseSwarmSubsystem>();
	if (!HasAuthority() || !Swarm)
	{
		return;
	}

	const FVector Origin = GetActorLocation();
	for (const FEclipseSpawnWaveEntry& Entry : Wave)
	{
		for (int32 N = 0; N < Entry.Count; ++N)
		{
			// Uniform disc sampling (sqrt) so packs aren't clumped at the centre.
			const float R = Radius * FMath::Sqrt(FMath::FRand());
			const float Theta = FMath::FRand() * UE_TWO_PI;
			const FVector Loc = Origin + FVector(R * FMath::Cos(Theta), R * FMath::Sin(Theta), 0.f);
			if (!Swarm->SpawnMob(Entry.Archetype, Loc).IsSet())
			{
				return; // Hard cap reached.
			}
		}
	}
}
