// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Combat/EclipseCombatTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/SubclassOf.h"

#include "EclipseGroundZone.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

/**
 * Generic pulsing hazard: Spire Warden's magmatic obelisks, Fallen Down's scorch
 * zone, etc. Replicated only for visuals (the BP subclass owns the Niagara /
 * mesh); pulses execute exclusively on the server through EclipseCombat.
 */
UCLASS()
class ECLIPSEGAME_API AEclipseGroundZone : public AActor
{
	GENERATED_BODY()

public:
	AEclipseGroundZone();

	/** Server: configure between SpawnActorDeferred and FinishSpawning. */
	void InitZone(UAbilitySystemComponent* InSourceASC, TSubclassOf<UGameplayEffect> InDamageEffect,
		float InDamagePerPulse, EEclipseAffinity InAffinity, const FEclipseStatusSpec& InStatus, float InLifetime);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void Pulse();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zone", meta = (ClampMin = "10"))
	float Radius = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zone", meta = (ClampMin = "0.1"))
	float PulseInterval = 0.5f;

	/** Obelisks block movement; scorch zones don't. Set on the BP's collision. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zone")
	bool bHitBossesAndActors = true;

private:
	TWeakObjectPtr<UAbilitySystemComponent> SourceASC;
	TWeakObjectPtr<AActor> SourceActor;

	UPROPERTY(Transient)
	TSubclassOf<UGameplayEffect> DamageEffect;

	float DamagePerPulse = 0.f;
	EEclipseAffinity Affinity = EEclipseAffinity::Neutral;
	FEclipseStatusSpec Status;
	FTimerHandle PulseTimer;
};
