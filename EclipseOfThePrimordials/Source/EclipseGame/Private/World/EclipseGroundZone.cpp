// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "World/EclipseGroundZone.h"

#include "AbilitySystemComponent.h"
#include "Combat/EclipseCombatStatics.h"
#include "TimerManager.h"

AEclipseGroundZone::AEclipseGroundZone()
{
	bReplicates = true;
	SetReplicatingMovement(false); // Zones never move.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AEclipseGroundZone::InitZone(UAbilitySystemComponent* InSourceASC, TSubclassOf<UGameplayEffect> InDamageEffect,
	float InDamagePerPulse, EEclipseAffinity InAffinity, const FEclipseStatusSpec& InStatus, float InLifetime)
{
	SourceASC = InSourceASC;
	SourceActor = InSourceASC ? InSourceASC->GetAvatarActor() : nullptr;
	DamageEffect = InDamageEffect;
	DamagePerPulse = InDamagePerPulse;
	Affinity = InAffinity;
	Status = InStatus;
	SetLifeSpan(InLifetime);
}

void AEclipseGroundZone::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(PulseTimer, this, &AEclipseGroundZone::Pulse, PulseInterval, true, 0.f);
	}
}

void AEclipseGroundZone::Pulse()
{
	FEclipseAreaHitParams Params;
	Params.SourceASC = SourceASC.Get();
	Params.DamageEffect = DamageEffect;
	Params.Damage.Amount = DamagePerPulse;
	Params.Damage.Affinity = Affinity;
	Params.Damage.Instigator = SourceActor; // Kill credit + ritual events go to the caster.
	Params.Status = Status.IsValid() ? &Status : nullptr;
	Params.bIncludeActors = bHitBossesAndActors && Params.SourceASC != nullptr;

	EclipseCombat::HitSphere(this, GetActorLocation(), Radius, Params);
}
