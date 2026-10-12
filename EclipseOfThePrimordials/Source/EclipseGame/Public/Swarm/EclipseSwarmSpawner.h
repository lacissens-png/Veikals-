// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EclipseSwarmSpawner.generated.h"

class UEclipseMobArchetype;

USTRUCT(BlueprintType)
struct FEclipseSpawnWaveEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TObjectPtr<UEclipseMobArchetype> Archetype;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "1"))
	int32 Count = 20;
};

/**
 * Level-placed encounter seed. Server-only: places packs into the swarm
 * subsystem (Trash + a Captain to lead them + optional Brutes). Has no
 * runtime cost after spawning; the actor itself is not replicated.
 */
UCLASS()
class ECLIPSEGAME_API AEclipseSwarmSpawner : public AActor
{
	GENERATED_BODY()

public:
	AEclipseSwarmSpawner();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Swarm")
	void SpawnWave();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<FEclipseSpawnWaveEntry> Wave;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "0"))
	float Radius = 1200.f;

	/** 0 = spawn once on BeginPlay. */
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (ClampMin = "0"))
	float RepeatInterval = 0.f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	bool bSpawnOnBeginPlay = true;

private:
	FTimerHandle RepeatTimer;
};
