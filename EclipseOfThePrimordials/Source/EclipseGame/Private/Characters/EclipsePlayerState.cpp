// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Characters/EclipsePlayerState.h"

#include "Classes/EclipseClassDefinition.h"
#include "EclipseCore/Aspects.h"
#include "Items/EclipseForgeComponent.h"
#include "Items/EclipseInventoryComponent.h"
#include "Net/UnrealNetwork.h"

AEclipsePlayerState::AEclipsePlayerState()
{
	Inventory = CreateDefaultSubobject<UEclipseInventoryComponent>(TEXT("Inventory"));
	Forge = CreateDefaultSubobject<UEclipseForgeComponent>(TEXT("Forge"));

	// PlayerStates default to a low rate; inventory changes are bursty and owner-only.
	NetUpdateFrequency = 10.f;
}

void AEclipsePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEclipsePlayerState, ClassDefinition);
}

uint8 AEclipsePlayerState::GetClassId() const
{
	return ClassDefinition ? static_cast<uint8>(ClassDefinition->ClassId) : Eclipse::AnyClass;
}
