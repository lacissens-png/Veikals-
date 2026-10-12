// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// SPIRE WARDEN  -  Resource: Seismic Pressure (Builder)
//   Grand Catastrophe   ground slam: damage + knockback, spawns magmatic obelisks
//                       (AEclipseGroundZone subclasses that pulse Burn).
//   Fallen Down         SUPER-TIER ULTIMATE: telegraphed orbital light beam that
//                       leaves a scorch zone.

#pragma once

#include "Abilities/EclipseGameplayAbility.h"
#include "CoreMinimal.h"

#include "SpireWardenAbilities.generated.h"

class AEclipseGroundZone;

UCLASS()
class ECLIPSEGAME_API UGA_GrandCatastrophe : public UEclipseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_GrandCatastrophe();

protected:
	virtual void OnAbilityCommitted() override;

	UPROPERTY(EditDefaultsOnly, Category = "Grand Catastrophe") float Radius = 700.f;
	UPROPERTY(EditDefaultsOnly, Category = "Grand Catastrophe") float Damage = 380.f;
	/** Knockback impulse (cm/s). Brutes are immune; bosses take flat stagger instead. */
	UPROPERTY(EditDefaultsOnly, Category = "Grand Catastrophe") float KnockbackImpulse = 1400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Obelisks") TSubclassOf<AEclipseGroundZone> ObeliskClass;
	UPROPERTY(EditDefaultsOnly, Category = "Obelisks") int32 ObeliskCount = 4;
	UPROPERTY(EditDefaultsOnly, Category = "Obelisks") float ObeliskRingRadius = 450.f;
	UPROPERTY(EditDefaultsOnly, Category = "Obelisks") float ObeliskLifetime = 8.f;
	UPROPERTY(EditDefaultsOnly, Category = "Obelisks") float ObeliskPulseDamage = 60.f;
	UPROPERTY(EditDefaultsOnly, Category = "Obelisks") FEclipseStatusSpec ObeliskBurn;
};

UCLASS()
class ECLIPSEGAME_API UGA_FallenDown : public UEclipseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_FallenDown();

protected:
	virtual void OnAbilityCommitted() override;

	UFUNCTION()
	void StrikeBeam();

	UPROPERTY(EditDefaultsOnly, Category = "Fallen Down") float TargetDistance = 700.f;
	UPROPERTY(EditDefaultsOnly, Category = "Fallen Down") float TelegraphDelay = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Fallen Down") float Radius = 650.f;
	UPROPERTY(EditDefaultsOnly, Category = "Fallen Down") float Damage = 4000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Fallen Down") FEclipseStatusSpec BeamBurn;

	UPROPERTY(EditDefaultsOnly, Category = "Scorch Zone") TSubclassOf<AEclipseGroundZone> ScorchZoneClass;
	UPROPERTY(EditDefaultsOnly, Category = "Scorch Zone") float ScorchLifetime = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = "Scorch Zone") float ScorchPulseDamage = 120.f;

private:
	/** Locked at cast time so the beam lands where it was telegraphed. */
	FVector BeamLocation = FVector::ZeroVector;
};
