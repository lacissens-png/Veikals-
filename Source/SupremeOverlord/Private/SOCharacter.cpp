#include "SOCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SOAttributesComponent.h"
#include "SOEquipmentComponent.h"
#include "SOInventoryComponent.h"
#include "SOQuestComponent.h"
#include "SOStatusEffectComponent.h"
#include "SOTalentComponent.h"
#include "SODamageType.h"
#include "SOExperienceComponent.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"
#include "SOSaveGame.h"
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
	AttributesComponent = CreateDefaultSubobject<USOAttributesComponent>(TEXT("AttributesComponent"));
	TalentComponent     = CreateDefaultSubobject<USOTalentComponent>(TEXT("TalentComponent"));
	EquipmentComponent  = CreateDefaultSubobject<USOEquipmentComponent>(TEXT("EquipmentComponent"));
	InventoryComponent  = CreateDefaultSubobject<USOInventoryComponent>(TEXT("InventoryComponent"));
	QuestComponent        = CreateDefaultSubobject<USOQuestComponent>(TEXT("QuestComponent"));
	StatusEffectComponent = CreateDefaultSubobject<USOStatusEffectComponent>(TEXT("StatusEffectComponent"));

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

	if (AttributesComponent)
	{
		AttributesComponent->GrantPoints(AttributesComponent->PointsPerLevel * LevelsGained);
	}

	if (TalentComponent)
	{
		TalentComponent->GrantPoints(TalentComponent->TalentPointsPerLevel * LevelsGained);
	}

	if (LevelUpSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LevelUpSFX, GetActorLocation());
	}
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

	if (DeathSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSFX, GetActorLocation());
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

	if (PrimaryAttackSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PrimaryAttackSFX, GetActorLocation());
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

	if (ShadowBoltCastSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShadowBoltCastSFX, Muzzle);
	}
	OnShadowBoltCast(Muzzle, AimDirection, Bolt);

	bShadowBoltOnCooldown = true;
	World->GetTimerManager().SetTimer(
		ShadowBoltCooldownHandle,
		FTimerDelegate::CreateLambda([this]() { bShadowBoltOnCooldown = false; }),
		ShadowBoltCooldown,
		false);
}

float ASOCharacter::GetPrimaryAttackCooldownRemaining() const
{
	if (UWorld* World = GetWorld())
	{
		return FMath::Max(0.0f, World->GetTimerManager().GetTimerRemaining(PrimaryAttackCooldownHandle));
	}
	return 0.0f;
}

float ASOCharacter::GetShadowBoltCooldownRemaining() const
{
	if (UWorld* World = GetWorld())
	{
		return FMath::Max(0.0f, World->GetTimerManager().GetTimerRemaining(ShadowBoltCooldownHandle));
	}
	return 0.0f;
}

float ASOCharacter::GetLifeDrainCooldownRemaining() const
{
	if (UWorld* World = GetWorld())
	{
		return FMath::Max(0.0f, World->GetTimerManager().GetTimerRemaining(LifeDrainCooldownHandle));
	}
	return 0.0f;
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

	if (LifeDrainCastSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LifeDrainCastSFX, Center);
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

bool ASOCharacter::SaveGameToSlotName(const FString& Slot)
{
	const FString UseSlot = Slot.IsEmpty() ? DefaultSaveSlot : Slot;

	USOSaveGame* Save = Cast<USOSaveGame>(UGameplayStatics::CreateSaveGameObject(USOSaveGame::StaticClass()));
	if (!Save)
	{
		OnSaveGameCompleted(true, false, UseSlot);
		return false;
	}

	Save->SlotName  = UseSlot;
	Save->UserIndex = DefaultSaveUserIndex;
	Save->SavedAt   = FDateTime::UtcNow();

	if (ExperienceComponent)
	{
		Save->CharacterLevel    = ExperienceComponent->GetCurrentLevel();
		Save->XPInCurrentLevel  = ExperienceComponent->GetCurrentXPInLevel();
	}

	Save->Gold = Gold;

	if (AttributesComponent)
	{
		Save->Strength               = AttributesComponent->GetAttribute(ESOAttribute::Strength);
		Save->Intellect              = AttributesComponent->GetAttribute(ESOAttribute::Intellect);
		Save->Vitality               = AttributesComponent->GetAttribute(ESOAttribute::Vitality);
		Save->UnspentAttributePoints = AttributesComponent->GetUnspentPoints();
	}

	if (HealthComponent)
	{
		Save->MaxHealth     = HealthComponent->MaxHealth;
		Save->CurrentHealth = HealthComponent->GetCurrentHealth();
	}

	if (ManaComponent)
	{
		Save->MaxMana     = ManaComponent->MaxMana;
		Save->CurrentMana = ManaComponent->GetCurrentMana();
	}

	Save->PrimaryAttackDamage  = PrimaryAttackDamage;
	Save->ShadowBoltBaseDamage = ShadowBoltBaseDamage;

	if (EquippedWeapon)
	{
		Save->EquippedWeaponPath = EquippedWeapon->GetPathName();
	}
	else
	{
		Save->EquippedWeaponPath.Reset();
	}

	const bool bOK = UGameplayStatics::SaveGameToSlot(Save, UseSlot, DefaultSaveUserIndex);
	OnSaveGameCompleted(true, bOK, UseSlot);
	return bOK;
}

bool ASOCharacter::LoadGameFromSlotName(const FString& Slot)
{
	const FString UseSlot = Slot.IsEmpty() ? DefaultSaveSlot : Slot;

	if (!UGameplayStatics::DoesSaveGameExist(UseSlot, DefaultSaveUserIndex))
	{
		OnSaveGameCompleted(false, false, UseSlot);
		return false;
	}

	USOSaveGame* Save = Cast<USOSaveGame>(UGameplayStatics::LoadGameFromSlot(UseSlot, DefaultSaveUserIndex));
	if (!Save)
	{
		OnSaveGameCompleted(false, false, UseSlot);
		return false;
	}

	// Pools first, so subsequent stat adjusts write into the correct MaxHealth/MaxMana.
	if (HealthComponent)
	{
		HealthComponent->MaxHealth = Save->MaxHealth;
		HealthComponent->Revive(FMath::Clamp(Save->CurrentHealth, 0.0f, Save->MaxHealth));
	}
	if (ManaComponent)
	{
		ManaComponent->MaxMana = Save->MaxMana;
		const float Delta = Save->CurrentMana - ManaComponent->GetCurrentMana();
		if (Delta > 0.0f)
		{
			ManaComponent->Restore(Delta);
		}
	}

	PrimaryAttackDamage  = Save->PrimaryAttackDamage;
	ShadowBoltBaseDamage = Save->ShadowBoltBaseDamage;

	if (ExperienceComponent)
	{
		// GainXP would trigger level-up handlers and stack duplicate scaling — instead do a raw reset via new starting level.
		ExperienceComponent->StartingLevel = FMath::Max(1, Save->CharacterLevel);
		// The component's internal fields aren't publicly settable to avoid save-driven abuse elsewhere,
		// so we award the banked XPInCurrentLevel through GainXP after re-parking StartingLevel.
		// Cleanest MVP: just call GainXP with what was banked.
		ExperienceComponent->GainXP(Save->XPInCurrentLevel);
	}

	Gold = FMath::Max(0, Save->Gold);
	OnGoldChanged.Broadcast(0, Gold, Gold);

	if (AttributesComponent)
	{
		// Diff against currently-applied values so the effects apply exactly once.
		const int32 DeltaS = Save->Strength  - AttributesComponent->GetAttribute(ESOAttribute::Strength);
		const int32 DeltaI = Save->Intellect - AttributesComponent->GetAttribute(ESOAttribute::Intellect);
		const int32 DeltaV = Save->Vitality  - AttributesComponent->GetAttribute(ESOAttribute::Vitality);

		if (DeltaS > 0) { AttributesComponent->GrantPoints(DeltaS); for (int32 i = 0; i < DeltaS; ++i) AttributesComponent->AllocatePoint(ESOAttribute::Strength);  }
		if (DeltaI > 0) { AttributesComponent->GrantPoints(DeltaI); for (int32 i = 0; i < DeltaI; ++i) AttributesComponent->AllocatePoint(ESOAttribute::Intellect); }
		if (DeltaV > 0) { AttributesComponent->GrantPoints(DeltaV); for (int32 i = 0; i < DeltaV; ++i) AttributesComponent->AllocatePoint(ESOAttribute::Vitality);  }

		AttributesComponent->GrantPoints(Save->UnspentAttributePoints);
	}

	if (!Save->EquippedWeaponPath.IsEmpty())
	{
		if (USOWeaponData* Weapon = LoadObject<USOWeaponData>(nullptr, *Save->EquippedWeaponPath))
		{
			EquipWeapon(Weapon);
		}
	}
	else
	{
		UnequipWeapon();
	}

	OnSaveGameCompleted(false, true, UseSlot);
	return true;
}

bool ASOCharacter::QuickSave()
{
	return SaveGameToSlotName(DefaultSaveSlot);
}

bool ASOCharacter::QuickLoad()
{
	return LoadGameFromSlotName(DefaultSaveSlot);
}
