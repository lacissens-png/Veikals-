#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOAchievementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnAchievementUnlocked, FName, AchievementID, FText, DisplayName);

/**
 * Generic unlock-by-ID milestone tracker. Doesn't own any milestone
 * definitions itself — callers (ASOCharacter's delegate handlers, quest
 * hooks, anything) call UnlockAchievement(ID, DisplayName) when a condition
 * is met; this component just dedupes, remembers, and surfaces a brief HUD
 * toast (see SOHUD.cpp) for whatever unlocked.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOAchievementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOAchievementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/** Unlocks ID if not already unlocked, showing DisplayName in the toast. Returns false if already unlocked. */
	UFUNCTION(BlueprintCallable, Category = "Achievements")
	bool UnlockAchievement(FName ID, const FText& DisplayName);

	UFUNCTION(BlueprintPure, Category = "Achievements")
	bool IsUnlocked(FName ID) const { return UnlockedAchievements.Contains(ID); }

	UFUNCTION(BlueprintPure, Category = "Achievements")
	int32 GetUnlockedCount() const { return UnlockedAchievements.Num(); }

	UFUNCTION(BlueprintPure, Category = "Achievements")
	TArray<FName> GetUnlockedAchievements() const { return UnlockedAchievements.Array(); }

	/** Load support: marks IDs unlocked directly, with no toast/SFX/delegate for any of them. */
	UFUNCTION(BlueprintCallable, Category = "Achievements")
	void RestoreUnlockedAchievements(const TArray<FName>& IDs);

	// ---------- Toast (read by SOHUD each frame) ----------

	UFUNCTION(BlueprintPure, Category = "Achievements")
	bool IsToastActive() const { return ToastTimeRemaining > 0.0f; }

	UFUNCTION(BlueprintPure, Category = "Achievements")
	FText GetToastText() const { return ToastText; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievements", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "10.0"))
	float ToastDuration = 3.5f;

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "Achievements")
	FSOOnAchievementUnlocked OnAchievementUnlocked;

	UFUNCTION(BlueprintImplementableEvent, Category = "Achievements")
	void OnAchievementUnlockedBP(FName ID, const FText& DisplayName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievements|Audio")
	TObjectPtr<class USoundBase> UnlockSFX;

private:
	UPROPERTY(Transient)
	TSet<FName> UnlockedAchievements;

	FText ToastText;
	float ToastTimeRemaining = 0.0f;
};
