#include "SOPickupOrb.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "SOCharacter.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"
#include "UObject/ConstructorHelpers.h"

ASOPickupOrb::ASOPickupOrb()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(45.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionSphere->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionSphere);

	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	OrbMesh->SetupAttachment(CollisionSphere);
	OrbMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OrbMesh->SetWorldScale3D(FVector(0.4f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BasicSphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (BasicSphere.Succeeded())
	{
		OrbMesh->SetStaticMesh(BasicSphere.Object);
	}
}

void ASOPickupOrb::BeginPlay()
{
	Super::BeginPlay();

	SpawnBaseLocation = GetActorLocation();

	if (CollisionSphere)
	{
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASOPickupOrb::HandleOverlap);
	}

	if (DespawnAfter > 0.0f)
	{
		SetLifeSpan(DespawnAfter);
	}
}

void ASOPickupOrb::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bClaimed)
	{
		return;
	}

	TimeAlive += DeltaSeconds;

	// Cosmetic spin + bob applied to the mesh so the collision sphere stays put.
	if (OrbMesh)
	{
		OrbMesh->AddLocalRotation(FRotator(0.0f, SpinRateDegPerSec * DeltaSeconds, 0.0f));
	}

	FVector NewLocation = GetActorLocation();

	if (BobAmplitude > 0.0f)
	{
		const float BobOffset = FMath::Sin(TimeAlive * BobRateHz * 2.0f * PI) * BobAmplitude;
		NewLocation.Z = SpawnBaseLocation.Z + BobOffset;
	}

	// Magnetize toward the player once they're inside MagnetRadius.
	if (MagnetRadius > 0.0f)
	{
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			const FVector PlayerLoc = PlayerPawn->GetActorLocation();
			const float DistSq = FVector::DistSquared(NewLocation, PlayerLoc);
			if (DistSq <= FMath::Square(MagnetRadius))
			{
				NewLocation = FMath::VInterpTo(NewLocation, PlayerLoc, DeltaSeconds, MagnetInterpSpeed);
				SpawnBaseLocation = NewLocation; // Keep bob centered on the moving base.
			}
		}
	}

	SetActorLocation(NewLocation);
}

void ASOPickupOrb::HandleOverlap(UPrimitiveComponent* /*OverlappedComp*/, AActor* OtherActor, UPrimitiveComponent* /*OtherComp*/,
                                 int32 /*OtherBodyIndex*/, bool /*bFromSweep*/, const FHitResult& /*SweepResult*/)
{
	if (bClaimed || !OtherActor)
	{
		return;
	}

	ASOCharacter* Picker = Cast<ASOCharacter>(OtherActor);
	if (!Picker || !Picker->IsAlive())
	{
		return;
	}

	if (!ApplyPayloadTo(Picker))
	{
		// Nothing applied (e.g. HP orb but player is already full). Leave the orb in the world so the player can grab it later.
		return;
	}

	bClaimed = true;
	OnPickedUp(Picker);
	Destroy();
}

bool ASOPickupOrb::ApplyPayloadTo(ASOCharacter* Picker)
{
	if (!Picker || Amount <= 0.0f)
	{
		return false;
	}

	switch (OrbType)
	{
	case ESOOrbType::Health:
		if (USOHealthComponent* HP = Picker->HealthComponent)
		{
			return HP->Heal(Amount) > 0.0f;
		}
		return false;

	case ESOOrbType::Mana:
		if (USOManaComponent* MP = Picker->ManaComponent)
		{
			return MP->Restore(Amount) > 0.0f;
		}
		return false;

	case ESOOrbType::Gold:
		Picker->AddGold(FMath::RoundToInt(Amount));
		return true;
	}

	return false;
}
