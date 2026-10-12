// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "World/EclipseCreationDomain.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "World/EclipseFieldAffinitySubsystem.h"

AEclipseCreationDomain::AEclipseCreationDomain()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AEclipseCreationDomain::InitDomain(EEclipseAffinity InAffinity, float InRadius, float InDuration, float InResonance)
{
	OverrideAffinity = InAffinity;
	Radius = InRadius;
	ResonanceScalar = InResonance;
	SetLifeSpan(InDuration); // Destroy replicates; EndPlay unregisters on every machine.
}

void AEclipseCreationDomain::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AEclipseCreationDomain, OverrideAffinity, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AEclipseCreationDomain, Radius, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(AEclipseCreationDomain, ResonanceScalar, COND_InitialOnly);
}

void AEclipseCreationDomain::BeginPlay()
{
	Super::BeginPlay();
	if (UEclipseFieldAffinitySubsystem* Field = GetWorld()->GetSubsystem<UEclipseFieldAffinitySubsystem>())
	{
		Field->RegisterDomain(this);
	}
}

void AEclipseCreationDomain::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UEclipseFieldAffinitySubsystem* Field = GetWorld()->GetSubsystem<UEclipseFieldAffinitySubsystem>())
	{
		Field->UnregisterDomain(this);
	}
	Super::EndPlay(Reason);
}
