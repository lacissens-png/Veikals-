#include "SOEnemySpawner.h"

#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "SOEnemyCharacter.h"
#include "TimerManager.h"

ASOEnemySpawner::ASOEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnAreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SpawnAreaSphere"));
	SpawnAreaSphere->InitSphereRadius(SpawnRadius);
	SpawnAreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnAreaSphere->SetHiddenInGame(true);
	SpawnAreaSphere->ShapeColor = FColor(200, 40, 40);
	SetRootComponent(SpawnAreaSphere);

	EditorSprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorSprite"));
	EditorSprite->SetupAttachment(SpawnAreaSphere);
	EditorSprite->SetHiddenInGame(true);
}

void ASOEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnAreaSphere)
	{
		SpawnAreaSphere->SetSphereRadius(SpawnRadius);
	}

	CurrentWave      = 0;
	bAllWavesCleared = false;
	StartWave(1);
}

void ASOEnemySpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
		World->GetTimerManager().ClearTimer(InterWaveTimerHandle);
	}
	Super::EndPlay(EndPlayReason);
}

void ASOEnemySpawner::StartWave(int32 WaveIndex)
{
	if (bAllWavesCleared)
	{
		return;
	}

	CurrentWave     = WaveIndex;
	WaveTargetCount = FMath::Clamp(EnemiesPerWave + EnemiesPerWaveGrowth * (WaveIndex - 1), 1, MaxEnemiesPerWave);
	SpawnedThisWave = 0;

	AliveEnemies.Reset();

	OnWaveStarted.Broadcast(CurrentWave, WaveTargetCount);

	if (UWorld* World = GetWorld())
	{
		if (SpawnStagger > 0.0f)
		{
			// Kick off the first spawn immediately, then stagger the rest on a repeating timer.
			SpawnOne();
			if (SpawnedThisWave < WaveTargetCount)
			{
				World->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASOEnemySpawner::SpawnOne, SpawnStagger, /*bLoop*/ true);
			}
		}
		else
		{
			for (int32 i = 0; i < WaveTargetCount; ++i)
			{
				SpawnOne();
			}
		}
	}
}

void ASOEnemySpawner::SpawnOne()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (SpawnedThisWave >= WaveTargetCount)
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	TSubclassOf<ASOEnemyCharacter> Chosen = PickWeightedEnemy();
	if (Chosen)
	{
		// Random point on a disk around the spawner.
		const float Angle       = FMath::FRandRange(0.0f, 2.0f * PI);
		const float DiskR       = SpawnRadius * FMath::Sqrt(FMath::FRand());
		const FVector Offset    = FVector(FMath::Cos(Angle) * DiskR, FMath::Sin(Angle) * DiskR, 0.0f);
		const FVector SpawnLoc  = GetActorLocation() + Offset + FVector(0.0f, 0.0f, 50.0f);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		Params.Owner = this;

		AActor* Spawned = World->SpawnActor<AActor>(Chosen, SpawnLoc, FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f), Params);
		if (Spawned)
		{
			AliveEnemies.Add(TWeakObjectPtr<AActor>(Spawned));
			Spawned->OnEndPlay.AddDynamic(this, &ASOEnemySpawner::HandleSpawnedEnemyEndPlay);
		}
	}

	++SpawnedThisWave;

	if (SpawnedThisWave >= WaveTargetCount)
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
		// Covers a misconfigured/empty EnemyPool (or every roll landing on a null
		// class): with nothing actually spawned, no enemy EndPlay will ever fire
		// to trigger this, so the wave would otherwise stall forever with 0 alive
		// enemies and never call OnWaveCleared/OnAllWavesCleared.
		CheckWaveCleared();
	}
}

void ASOEnemySpawner::HandleSpawnedEnemyEndPlay(AActor* /*DestroyedActor*/, EEndPlayReason::Type /*EndPlayReason*/)
{
	// AliveEnemies uses weak pointers - the destroyed entry will resolve to null in the next check.
	CheckWaveCleared();
}

void ASOEnemySpawner::CheckWaveCleared()
{
	// Only consider the wave cleared once every planned spawn has actually gone out.
	if (SpawnedThisWave < WaveTargetCount)
	{
		return;
	}

	AliveEnemies.RemoveAll([](const TWeakObjectPtr<AActor>& Ptr) { return !Ptr.IsValid(); });

	if (AliveEnemies.Num() > 0)
	{
		return;
	}

	OnWaveCleared.Broadcast(CurrentWave);

	if (bLimitTotalWaves && CurrentWave >= TotalWaves)
	{
		bAllWavesCleared = true;
		OnAllWavesCleared.Broadcast();
		return;
	}

	StartInterWaveTimer();
}

void ASOEnemySpawner::StartInterWaveTimer()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (InterWaveDelay <= 0.0f)
	{
		StartWave(CurrentWave + 1);
		return;
	}

	World->GetTimerManager().SetTimer(
		InterWaveTimerHandle,
		FTimerDelegate::CreateLambda([this]() { StartWave(CurrentWave + 1); }),
		InterWaveDelay,
		false);
}

TSubclassOf<ASOEnemyCharacter> ASOEnemySpawner::PickWeightedEnemy() const
{
	if (EnemyPool.Num() == 0)
	{
		return nullptr;
	}

	float TotalWeight = 0.0f;
	for (const FSOSpawnEntry& Entry : EnemyPool)
	{
		if (Entry.EnemyClass && Entry.Weight > 0.0f)
		{
			TotalWeight += Entry.Weight;
		}
	}
	if (TotalWeight <= 0.0f)
	{
		return nullptr;
	}

	float Roll = FMath::FRandRange(0.0f, TotalWeight);
	for (const FSOSpawnEntry& Entry : EnemyPool)
	{
		if (!Entry.EnemyClass || Entry.Weight <= 0.0f)
		{
			continue;
		}
		Roll -= Entry.Weight;
		if (Roll <= 0.0f)
		{
			return Entry.EnemyClass;
		}
	}

	// Numerical fallback.
	for (const FSOSpawnEntry& Entry : EnemyPool)
	{
		if (Entry.EnemyClass && Entry.Weight > 0.0f)
		{
			return Entry.EnemyClass;
		}
	}
	return nullptr;
}
