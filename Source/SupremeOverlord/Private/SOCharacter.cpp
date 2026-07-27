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
#include "SOExperienceComponent.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"
#include "SOShadowBoltProjectile.h"
#include "SOWeaponData.h"
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

	HealthComponent     = CreateDefaultSubobject<USOHealthComponent>(TEXT("HealthComponent"));
	ManaComponent       = CreateDefaultSubobject<USOManaComponent>(TEXT("ManaComponent"));
	ExperienceComponent = CreateDefaultSubobject<USOExperienceComponent>(TEXT("ExperienceComponent"));

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

	if (ExperienceComponent)
	{
		ExperienceComponent->OnLevelUp.AddDynamic(this, &ASOCharacter::HandleLevelUp);
	}

	Gold = FMath::Max(0, StartingGold);
	if (Gold != 0)
	{
		OnGoldChanged.Broadcast(0, Gold, Gold);
	}

	if (StartingWeapon)
	{
		EquipWeapon(StartingWeapon);
	}
}

void ASOCharacter::EquipWeapon(USOWeaponData* NewWeapon)
{
	if (EquippedWeapon == NewWeapon)
	{
		return;
	}

	USOWeaponData* OldWeapon = EquippedWeapon;
	EquippedWeapon = NewWeapon;
	OnWeaponChanged.Broadcast(OldWeapon, EquippedWeapon);
}

void ASOCharacter::UnequipWeapon()
{
	EquipWeapon(nullptr);
}

float ASOCharacter::GetEffectivePrimaryAttackDamage() const
{
	return PrimaryAttackDamage + (EquippedWeapon ? EquippedWeapon->PrimaryDamageBonus : 0.0f);
}

float ASOCharacter::GetEffectivePrimaryAttackCooldown() const
{
	const float Mult = EquippedWeapon ? FMath::Max(0.05f, EquippedWeapon->PrimaryAttackCooldownMultiplier) : 1.0f;
	return PrimaryAttackCooldown * Mult;
}

float ASOCharacter::GetEffectiveShadowBoltDamage() const
{
	return ShadowBoltBaseDamage + (EquippedWeapon ? EquippedWeapon->ShadowBoltDamageBonus : 0.0f);
}

void ASOCharacter::AddGold(int32 Amount)
{
	if (Amount == 0)
	{
		return;
	}

	const int32 OldGold = Gold;
	Gold = FMath::Max(0, Gold + Amount);
	const int32 Delta = Gold - OldGold;
	if (Delta != 0)
	{
		OnGoldChanged.Broadcast(OldGold, Gold, Delta);
	}
}

void ASOCharacter::HandleDeath(USOHealthComponent* /*OwningComponent*/, AController* InstigatedBy, AActor* DamageCauser)
{
	OnCharacterDied(InstigatedBy, DamageCauser);
}

void ASOCharacter::HandleLevelUp(USOExperienceComponent* /*OwningComponent*/, int32 NewLevel, int32 PreviousLevel)
{
	const int32 LevelsGained = FMath::Max(0, NewLevel - PreviousLevel);
	if (LevelsGained == 0)
	{
		return;
	}

	// Bump stats and top the player off - level ups feel meaningful because
	// the pool grows *and* refills.
	if (HealthComponent && MaxHealthPerLevel > 0.0f)
	{
		HealthComponent->MaxHealth += MaxHealthPerLevel * LevelsGained;
		HealthComponent->Revive(HealthComponent->MaxHealth);
	}

	if (ManaComponent && MaxManaPerLevel > 0.0f)
	{
		ManaComponent->MaxMana += MaxManaPerLevel * LevelsGained;
		ManaComponent->RefillToMax();
	}

	PrimaryAttackDamage  += PrimaryDamagePerLevel   * LevelsGained;
	ShadowBoltBaseDamage += ShadowBoltDamagePerLevel * LevelsGained;

	OnLevelUpReached(NewLevel);
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

		UGameplayStatics::ApplyDamage(HitActor, GetEffectivePrimaryAttackDamage(), InstigatorController, this, DTClass);
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
		GetEffectivePrimaryAttackCooldown(),
		false);
}

bool ASOCharacter::CanCastShadowBolt() const
{
	if (!IsAlive() || bShadowBoltOnCooldown || ShadowBoltClass == nullptr)
	{
		return false;
	}

	if (ShadowBoltManaCost > 0.0f && ManaComponent && !ManaComponent->HasEnough(ShadowBoltManaCost))
	{
		return false;
	}

	return true;
}

void ASOCharacter::CastShadowBolt(FVector TargetLocation)
{
	if (!CanCastShadowBolt())
	{
		return;
	}

	// Consume mana before we spawn. If the pool is missing entirely we just
	// let the cast go through (allows enemies without a mana component to
	// still fire this ability later without ceremony).
	if (ShadowBoltManaCost > 0.0f && ManaComponent && !ManaComponent->Consume(ShadowBoltManaCost))
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

	// Push the character's effective spell damage onto the projectile so it
	// scales with equipped weapons without touching the projectile BP.
	Bolt->Damage = GetEffectiveShadowBoltDamage();

	OnShadowBoltCast(Muzzle, AimDirection, Bolt);

	bShadowBoltOnCooldown = true;
	World->GetTimerManager().SetTimer(
		ShadowBoltCooldownHandle,
		FTimerDelegate::CreateLambda([this]() { bShadowBoltOnCooldown = false; }),
		ShadowBoltCooldown,
		false);
}

bool ASOCharacter::CanCastLifeDrain() const
{
	if (!IsAlive() || bLifeDrainOnCooldown)
	{
		return false;
	}
	if (LifeDrainManaCost > 0.0f && ManaComponent && !ManaComponent->HasEnough(LifeDrainManaCost))
	{
		return false;
	}
	return true;
}

void ASOCharacter::CastLifeDrain()
{
	if (!CanCastLifeDrain())
	{
		return;
	}

	if (LifeDrainManaCost > 0.0f && ManaComponent && !ManaComponent->Consume(LifeDrainManaCost))
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Center = GetActorLocation();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(SOLifeDrain), /*bTraceComplex=*/ false, /*Ignore=*/ this);

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByChannel(
		Overlaps,
		Center,
		FQuat::Identity,
		LifeDrainChannel,
		FCollisionShape::MakeSphere(LifeDrainRadius),
		Params);

	TSubclassOf<UDamageType> DTClass = LifeDrainDamageType
		? LifeDrainDamageType
		: TSubclassOf<UDamageType>(USODamageType::StaticClass());

	AController* InstigatorController = GetController();

	TArray<AActor*> UniqueHitActors;
	UniqueHitActors.Reserve(Overlaps.Num());
	float TotalDamage = 0.0f;

	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* HitActor = Result.GetActor();
		if (!HitActor || HitActor == this)
		{
			continue;
		}
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

		TotalDamage += UGameplayStatics::ApplyDamage(HitActor, LifeDrainDamage, InstigatorController, this, DTClass);
	}

	float HealedAmount = 0.0f;
	if (LifeDrainHealFraction > 0.0f && TotalDamage > 0.0f && HealthComponent)
	{
		HealedAmount = HealthComponent->Heal(TotalDamage * LifeDrainHealFraction, InstigatorController, this);
	}

	OnLifeDrainCast(UniqueHitActors, TotalDamage, HealedAmount);

	if (bDrawLifeDrainDebug)
	{
		DrawDebugSphere(World, Center, LifeDrainRadius, 24, FColor(140, 20, 200), false, 0.8f, 0, 2.0f);
	}

	bLifeDrainOnCooldown = true;
	World->GetTimerManager().SetTimer(
		LifeDrainCooldownHandle,
		FTimerDelegate::CreateLambda([this]() { bLifeDrainOnCooldown = false; }),
		LifeDrainCooldown,
		false);
}
