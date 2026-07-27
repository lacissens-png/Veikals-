#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOEnemySpawner.generated.h"

class ASOEnemyCharacter;
class UBillboardComponent;
class USphereComponent;

/** One row in the spawner's enemy pool: which class + relative weight. */
USTRUCT(BlueprintType)
struct FSOSpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry")
	TSubclassOf<ASOEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEntry", meta = (ClampMin = "0.01", UIMin = "0.01", UIMax = "10.0"))
	float Weight = 1.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnWaveStarted,
	int32, WaveIndex,
	int32, EnemyCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnWaveCleared,
	int32, WaveIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSOOnAllWavesCleared);

/**
 * Diablo-lite wave spawner. Spawns EnemiesPerWave enemies in a disc around
 * the spawner, waits until they're all dead, then rests for InterWaveDelay
 * seconds and starts the next wave. EnemiesPerWave grows by EnemiesPerWaveGrowth
 * each wave.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	ASOEnemySpawner();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void StartWave(int32 WaveIndex);
	void SpawnOne();
	void CheckWaveCleared();
	void StartInterWaveTimer();

	UFUNCTION()
	void HandleSpawnedEnemyEndPlay(AActor* DestroyedActor, EEndPlayReason::Type EndPlayReason);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Spawner")
	TObjectPtr<USphereComponent> SpawnAreaSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Spawner")
	TObjectPtr<UBillboardComponent> EditorSprite;

	/** Weighted pool of enemy classes this spawner picks from. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Spawner")
	TArray<FSOSpawnEntry> EnemyPool;

	/** How many enemies the first wave contains. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Spawner", meta = (ClampMin = "1", UIMin = "1", UIMax = "100"))
	int32 EnemiesPerWave = 3;

	/** Additional enemies added to each subsequent wave. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Spawner", meta = (ClampMin = "0", UIMin = "0", UIMax = "20"))
	int32 EnemiesPerWaveGrowth = 1;

	/** Absolute cap so waves don't explode past what the machine can render. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Spawner", meta = (ClampMin = "1", UIMin = "1", UIMax = "500"))
	int32 MaxEnemiesPerWave = 40;

	/** Radius (cm) around the spawner in which enemies are spawned. Uses the visible sphere for editor feedback. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Spawner", meta = (ClampMin = "100.0", UIMin = "100.0", UIMax = "5000.0"))
	float SpawnRadius = 800.0f;

	/** Delay (s) between the last kill and the next wave starting. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Spawner", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "60.0"))
	float InterWaveDelay = 5.0f;

	/** Delay (s) between individual spawns inside a wave, so the pack doesn't materialize on top of each other. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Spawner", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float SpawnStagger = 0.3f;

	/** If false, waves run indefinitely. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Spawner")
	bool bLimitTotalWaves = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Spawner", meta = (ClampMin = "1", UIMin = "1", UIMax = "50", EditCondition = "bLimitTotalWaves"))
	int32 TotalWaves = 5;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Spawner")
	FSOOnWaveStarted OnWaveStarted;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Spawner")
	FSOOnWaveCleared OnWaveCleared;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Spawner")
	FSOOnAllWavesCleared OnAllWavesCleared;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Spawner")
	int32 GetCurrentWave() const { return CurrentWave; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Spawner")
	int32 GetAliveEnemyCount() const { return AliveEnemies.Num(); }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Spawner")
	int32 GetTargetEnemyCount() const { return WaveTargetCount; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Spawner")
	bool AreAllWavesCleared() const { return bAllWavesCleared; }

private:
	TSubclassOf<ASOEnemyCharacter> PickWeightedEnemy() const;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> AliveEnemies;

	int32 CurrentWave     = 0;
	int32 WaveTargetCount = 0;
	int32 SpawnedThisWave = 0;
	bool  bAllWavesCleared = false;

	FTimerHandle SpawnTimerHandle;
	FTimerHandle InterWaveTimerHandle;
};
