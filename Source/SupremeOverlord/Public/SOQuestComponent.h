#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOQuestData.h"
#include "SOQuestComponent.generated.h"

class ASOCharacter;

// ---------------------------------------------------------------------------
// Runtime state for one active quest
// ---------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FSOActiveQuest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TObjectPtr<USOQuestData> Data = nullptr;

	/** Progress counter per objective (index matches Data->Objectives). */
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TArray<int32> Progress;

	bool IsObjectiveComplete(int32 Idx) const;
	bool IsAllComplete() const;
};

// ---------------------------------------------------------------------------
// Delegates
// ---------------------------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnQuestAccepted,    USOQuestData*, Quest, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnObjectiveUpdated, USOQuestData*, Quest, int32, ObjectiveIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FSOOnQuestCompleted,   USOQuestData*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FSOOnQuestFailed,      USOQuestData*, Quest);

/**
 * Tracks active and completed quests on the player character.
 * Enemy kills are reported from ASOEnemyCharacter::HandleDeath.
 * Wave clears are reported via a delegate bound to ASOEnemySpawner at runtime.
 * CollectMaterial objectives are seeded from inventory on accept and on every
 * OnMaterialCountChanged event.
 * ReachLocation objectives are polled on Tick (4 Hz).
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOQuestComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// ---------- API ----------

	/** Returns true if the quest was added (false if already active / completed / prereq unmet). */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool AcceptQuest(USOQuestData* Quest);

	/** Called externally when a quest should auto-fail. */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void FailQuest(USOQuestData* Quest);

	UFUNCTION(BlueprintPure, Category = "Quest")
	bool IsQuestActive(USOQuestData* Quest) const;

	UFUNCTION(BlueprintPure, Category = "Quest")
	bool IsQuestComplete(USOQuestData* Quest) const;

	UFUNCTION(BlueprintPure, Category = "Quest")
	int32 GetObjectiveProgress(USOQuestData* Quest, int32 ObjectiveIndex) const;

	// ---------- Event entry points (called from other systems) ----------

	/** Called from ASOEnemyCharacter::HandleDeath for every enemy death. */
	void NotifyEnemyKilled(AActor* EnemyActor);

	/** Called from SOInventoryComponent::OnMaterialCountChanged. */
	UFUNCTION()
	void HandleMaterialCountChanged(USOMaterialData* Material, int32 NewCount, int32 Delta);

	// ---------- Delegates ----------
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FSOOnQuestAccepted OnQuestAccepted;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FSOOnObjectiveUpdated OnObjectiveUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FSOOnQuestCompleted OnQuestCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FSOOnQuestFailed OnQuestFailed;

	// ---------- State (read-only in BP) ----------
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TArray<FSOActiveQuest> ActiveQuests;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TArray<TObjectPtr<USOQuestData>> CompletedQuests;

private:
	UFUNCTION()
	void HandleWaveCleared(int32 WaveIndex);

	void AdvanceObjective(FSOActiveQuest& AQ, int32 ObjIdx, int32 Delta = 1);
	void TryCompleteQuest(FSOActiveQuest& AQ);
	void GrantReward(const FSOQuestReward& Reward);
	void SeedMaterialObjectives();

	FSOActiveQuest* FindActiveQuest(USOQuestData* Data);
};
