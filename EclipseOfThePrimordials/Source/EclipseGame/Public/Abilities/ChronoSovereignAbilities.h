// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// CHRONO-SOVEREIGN  -  Resource: Temporal Strain (Strain model: casting ADDS strain)
//   Time Stop                         AoE Freeze for 3-5 s (bosses: fills Stagger instead).
//   Reality Slash                     line AoE ignoring 50% armor.
//   The Goal of All Life is Death     SUPER-TIER ULTIMATE: marks the area, then after
//                                     12 s a fatal death wave erupts from the caster.

#pragma once

#include "Abilities/EclipseGameplayAbility.h"
#include "CoreMinimal.h"

#include "ChronoSovereignAbilities.generated.h"

UCLASS()
class ECLIPSEGAME_API UGA_TimeStop : public UEclipseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_TimeStop();

protected:
	virtual void OnAbilityCommitted() override;

	UPROPERTY(EditDefaultsOnly, Category = "Time Stop") float Radius = 1200.f;
	/** Clamped to [MinDuration, MaxDuration]; runewords pass their own duration as the event param. */
	UPROPERTY(EditDefaultsOnly, Category = "Time Stop") float Duration = 4.f;
	UPROPERTY(EditDefaultsOnly, Category = "Time Stop") float MinDuration = 3.f;
	UPROPERTY(EditDefaultsOnly, Category = "Time Stop") float MaxDuration = 5.f;
};

UCLASS()
class ECLIPSEGAME_API UGA_RealitySlash : public UEclipseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_RealitySlash();

protected:
	virtual void OnAbilityCommitted() override;

	UPROPERTY(EditDefaultsOnly, Category = "Reality Slash") float Length = 1800.f;
	UPROPERTY(EditDefaultsOnly, Category = "Reality Slash") float HalfWidth = 120.f;
	UPROPERTY(EditDefaultsOnly, Category = "Reality Slash") float Damage = 650.f;
	/** "Ignores 50% armor". */
	UPROPERTY(EditDefaultsOnly, Category = "Reality Slash", meta = (ClampMin = "0", ClampMax = "1")) float ArmorPenetration = 0.5f;
};

UCLASS()
class ECLIPSEGAME_API UGA_GoalOfAllLifeIsDeath : public UEclipseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_GoalOfAllLifeIsDeath();

protected:
	virtual void OnAbilityCommitted() override;

	UFUNCTION()
	void Detonate();

	UPROPERTY(EditDefaultsOnly, Category = "Ultimate") float Delay = 12.f;
	UPROPERTY(EditDefaultsOnly, Category = "Ultimate") float Radius = 2200.f;
	/** Marked enemies are Vulnerable while the clock runs down. */
	UPROPERTY(EditDefaultsOnly, Category = "Ultimate") float MarkVulnerableBonus = 0.25f;
	/** Non-boss mobs: fatal. Bosses/actors: this fraction of the boss's MAX health as damage. */
	UPROPERTY(EditDefaultsOnly, Category = "Ultimate") float BossMaxHealthFraction = 0.2f;
};
