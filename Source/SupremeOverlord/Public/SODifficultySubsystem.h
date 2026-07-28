#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SODifficultySubsystem.generated.h"

/** New-Game-Plus-style global difficulty tier. */
UENUM(BlueprintType)
enum class ESODifficultyTier : uint8
{
	Normal    UMETA(DisplayName = "Normal"),
	Hard      UMETA(DisplayName = "Hard"),
	Nightmare UMETA(DisplayName = "Nightmare"),
	Torment   UMETA(DisplayName = "Torment"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnDifficultyChanged, ESODifficultyTier, NewTier);

/**
 * Global difficulty scalar, one per game instance. ASOEnemyCharacter reads
 * the active tier's multipliers in BeginPlay (before Super::BeginPlay, so the
 * scaled MaxHealth is what USOHealthComponent snaps CurrentHealth to) — so
 * raising the tier only affects enemies spawned afterward, matching how
 * "start a harder run" difficulty selection works in Diablo-likes.
 */
UCLASS()
class SUPREMEOVERLORD_API USODifficultySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Convenience accessor. Returns null if World or its GameInstance is unavailable. */
	static USODifficultySubsystem* Get(const UWorld* World);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Difficulty")
	ESODifficultyTier GetDifficultyTier() const { return CurrentTier; }

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Difficulty")
	void SetDifficultyTier(ESODifficultyTier NewTier);

	/** Advances to the next tier, wrapping from Torment back to Normal. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Difficulty")
	void CycleDifficulty();

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Difficulty")
	FText GetDifficultyDisplayName() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Difficulty")
	float GetEnemyHealthMultiplier() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Difficulty")
	float GetEnemyDamageMultiplier() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Difficulty")
	float GetXPMultiplier() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Difficulty")
	float GetGoldMultiplier() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Difficulty")
	float GetItemDropChanceMultiplier() const;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Difficulty")
	FSOOnDifficultyChanged OnDifficultyChanged;

private:
	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Difficulty", Transient)
	ESODifficultyTier CurrentTier = ESODifficultyTier::Normal;
};
