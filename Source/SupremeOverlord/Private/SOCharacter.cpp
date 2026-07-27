#include "SOCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SODamageType.h"
#include "SOHealthComponent.h"
#include "SOShadowBoltProjectile.h"
#include "TimerManager.h"

ASOCharacter::ASOCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength         = CameraDistance;
	CameraBoom->bDoCollisionTest        = false;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch           = false;
	CameraBoom->bInheritYaw             = false;
	CameraBoom->bInheritRoll            = false;
	CameraBoom->SetRelativeRotation(FRotator(-CameraPitch, CameraYaw, 0.0f));

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<USOHealthComponent>(TEXT("HealthComponent"));

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate              = FRotator(0.0f, RotationRateYaw, 0.0f);
		Movement->MaxWalkSpeed              = MovementSpeed;
		Movement->bConstrainToPlane         = true;
		Movement->bSnapToPlaneAtStart       = true;
	}
}

void ASOCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ApplyCameraSettings();
	ApplyMovementSettings();
}

void ASOCharacter::BeginPlay()
{
	Super::BeginPlay();
	ApplyCameraSettings();
	ApplyMovementSettings();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ASOCharacter::HandleDeath);
	}
}

void ASOCharacter::HandleDeath(USOHealthComponent* /*OwningComponent*/, AController* InstigatedBy, AActor* DamageCauser)
{
	OnCharacterDied(InstigatedBy, DamageCauser);
}

bool ASOCharacter::IsAlive() const
{
	return HealthComponent ? HealthComponent->IsAlive() : true;
}

void ASOCharacter::OnCharacterDied(AController* /*InstigatedBy*/, AActor* /*DamageCauser*/)
{
	// Kill in-progress navigation and freeze the movement component.
	if (AController* MyController = GetController())
	{
		MyController->StopMovement();
	}

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->DisableMovement();
		Movement->StopMovementImmediately();
	}

	// Let corpses be walked over; keeps AI pathing clean once enemies exist.
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// The controller listens to this via the Pawn->PlayerController path in Blueprint if desired.
	// Actual respawn / game-over UI is intentionally left to a later system.
}

void ASOCharacter::ApplyCameraSettings()
{
	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = CameraDistance;
		CameraBoom->SetRelativeRotation(FRotator(-CameraPitch, CameraYaw, 0.0f));
	}
}

void ASOCharacter::ApplyMovementSettings()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = MovementSpeed;
		Movement->RotationRate = FRotator(0.0f, RotationRateYaw, 0.0f);
	}
}

bool ASOCharacter::CanPrimaryAttack() const
{
	return IsAlive() && !bPrimaryAttackOnCooldown;
}

void ASOCharacter::PerformPrimaryAttack(FVector TargetLocation)
{
	if (!CanPrimaryAttack())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Face the target on the ground plane so the swing lines up with the click.
	const FVector MyLocation = GetActorLocation();
	FVector ToTarget         = (TargetLocation - MyLocation);
	ToTarget.Z               = 0.0f;
	if (bFaceAttackDirection && !ToTarget.IsNearlyZero())
	{
		const FRotator DesiredRot = ToTarget.GetSafeNormal().Rotation();
		SetActorRotation(FRotator(0.0f, DesiredRot.Yaw, 0.0f));
	}

	// Center the hit sphere in front of the character regardless of how far away the click was.
	const FVector Forward      = GetActorForwardVector();
	const FVector AttackCenter = MyLocation + Forward * PrimaryAttackRange;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(SOPrimaryAttack), /*bTraceComplex=*/ false, /*Ignore=*/ this);

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByChannel(
		Overlaps,
		AttackCenter,
		FQuat::Identity,
		PrimaryAttackChannel,
		FCollisionShape::MakeSphere(PrimaryAttackRadius),
		Params);

	TArray<AActor*> UniqueHitActors;
	UniqueHitActors.Reserve(Overlaps.Num());

	TSubclassOf<UDamageType> DTClass = PrimaryAttackDamageType
		? PrimaryAttackDamageType
		: TSubclassOf<UDamageType>(USODamageType::StaticClass());

	AController* InstigatorController = GetController();

	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* HitActor = Result.GetActor();
		if (!HitActor || HitActor == this)
		{
			continue;
		}

		// Only damage actors carrying a live health component - keeps the hit list clean
		// (props, decor, and dead characters are skipped automatically).
		USOHealthComponent* HitHealth = HitActor->FindComponentByClass<USOHealthComponent>();
		if (!HitHealth || HitHealth->IsDead())
		{
			continue;
		}

		if (UniqueHitActors.Contains(HitActor))
		{
			continue;
		}
		UniqueHitActors.Add(HitActor);

		UGameplayStatics::ApplyDamage(HitActor, PrimaryAttackDamage, InstigatorController, this, DTClass);
	}

	OnPrimaryAttackPerformed(AttackCenter, UniqueHitActors);

	if (bDrawPrimaryAttackDebug)
	{
		DrawDebugSphere(World, AttackCenter, PrimaryAttackRadius, 20, FColor::Green, false, 0.6f, 0, 2.0f);
		DrawDebugLine  (World, MyLocation, AttackCenter, FColor::Green, false, 0.6f, 0, 2.0f);
	}

	bPrimaryAttackOnCooldown = true;
	World->GetTimerManager().SetTimer(
		PrimaryAttackCooldownHandle,
		FTimerDelegate::CreateLambda([this]() { bPrimaryAttackOnCooldown = false; }),
		PrimaryAttackCooldown,
		false);
}

bool ASOCharacter::CanCastShadowBolt() const
{
	return IsAlive() && !bShadowBoltOnCooldown && ShadowBoltClass != nullptr;
}

void ASOCharacter::CastShadowBolt(FVector TargetLocation)
{
	if (!CanCastShadowBolt())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector MyLocation = GetActorLocation();
	FVector AimDelta         = (TargetLocation - MyLocation);
	AimDelta.Z               = 0.0f;
	FVector AimDirection     = AimDelta.GetSafeNormal();
	if (AimDirection.IsNearlyZero())
	{
		AimDirection = GetActorForwardVector();
	}

	if (bFaceCastDirection)
	{
		SetActorRotation(FRotator(0.0f, AimDirection.Rotation().Yaw, 0.0f));
	}

	const FVector Muzzle = MyLocation + AimDirection * ShadowBoltMuzzleForward + FVector(0.0f, 0.0f, ShadowBoltMuzzleHeight);
	const FRotator SpawnRot = AimDirection.Rotation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Params.Owner      = this;
	Params.Instigator = this;

	ASOShadowBoltProjectile* Bolt = World->SpawnActor<ASOShadowBoltProjectile>(ShadowBoltClass, Muzzle, SpawnRot, Params);
	if (!Bolt)
	{
		return;
	}

	OnShadowBoltCast(Muzzle, AimDirection, Bolt);

	bShadowBoltOnCooldown = true;
	World->GetTimerManager().SetTimer(
		ShadowBoltCooldownHandle,
		FTimerDelegate::CreateLambda([this]() { bShadowBoltOnCooldown = false; }),
		ShadowBoltCooldown,
		false);
}
