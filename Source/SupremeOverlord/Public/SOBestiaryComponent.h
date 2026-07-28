#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOBestiaryComponent.generated.h"

class ASOEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnBestiaryEntryUpdated,
	TSubclassOf<ASOEnemyCharacter>, EnemyClass,
	int32,                         NewCount);

/**
 * Tracks kills per enemy *class* (not per-instance) for a simple meta-progression
 * codex. ASOEnemyCharacter::HandleDeath calls RecordKill on the killer's
 * component alongside the existing XP/quest/corruption grants.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOBestiaryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOBestiaryComponent();

	/** Increments the kill count for EnemyClass and broadcasts the update. */
	UFUNCTION(BlueprintCallable, Category = "Bestiary")
	void RecordKill(TSubclassOf<ASOEnemyCharacter> EnemyClass);

	UFUNCTION(BlueprintPure, Category = "Bestiary")
	int32 GetKillCount(TSubclassOf<ASOEnemyCharacter> EnemyClass) const;

	UFUNCTION(BlueprintPure, Category = "Bestiary")
	int32 GetTotalKills() const;

	/** How many distinct enemy species have at least one recorded kill. */
	UFUNCTION(BlueprintPure, Category = "Bestiary")
	int32 GetDiscoveredSpeciesCount() const { return KillCounts.Num(); }

	/** Every recorded species sorted by kill count, descending. */
	UFUNCTION(BlueprintPure, Category = "Bestiary")
	TArray<FString> GetEntryDescriptions() const;

	UFUNCTION(BlueprintPure, Category = "Bestiary")
	bool IsCodexOpen() const { return bCodexOpen; }

	UFUNCTION(BlueprintCallable, Category = "Bestiary")
	void ToggleCodex();

	UPROPERTY(BlueprintAssignable, Category = "Bestiary")
	FSOOnBestiaryEntryUpdated OnBestiaryEntryUpdated;

private:
	UPROPERTY(Transient)
	TMap<TSubclassOf<ASOEnemyCharacter>, int32> KillCounts;

	bool bCodexOpen = false;
};
