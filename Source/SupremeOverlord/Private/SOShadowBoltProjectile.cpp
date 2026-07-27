#include "SOShadowBoltProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "SODamageType.h"
#include "UObject/ConstructorHelpers.h"

ASOShadowBoltProjectile::ASOShadowBoltProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(18.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionSphere->SetNotifyRigidBodyCollision(true);
	CollisionSphere->CanCharacterStepUpOn = ECB_No;
	SetRootComponent(CollisionSphere);

	BoltMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoltMesh"));
	BoltMesh->SetupAttachment(CollisionSphere);
	BoltMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoltMesh->SetWorldScale3D(FVector(0.35f));

	// Try to use the engine's basic sphere so the bolt is visible without any user setup.
	// Swappable per-instance in a BP subclass for something spookier.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BasicSphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (BasicSphere.Succeeded())
	{
		BoltMesh->SetStaticMesh(BasicSphere.Object);
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent    = CollisionSphere;
	ProjectileMovement->InitialSpeed        = Speed;
	ProjectileMovement->MaxSpeed            = Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce       = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	InitialLifeSpan = MaxLifetime;
}

void ASOShadowBoltProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Sync runtime speed with whatever the designer set (both properties are exposed).
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = Speed;
		ProjectileMovement->MaxSpeed     = Speed;
		ProjectileMovement->Velocity     = ProjectileMovement->Velocity.GetSafeNormal() * Speed;
	}

	if (MaxLifetime > 0.0f)
	{
		SetLifeSpan(MaxLifetime);
	}

	if (CollisionSphere)
	{
		CollisionSphere->OnComponentHit.AddDynamic(this, &ASOShadowBoltProjectile::HandleHit);

		// Never collide with the actor that fired us, otherwise the bolt dies on the caster's capsule.
		if (AActor* MyInstigator = GetInstigator())
		{
			CollisionSphere->IgnoreActorWhenMoving(MyInstigator, true);
		}
	}
}

void ASOShadowBoltProjectile::HandleHit(UPrimitiveComponent* /*HitComp*/, AActor* OtherActor, UPrimitiveComponent* /*OtherComp*/,
                                        FVector /*NormalImpulse*/, const FHitResult& Hit)
{
	if (OtherActor == this || OtherActor == GetInstigator())
	{
		return;
	}

	if (OtherActor)
	{
		TSubclassOf<UDamageType> DTClass = DamageType
			? DamageType
			: TSubclassOf<UDamageType>(USODamageType::StaticClass());

		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, DTClass);
	}

	if (ImpactSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX, Hit.ImpactPoint);
	}

	OnBoltImpact(Hit, OtherActor);
	Destroy();
}
