// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// ARCHITECT OF FLESH  -  Resource: Biomass / Blood Energy (Builder)
//   Grasping Heart       single-target execute or stun; kills generate Biomass.
//   Ia Mshub-Niggurath   SUPER-TIER ULTIMATE: annihilates trash in a wide radius,
//                        heavily damages everything else, spawns 3 Dark Young.
//   Flesh Colossus       rune-effect ability (SpawnFleshColossus).

#pragma once

#include "Abilities/EclipseGameplayAbility.h"
#include "CoreMinimal.h"

#include "ArchitectOfFleshAbilities.generated.h"

UCLASS()
class ECLIPSEGAME_API UGA_GraspingHeart : public UEclipseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_GraspingHeart();

protected:
	virtual void OnAbilityCommitted() override;

	UPROPERTY(EditDefaultsOnly, Category = "Grasping Heart") float Range = 900.f;
	UPROPERTY(EditDefaultsOnly, Category = "Grasping Heart") float TargetConeHalfAngle = 30.f;
	UPROPERTY(EditDefaultsOnly, Category = "Grasping Heart") float Damage = 420.f;
	/** Captains below this health fraction (Brutes below half of it) are executed outright. */
	UPROPERTY(EditDefaultsOnly, Category = "Grasping Heart") float ExecuteThreshold = 0.25f;
	UPROPERTY(EditDefaultsOnly, Category = "Grasping Heart") float StunDuration = 1.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Grasping Heart") float BiomassOnKill = 25.f;
};

UCLASS()
class ECLIPSEGAME_API UGA_IaMshubNiggurath : public UEclipseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_IaMshubNiggurath();

protected:
	virtual void OnAbilityCommitted() override;

	UPROPERTY(EditDefaultsOnly, Category = "Ultimate") float Radius = 1600.f;
	/** Damage to anything that survives the trash cull (captains, brutes, bosses). */
	UPROPERTY(EditDefaultsOnly, Category = "Ultimate") float Damage = 2500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Ultimate") TSubclassOf<AActor> DarkYoungClass;
	UPROPERTY(EditDefaultsOnly, Category = "Ultimate") int32 DarkYoungCount = 3;
	UPROPERTY(EditDefaultsOnly, Category = "Ultimate") float DarkYoungLifetime = 20.f;
	UPROPERTY(EditDefaultsOnly, Category = "Ultimate") float BiomassPerKill = 2.f;
};

/** Rune effect "SpawnFleshColossus(Lifetime)". */
UCLASS()
class ECLIPSEGAME_API UGA_SpawnFleshColossus : public UEclipseGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void OnAbilityCommitted() override;

	UPROPERTY(EditDefaultsOnly, Category = "Colossus") TSubclassOf<AActor> ColossusClass;
	UPROPERTY(EditDefaultsOnly, Category = "Colossus") float DefaultLifetime = 12.f;
};
