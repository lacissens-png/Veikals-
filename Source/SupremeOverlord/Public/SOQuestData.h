#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SOQuestData.generated.h"

class USOItemData;

UENUM(BlueprintType)
enum class ESOQuestObjectiveType : uint8
{
	/** Kill a specific enemy class (or any enemy if TargetClass is null). */
	KillEnemies    UMETA(DisplayName = "Kill Enemies"),
	/** Collect / own a material (checked against SOInventoryComponent). */
	CollectMaterial UMETA(DisplayName = "Collect Material"),
	/** Clear N waves via SOEnemySpawner (tracked by OnWaveCleared delegate). */
	ClearWaves     UMETA(DisplayName = "Clear Waves"),
	/** Defeat a boss (any SOBossCharacter death, or specific subclass). */
	DefeatBoss     UMETA(DisplayName = "Defeat Boss"),
	/** Reach a world location within AcceptanceRadius. */
	ReachLocation  UMETA(DisplayName = "Reach Location"),
};

USTRUCT(BlueprintType)
struct FSOQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	ESOQuestObjectiveType Type = ESOQuestObjectiveType::KillEnemies;

	/** Enemy / boss class to match (null = any). KillEnemies + DefeatBoss. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest", meta = (AllowAbstract = "false"))
	TSoftClassPtr<AActor> TargetClass;

	/** Material DataAsset for CollectMaterial objectives. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TSoftObjectPtr<UPrimaryDataAsset> TargetMaterial;

	/** How many to kill / collect / waves. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest", meta = (ClampMin = "1"))
	int32 RequiredCount = 1;

	/** World-space target for ReachLocation objectives. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest", meta = (ClampMin = "50"))
	float AcceptanceRadius = 200.0f;
};

USTRUCT(BlueprintType)
struct FSOQuestReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int32 XP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int32 Gold = 0;

	/** Optional item to auto-equip on completion. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TSoftObjectPtr<USOItemData> ItemReward;
};

/**
 * DataAsset describing a single quest — objectives and rewards.
 * Create one asset per quest in the Content Browser.
 */
UCLASS(BlueprintType)
class SUPREMEOVERLORD_API USOQuestData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText QuestTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest", meta = (MultiLine = "true"))
	FText QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FSOQuestObjective> Objectives;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FSOQuestReward Reward;

	/** If set, this quest only becomes available after the listed quest is complete. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TSoftObjectPtr<USOQuestData> PrerequisiteQuest;
};
