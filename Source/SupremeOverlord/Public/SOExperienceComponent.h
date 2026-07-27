#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOExperienceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSOOnExperienceChanged,
	USOExperienceComponent*, OwningComponent,
	int32,                   NewXP,
	int32,                   XPForNextLevel,
	int32,                   CurrentLevel);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSOOnLevelUp,
	USOExperienceComponent*, OwningComponent,
	int32,                   NewLevel,
	int32,                   PreviousLevel);

/**
 * Simple XP/level pool. Curve:
 *   XPForNextLevel(L) = FMath::RoundToInt(BaseXPForLevel2 * L^Curve)
 * With defaults (100, 1.5) it takes ~100 to hit L2, ~283 to hit L3, ~520 to hit L4,
 * feeling snappy at low levels while ramping steadily.
 */
UCLASS(ClassGroup = (SupremeOverlord), meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOExperienceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOExperienceComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|XP", meta = (ClampMin = "1", UIMin = "1"))
	int32 StartingLevel = 1;

	/** XP required to hit level 2 (baseline for the curve). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|XP", meta = (ClampMin = "1", UIMin = "10", UIMax = "10000"))
	int32 BaseXPForLevel2 = 100;

	/** Exponent applied to level in the XP requirement formula. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|XP", meta = (ClampMin = "1.0", UIMin = "1.0", UIMax = "3.0"))
	float LevelCurveExponent = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|XP", meta = (ClampMin = "1", UIMin = "1", UIMax = "100"))
	int32 MaxLevel = 60;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|XP")
	FSOOnExperienceChanged OnExperienceChanged;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|XP")
	FSOOnLevelUp OnLevelUp;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|XP")
	int32 GetCurrentLevel() const { return CurrentLevel; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|XP")
	int32 GetCurrentXPInLevel() const { return XPInCurrentLevel; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|XP")
	int32 GetXPToNextLevel() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|XP")
	float GetLevelProgressPercent() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|XP")
	bool IsMaxLevel() const { return CurrentLevel >= MaxLevel; }

	/** Adds Amount XP and rolls level-ups until the pool is under the next requirement. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|XP")
	void GainXP(int32 Amount);

private:
	int32 ComputeXPForLevel(int32 Level) const;

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|XP", Transient)
	int32 CurrentLevel = 1;

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|XP", Transient)
	int32 XPInCurrentLevel = 0;
};
