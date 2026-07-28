#include "SOWaypoint.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "SOCharacter.h"
#include "SOWaypointComponent.h"
#include "UObject/ConstructorHelpers.h"

ASOWaypoint::ASOWaypoint()
{
	PrimaryActorTick.bCanEverTick = false;

	DiscoverySphere = CreateDefaultSubobject<USphereComponent>(TEXT("DiscoverySphere"));
	DiscoverySphere->SetSphereRadius(DiscoveryRadius);
	DiscoverySphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DiscoverySphere->SetCollisionObjectType(ECC_WorldDynamic);
	DiscoverySphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DiscoverySphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetRootComponent(DiscoverySphere);

	WaypointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaypointMesh"));
	WaypointMesh->SetupAttachment(DiscoverySphere);
	WaypointMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WaypointMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 2.5f));

	// Tall pillar reads as a landmark from a distance until proper art is authored.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BasicCylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (BasicCylinder.Succeeded())
	{
		WaypointMesh->SetStaticMesh(BasicCylinder.Object);
	}

	DiscoverySphere->OnComponentBeginOverlap.AddDynamic(this, &ASOWaypoint::OnSphereBeginOverlap);
}

void ASOWaypoint::BeginPlay()
{
	Super::BeginPlay();

	DiscoverySphere->SetSphereRadius(DiscoveryRadius);

	if (bStartDiscovered)
	{
		bDiscovered = true;
	}
}

void ASOWaypoint::OnSphereBeginOverlap(UPrimitiveComponent* /*OverlappedComponent*/,
                                        AActor* OtherActor,
                                        UPrimitiveComponent* /*OtherComp*/,
                                        int32 /*OtherBodyIndex*/,
                                        bool /*bFromSweep*/,
                                        const FHitResult& /*SweepResult*/)
{
	ASOCharacter* Character = Cast<ASOCharacter>(OtherActor);
	if (!Character || !Character->WaypointComponent)
	{
		return;
	}

	const bool bFirstDiscovery = !bDiscovered;
	if (!Character->WaypointComponent->DiscoverWaypoint(this))
	{
		return;
	}

	bDiscovered = true;

	if (bFirstDiscovery)
	{
		if (DiscoverSFX)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DiscoverSFX, GetActorLocation());
		}
		OnDiscovered(Character);
	}
}
