#include "SOCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SOAchievementComponent.h"
#include "SOAttributesComponent.h"
#include "SOAuraComponent.h"
#include "SOBestiaryComponent.h"
#include "SOBlinkComponent.h"
#include "SOConsumableComponent.h"
#include "SOCorpseExplosionComponent.h"
#include "SORealitySlashComponent.h"
#include "SOCorruptionComponent.h"
#include "SOCursedGround.h"
#include "SODodgeRollComponent.h"
#include "SOEnemyCharacter.h"
#include "SOEquipmentComponent.h"
#include "SOFamiliarActor.h"
#include "SOTrap.h"
#include "SODifficultySubsystem.h"
#include "SOGemData.h"
#include "SOInventoryComponent.h"
#include "SOItemData.h"
#include "SOLootRoller.h"
#include "SOMaterialData.h"
#include "SOMinion.h"
#include "SOQuestComponent.h"
#include "SOQuestData.h"
#include "SOStatusEffectComponent.h"
#include "SOSummonComponent.h"
#include "SOTalentComponent.h"
#include "SOTalentNode.h"
#include "SOVassalComponent.h"
#include "SOVassalData.h"
#include "SODamageType.h"
#include "SOExperienceComponent.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"
#include "SOSaveGame.h"
#include "SOShadowBoltProjectile.h"
#include "SOWaypoint.h"
#include "SOWaypointComponent.h"
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
	SummonComponent       = CreateDefaultSubobject<USOSummonComponent>(TEXT("SummonComponent"));
	AuraComponent         = CreateDefaultSubobject<USOAuraComponent>(TEXT("AuraComponent"));
	CorruptionComponent   = CreateDefaultSubobject<USOCorruptionComponent>(TEXT("CorruptionComponent"));
	CorpseExplosionComponent = CreateDefaultSubobject<USOCorpseExplosionComponent>(TEXT("CorpseExplosionComponent"));
	BlinkComponent           = CreateDefaultSubobject<USOBlinkComponent>(TEXT("BlinkComponent"));
	DodgeRollComponent       = CreateDefaultSubobject<USODodgeRollComponent>(TEXT("DodgeRollComponent"));
	WaypointComponent        = CreateDefaultSubobject<USOWaypointComponent>(TEXT("WaypointComponent"));
	BestiaryComponent        = CreateDefaultSubobject<USOBestiaryComponent>(TEXT("BestiaryComponent"));
	ConsumableComponent      = CreateDefaultSubobject<USOConsumableComponent>(TEXT("ConsumableComponent"));
	AchievementComponent     = CreateDefaultSubobject<USOAchievementComponent>(TEXT("AchievementComponent"));
	RealitySlashComponent    = CreateDefaultSubobject<USORealitySlashComponent>(TEXT("RealitySlashComponent"));
	VassalComponent          = CreateDefaultSubobject<USOVassalComponent>(TEXT("VassalComponent"));

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
		HealthComponent->OnHealthChanged.AddDynamic(this, &ASOCharacter::HandleHealthChanged);
	}

	if (BestiaryComponent)
	{
		BestiaryComponent->OnBestiaryEntryUpdated.AddDynamic(this, &ASOCharacter::HandleBestiaryEntryUpdated);
	}

	if (WaypointComponent)
	{
		WaypointComponent->OnWaypointDiscovered.AddDynamic(this, &ASOCharacter::HandleWaypointDiscovered);
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

	if (FamiliarClass)
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			Params.Owner = this;

			ActiveFamiliar = World->SpawnActor<ASOFamiliarActor>(FamiliarClass, GetActorLocation(), GetActorRotation(), Params);
			if (ActiveFamiliar)
			{
				ActiveFamiliar->OwnerCharacter = this;
			}
		}
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
	const float Base = PrimaryAttackDamage + (EquippedWeapon ? EquippedWeapon->PrimaryDamageBonus : 0.0f);
	const float Mult = CorruptionComponent ? CorruptionComponent->GetOutgoingDamageMultiplier() : 1.0f;
	return Base * Mult;
}

float ASOCharacter::GetEffectivePrimaryAttackCooldown() const
{
	const float WeaponMult = EquippedWeapon ? FMath::Max(0.05f, EquippedWeapon->PrimaryAttackCooldownMultiplier) : 1.0f;
	const float VassalMult = VassalComponent ? VassalComponent->GetAttackCooldownMultiplier() : 1.0f;
	return PrimaryAttackCooldown * WeaponMult * VassalMult;
}

float ASOCharacter::GetEffectiveShadowBoltDamage() const
{
	const float Base = ShadowBoltBaseDamage + (EquippedWeapon ? EquippedWeapon->ShadowBoltDamageBonus : 0.0f);
	const float Mult = CorruptionComponent ? CorruptionComponent->GetOutgoingDamageMultiplier() : 1.0f;
	return Base * Mult;
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

	if (Gold >= 1000 && AchievementComponent)
	{
		AchievementComponent->UnlockAchievement(TEXT("rich"), FText::FromString(TEXT("Ill-Gotten Gains: Amass 1000 gold")));
	}
}

void ASOCharacter::HandleDeath(USOHealthComponent* /*OwningComponent*/, AController* InstigatedBy, AActor* DamageCauser)
{
	OnCharacterDied(InstigatedBy, DamageCauser);

	if (bAutoRespawn && RespawnDelay > 0.0f)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ASOCharacter::Respawn, RespawnDelay, false);
		}
	}
}

void ASOCharacter::HandleHealthChanged(USOHealthComponent* OwningComponent, float /*OldHealth*/, float /*NewHealth*/,
                                       float Delta, AController* /*InstigatedBy*/, AActor* /*DamageCauser*/)
{
	if (Delta < 0.0f && IsAlive())
	{
		// A heavy boss slam (HitReactStrength near 1) shakes the camera harder than a chip DOT tick.
		const float ShakeScale = OwningComponent ? OwningComponent->GetLastHitReactStrength() : 1.0f;
		TriggerHitImpact(ShakeScale);
	}
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

	if (AchievementComponent)
	{
		if (NewLevel >= 10)
		{
			AchievementComponent->UnlockAchievement(TEXT("level_10"), FText::FromString(TEXT("Rising Power: Reach level 10")));
		}
		if (NewLevel >= 25)
		{
			AchievementComponent->UnlockAchievement(TEXT("level_25"), FText::FromString(TEXT("Overlord in Training: Reach level 25")));
		}
	}
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
	// See HandleDeath / Respawn() below for what happens next.
}

void ASOCharacter::Respawn()
{
	if (!HealthComponent || HealthComponent->IsAlive())
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RespawnTimerHandle);
	}

	if (RespawnGoldPenaltyFraction > 0.0f && Gold > 0)
	{
		AddGold(-FMath::RoundToInt(Gold * RespawnGoldPenaltyFraction));
	}

	HealthComponent->Revive();
	if (ManaComponent)
	{
		ManaComponent->RefillToMax();
	}
	if (ConsumableComponent)
	{
		ConsumableComponent->RefillCharges();
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->SetMovementMode(MOVE_Walking);
	}

	if (WaypointComponent)
	{
		if (ASOWaypoint* Waypoint = WaypointComponent->GetLastWaypoint())
		{
			TeleportTo(Waypoint->GetTravelDestination(), GetActorRotation(), false, false);
		}
	}

	OnCharacterRespawned();
}

float ASOCharacter::GetRespawnTimeRemaining() const
{
	if (!HealthComponent || HealthComponent->IsAlive())
	{
		return 0.0f;
	}
	if (const UWorld* World = GetWorld())
	{
		return FMath::Max(0.0f, World->GetTimerManager().GetTimerRemaining(RespawnTimerHandle));
	}
	return 0.0f;
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

bool ASOCharacter::IsComboWindowActive() const
{
	if (ComboStage <= 0)
	{
		return false;
	}
	const UWorld* World = GetWorld();
	return World && (World->GetTimeSeconds() - LastPrimaryAttackTimestamp <= ComboWindowSeconds);
}

float ASOCharacter::GetPrimaryAttackArcDegrees() const
{
	if (!EquippedWeapon)
	{
		return UnarmedAttackArcDegrees;
	}
	switch (EquippedWeapon->WeaponType)
	{
	case ESOWeaponType::Axe:  return CleaveWeaponArcDegrees;
	case ESOWeaponType::Mace: return CrushWeaponArcDegrees;
	default:                  return ThrustWeaponArcDegrees; // Sword, Staff, Wand, Orb
	}
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

	// Advance the combo chain if this swing landed inside the window since the
	// last one, otherwise the gap was too long and the chain resets to stage 0.
	const float Now             = World->GetTimeSeconds();
	const int32 NumComboStages  = FMath::Max(1, ComboDamageMultipliers.Num());
	ComboStage = (Now - LastPrimaryAttackTimestamp > ComboWindowSeconds)
		? 0
		: (ComboStage + 1) % NumComboStages;
	LastPrimaryAttackTimestamp = Now;
	const bool bIsFinisher = (ComboStage == NumComboStages - 1);

	// Cone check in front of the character: anything inside PrimaryAttackRadius
	// is always hit (point-blank), everything else out to PrimaryAttackRange
	// needs to be within the equipped weapon's swing arc - widened further on
	// the combo finisher for a bigger finishing cleave.
	const FVector Forward    = GetActorForwardVector();
	const float   ArcDegrees = FMath::Clamp(GetPrimaryAttackArcDegrees() + (bIsFinisher ? ComboFinisherArcBonusDegrees : 0.0f), 0.0f, 360.0f);
	const float   CosHalfArc = FMath::Cos(FMath::DegreesToRadians(ArcDegrees * 0.5f));
	const float   RadiusSq   = FMath::Square(PrimaryAttackRadius);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(SOPrimaryAttack), /*bTraceComplex=*/ false, /*Ignore=*/ this);

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByChannel(
		Overlaps,
		MyLocation,
		FQuat::Identity,
		PrimaryAttackChannel,
		FCollisionShape::MakeSphere(PrimaryAttackRange),
		Params);

	TArray<AActor*> UniqueHitActors;
	UniqueHitActors.Reserve(Overlaps.Num());

	TSubclassOf<UDamageType> DTClass = PrimaryAttackDamageType
		? PrimaryAttackDamageType
		: TSubclassOf<UDamageType>(USODamageType::StaticClass());

	AController* InstigatorController = GetController();

	float TotalDamageDealt = 0.0f;
	const float ComboMult  = ComboDamageMultipliers.IsValidIndex(ComboStage) ? ComboDamageMultipliers[ComboStage] : 1.0f;

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

		const FVector ToActor = HitActor->GetActorLocation() - MyLocation;
		if (ToActor.SizeSquared() > RadiusSq)
		{
			FVector FlatToActor = ToActor;
			FlatToActor.Z        = 0.0f;
			const FVector DirToActor = FlatToActor.GetSafeNormal();
			if (!DirToActor.IsNearlyZero() && FVector::DotProduct(Forward, DirToActor) < CosHalfArc)
			{
				continue; // outside the weapon's swing arc, and outside the point-blank bubble
			}
		}

		UniqueHitActors.Add(HitActor);

		// Each target hit by the swing rolls its own crit - a wide-arc cleave
		// hitting several enemies gets several independent chances, rewarding
		// good positioning rather than one roll deciding the whole swing.
		const bool  bCrit       = RollCriticalHit();
		const float SwingDamage = GetEffectivePrimaryAttackDamage() * ComboMult * (bCrit ? CritDamageMultiplier : 1.0f);
		UGameplayStatics::ApplyDamage(HitActor, SwingDamage, InstigatorController, this, DTClass);
		TotalDamageDealt += SwingDamage;

		if (bCrit)
		{
			OnCriticalHit.Broadcast(HitActor, SwingDamage);
		}
	}

	// Vampiric Strikes legendary — heal back a fraction of everything this swing dealt.
	if (TotalDamageDealt > 0.0f && HealthComponent && HasLegendaryEffect(ESOLegendaryEffect::VampiricStrikes))
	{
		HealthComponent->Heal(TotalDamageDealt * VampiricStrikesHealFraction, InstigatorController, this);
	}

	if (UniqueHitActors.Num() > 0)
	{
		TriggerHitImpact(0.5f);
	}

	if (PrimaryAttackSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PrimaryAttackSFX, GetActorLocation());
	}
	OnPrimaryAttackPerformed(MyLocation, UniqueHitActors, ComboStage);

	if (bDrawPrimaryAttackDebug)
	{
		DrawDebugSphere(World, MyLocation, PrimaryAttackRange, 24, FColor::Green, false, 0.6f, 0, 1.5f);
		DrawDebugSphere(World, MyLocation, PrimaryAttackRadius, 16, FColor::Cyan, false, 0.6f, 0, 1.5f);
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
	// scales with equipped weapons without touching the projectile BP. The
	// cast rolls a single crit up front (unlike Primary Attack's per-target
	// rolls) since a bolt only ever has one primary target.
	const bool bCrit  = RollCriticalHit();
	Bolt->Damage      = GetEffectiveShadowBoltDamage() * (bCrit ? CritDamageMultiplier : 1.0f);
	Bolt->bIsCrit     = bCrit;
	Bolt->bChainOnHit = HasLegendaryEffect(ESOLegendaryEffect::ShadowBoltChain);

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

float ASOCharacter::GetSummonCooldownRemaining() const
{
	return SummonComponent ? SummonComponent->GetSummonCooldownRemaining() : 0.0f;
}

float ASOCharacter::GetSummonCooldown() const
{
	return SummonComponent ? SummonComponent->SummonCooldown : 1.0f;
}

void ASOCharacter::CastSummonMinion(FVector TargetLocation)
{
	if (!IsAlive() || !SummonComponent)
	{
		return;
	}
	if (SummonComponent->SummonMinion(TargetLocation, this))
	{
		if (HasLegendaryEffect(ESOLegendaryEffect::EndlessMinions))
		{
			if (ASOMinion* Minion = SummonComponent->GetLastSpawnedMinion())
			{
				Minion->SetLifeSpan(0.0f);
			}
		}

		if (SummonCastSFX)
		{
			UGameplayStatics::PlaySoundAtLocation(this, SummonCastSFX, GetActorLocation());
		}
	}
}

void ASOCharacter::DismissAllMinions()
{
	if (SummonComponent)
	{
		SummonComponent->DismissAll();
	}
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

		// Same per-target crit roll pattern as Primary Attack - each enemy
		// caught in the drain gets its own independent chance.
		const bool  bCrit  = RollCriticalHit();
		const float Damage = LifeDrainDamage * (bCrit ? CritDamageMultiplier : 1.0f);
		const float Applied = UGameplayStatics::ApplyDamage(HitActor, Damage, InstigatorController, this, DTClass);
		TotalDamage += Applied;

		if (bCrit)
		{
			OnCriticalHit.Broadcast(HitActor, Applied);
		}
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
	Save->MovementSpeed        = MovementSpeed;

	if (EquipmentComponent)
	{
		for (const TPair<ESOEquipSlot, TObjectPtr<USOItemData>>& Pair : EquipmentComponent->GetAllEquipped())
		{
			USOItemData* Item = Pair.Value;
			if (!Item)
			{
				continue;
			}

			FSOSavedItemInstance SavedItem;
			SavedItem.SourceTemplate = Item->SourceTemplate.IsNull() ? TSoftObjectPtr<USOItemData>(Item) : Item->SourceTemplate;
			SavedItem.Rarity         = Item->Rarity;
			SavedItem.ItemLevel      = Item->ItemLevel;
			SavedItem.RolledAffixes  = Item->RolledAffixes;
			for (USOGemData* Gem : Item->SocketedGems)
			{
				if (Gem)
				{
					SavedItem.SocketedGemPaths.Add(Gem);
				}
			}

			Save->EquippedItems.Add(Pair.Key, SavedItem);
		}
	}

	if (InventoryComponent)
	{
		for (const TPair<TObjectPtr<USOMaterialData>, int32>& Pair : InventoryComponent->GetAllMaterials())
		{
			if (!Pair.Key)
			{
				continue;
			}
			FSOSavedMaterialCount SavedCount;
			SavedCount.Material = Pair.Key.Get();
			SavedCount.Count    = Pair.Value;
			Save->MaterialCounts.Add(SavedCount);
		}
	}

	if (TalentComponent)
	{
		for (USOTalentNode* Node : TalentComponent->GetUnlockedNodes())
		{
			if (Node)
			{
				Save->UnlockedTalentNodes.Add(Node);
			}
		}
		Save->AvailableTalentPoints = TalentComponent->GetTalentPoints();
	}

	if (QuestComponent)
	{
		for (const FSOActiveQuest& AQ : QuestComponent->ActiveQuests)
		{
			if (!AQ.Data)
			{
				continue;
			}
			FSOSavedQuestProgress Progress;
			Progress.Quest    = AQ.Data.Get();
			Progress.Progress = AQ.Progress;
			Save->ActiveQuestProgress.Add(Progress);
		}
		for (USOQuestData* Quest : QuestComponent->CompletedQuests)
		{
			if (Quest)
			{
				Save->CompletedQuests.Add(Quest);
			}
		}
	}

	if (BestiaryComponent)
	{
		Save->BestiaryKillCounts = BestiaryComponent->GetAllKillCounts();
	}

	if (AchievementComponent)
	{
		Save->UnlockedAchievements = AchievementComponent->GetUnlockedAchievements();
	}

	if (ConsumableComponent)
	{
		Save->PotionCharges = ConsumableComponent->GetCurrentCharges();
	}

	if (WaypointComponent)
	{
		Save->DiscoveredWaypointIDs = WaypointComponent->GetDiscoveredWaypointIDs();
		Save->LastWaypointID        = WaypointComponent->GetLastWaypointID();
	}

	if (USODifficultySubsystem* Difficulty = USODifficultySubsystem::Get(GetWorld()))
	{
		Save->DifficultyTier = Difficulty->GetDifficultyTier();
	}

	if (CorruptionComponent)
	{
		Save->CorruptionAmount = CorruptionComponent->GetCorruption();
	}

	if (VassalComponent)
	{
		for (USOVassalData* Vassal : VassalComponent->GetRecruitedVassals())
		{
			if (Vassal)
			{
				Save->RecruitedVassals.Add(Vassal);
			}
		}
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
	MovementSpeed        = Save->MovementSpeed;
	ApplyMovementSettings();

	if (ExperienceComponent)
	{
		ExperienceComponent->RestoreFromSave(Save->CharacterLevel, Save->XPInCurrentLevel);
	}

	Gold = FMath::Max(0, Save->Gold);
	OnGoldChanged.Broadcast(0, Gold, Gold);

	if (AttributesComponent)
	{
		AttributesComponent->RestoreFromSave(Save->Strength, Save->Intellect, Save->Vitality, Save->UnspentAttributePoints);
	}

	if (EquipmentComponent)
	{
		for (const TPair<ESOEquipSlot, FSOSavedItemInstance>& Pair : Save->EquippedItems)
		{
			USOItemData* Template = Pair.Value.SourceTemplate.LoadSynchronous();
			if (!Template)
			{
				continue;
			}

			USOItemData* Restored = USOLootRoller::ReconstructItemInstance(
				Template, GetTransientPackage(), Pair.Value.Rarity, Pair.Value.ItemLevel, Pair.Value.RolledAffixes);
			if (!Restored)
			{
				continue;
			}

			EquipmentComponent->Equip(Restored);

			for (const TSoftObjectPtr<USOGemData>& GemPath : Pair.Value.SocketedGemPaths)
			{
				if (USOGemData* Gem = GemPath.LoadSynchronous())
				{
					EquipmentComponent->SocketGem(Pair.Key, Gem);
				}
			}
		}

		// Equip() -> RecomputeAggregateStats() just added the full equipment
		// bonus on top of MaxHealth/MaxMana/MovementSpeed — but those were
		// already restored to their final, bonus-inclusive saved totals above,
		// so this double-counts it. Snap back to the saved totals now that
		// every item is equipped and EquipmentComponent's Applied* trackers
		// correctly reflect the aggregate (so future equip/unequip deltas,
		// e.g. picking up a new drop, still compute correctly from here).
		if (HealthComponent)
		{
			HealthComponent->MaxHealth = Save->MaxHealth;
		}
		if (ManaComponent)
		{
			ManaComponent->MaxMana = Save->MaxMana;
		}
		MovementSpeed = Save->MovementSpeed;
		ApplyMovementSettings();
	}

	if (InventoryComponent)
	{
		TMap<USOMaterialData*, int32> Materials;
		for (const FSOSavedMaterialCount& SavedCount : Save->MaterialCounts)
		{
			if (USOMaterialData* Material = SavedCount.Material.LoadSynchronous())
			{
				Materials.Add(Material, SavedCount.Count);
			}
		}
		InventoryComponent->RestoreFromSave(Materials);
	}

	if (TalentComponent)
	{
		TArray<USOTalentNode*> Nodes;
		for (const TSoftObjectPtr<USOTalentNode>& NodePath : Save->UnlockedTalentNodes)
		{
			if (USOTalentNode* Node = NodePath.LoadSynchronous())
			{
				Nodes.Add(Node);
			}
		}
		TalentComponent->RestoreFromSave(Nodes, Save->AvailableTalentPoints);
	}

	if (QuestComponent)
	{
		TArray<FSOActiveQuest> Active;
		for (const FSOSavedQuestProgress& Progress : Save->ActiveQuestProgress)
		{
			USOQuestData* Quest = Progress.Quest.LoadSynchronous();
			if (!Quest)
			{
				continue;
			}
			FSOActiveQuest AQ;
			AQ.Data     = Quest;
			AQ.Progress = Progress.Progress;
			Active.Add(AQ);
		}

		TArray<USOQuestData*> Completed;
		for (const TSoftObjectPtr<USOQuestData>& QuestPath : Save->CompletedQuests)
		{
			if (USOQuestData* Quest = QuestPath.LoadSynchronous())
			{
				Completed.Add(Quest);
			}
		}

		QuestComponent->RestoreQuestState(Active, Completed);
	}

	if (BestiaryComponent)
	{
		for (const TPair<TSubclassOf<ASOEnemyCharacter>, int32>& Pair : Save->BestiaryKillCounts)
		{
			BestiaryComponent->RestoreKillCount(Pair.Key, Pair.Value);
		}
	}

	if (AchievementComponent)
	{
		AchievementComponent->RestoreUnlockedAchievements(Save->UnlockedAchievements);
	}

	if (ConsumableComponent)
	{
		ConsumableComponent->SetCurrentCharges(Save->PotionCharges);
	}

	if (WaypointComponent)
	{
		UWorld* World = GetWorld();
		for (const FName& ID : Save->DiscoveredWaypointIDs)
		{
			WaypointComponent->RestoreDiscoveredWaypointByID(ID, World);
		}
		if (!Save->LastWaypointID.IsNone())
		{
			WaypointComponent->RestoreLastWaypointByID(Save->LastWaypointID);
		}
	}

	if (USODifficultySubsystem* Difficulty = USODifficultySubsystem::Get(GetWorld()))
	{
		Difficulty->SetDifficultyTier(Save->DifficultyTier);
	}

	if (CorruptionComponent)
	{
		CorruptionComponent->SetCorruption(Save->CorruptionAmount);
	}

	if (VassalComponent)
	{
		TArray<USOVassalData*> Recruited;
		for (const TSoftObjectPtr<USOVassalData>& VassalPath : Save->RecruitedVassals)
		{
			if (USOVassalData* Vassal = VassalPath.LoadSynchronous())
			{
				Recruited.Add(Vassal);
			}
		}
		VassalComponent->RestoreRecruitedVassals(Recruited);
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

// ---------------------------------------------------------------------------
// Trap System
// ---------------------------------------------------------------------------

void ASOCharacter::PlaceTrap(FVector TargetLocation)
{
	if (!IsAlive() || !TrapClass || bTrapPlaceOnCooldown)
	{
		return;
	}

	// Prune destroyed traps before checking the limit.
	ActiveTraps.RemoveAll([](const TWeakObjectPtr<ASOTrap>& W) { return !W.IsValid(); });
	if (ActiveTraps.Num() >= MaxActiveTrapCount)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Params.Owner      = this;
	Params.Instigator = this;

	ASOTrap* Trap = World->SpawnActor<ASOTrap>(TrapClass, TargetLocation, FRotator::ZeroRotator, Params);
	if (!Trap)
	{
		return;
	}

	Trap->TrapType       = SelectedTrapType;
	Trap->OwnerCharacter = this;
	ActiveTraps.Add(Trap);

	if (TrapPlaceSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, TrapPlaceSFX, TargetLocation);
	}

	bTrapPlaceOnCooldown = true;
	World->GetTimerManager().SetTimer(
		TrapPlaceCooldownHandle,
		FTimerDelegate::CreateLambda([this]() { bTrapPlaceOnCooldown = false; }),
		TrapPlaceCooldown,
		false);
}

void ASOCharacter::CycleTrap()
{
	switch (SelectedTrapType)
	{
	case ESOTrapType::ShadowSnare:   SelectedTrapType = ESOTrapType::ArcaneMine;    break;
	case ESOTrapType::ArcaneMine:    SelectedTrapType = ESOTrapType::NecroticSpore; break;
	case ESOTrapType::NecroticSpore: SelectedTrapType = ESOTrapType::ShadowSnare;   break;
	default:                         SelectedTrapType = ESOTrapType::ShadowSnare;   break;
	}
}

float ASOCharacter::GetTrapPlaceCooldownRemaining() const
{
	if (const UWorld* World = GetWorld())
	{
		return FMath::Max(0.0f, World->GetTimerManager().GetTimerRemaining(TrapPlaceCooldownHandle));
	}
	return 0.0f;
}

// ---------------------------------------------------------------------------
// Corruption / Overlord Mode
// ---------------------------------------------------------------------------

void ASOCharacter::ActivateOverlordMode()
{
	if (!IsAlive() || !CorruptionComponent)
	{
		return;
	}
	CorruptionComponent->ActivateOverlordMode();
	if (OverlordModeActivateSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OverlordModeActivateSFX, GetActorLocation());
	}
}

// ---------------------------------------------------------------------------
// Necromantic Resurrection
// ---------------------------------------------------------------------------

void ASOCharacter::CastNecroticResurrect(FVector TargetLocation)
{
	if (!IsAlive() || !SummonComponent)
	{
		return;
	}
	if (SummonComponent->ResurrectAtLocation(TargetLocation, this))
	{
		if (HasLegendaryEffect(ESOLegendaryEffect::EndlessMinions))
		{
			if (ASOMinion* Minion = SummonComponent->GetLastSpawnedMinion())
			{
				Minion->SetLifeSpan(0.0f);
			}
		}

		if (NecroResurrectSFX)
		{
			UGameplayStatics::PlaySoundAtLocation(this, NecroResurrectSFX, TargetLocation);
		}
	}
}

float ASOCharacter::GetNecromancyCooldownRemaining() const
{
	return SummonComponent ? SummonComponent->GetNecromancyCooldownRemaining() : 0.0f;
}

float ASOCharacter::GetNecromancyCooldown() const
{
	return SummonComponent ? SummonComponent->NecromancyCooldown : 1.0f;
}

// ---------------------------------------------------------------------------
// Corpse Explosion
// ---------------------------------------------------------------------------

void ASOCharacter::CastCorpseExplosion(FVector TargetLocation)
{
	if (!IsAlive() || !CorpseExplosionComponent)
	{
		return;
	}
	const bool bFreeCast = HasLegendaryEffect(ESOLegendaryEffect::CorpseExplosionFree);
	if (CorpseExplosionComponent->Cast(TargetLocation, this, bFreeCast))
	{
		if (CorpseExplosionSFX)
		{
			UGameplayStatics::PlaySoundAtLocation(this, CorpseExplosionSFX, TargetLocation);
		}
	}
}

float ASOCharacter::GetCorpseExplosionCooldownRemaining() const
{
	return CorpseExplosionComponent ? CorpseExplosionComponent->GetCooldownRemaining() : 0.0f;
}

// ---------------------------------------------------------------------------
// Shadow Step / Blink
// ---------------------------------------------------------------------------

void ASOCharacter::CastBlink(FVector TargetLocation)
{
	if (!IsAlive() || !BlinkComponent)
	{
		return;
	}
	if (BlinkComponent->Blink(TargetLocation, this))
	{
		if (BlinkSFX)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BlinkSFX, GetActorLocation());
		}
	}
}

float ASOCharacter::GetBlinkCooldownRemaining() const
{
	return BlinkComponent ? BlinkComponent->GetCooldownRemaining() : 0.0f;
}

// ---------------------------------------------------------------------------
// Cursed Ground
// ---------------------------------------------------------------------------

void ASOCharacter::PlaceCursedGround(FVector TargetLocation)
{
	if (!IsAlive() || !CursedGroundClass || bCursedGroundOnCooldown)
	{
		return;
	}

	if (CursedGroundManaCost > 0.0f && (!ManaComponent || !ManaComponent->HasEnough(CursedGroundManaCost)))
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Prune expired zones before checking the limit.
	ActiveCursedGrounds.RemoveAll([](const TWeakObjectPtr<ASOCursedGround>& W) { return !W.IsValid(); });
	if (ActiveCursedGrounds.Num() >= MaxCursedGrounds)
	{
		if (ASOCursedGround* Oldest = ActiveCursedGrounds[0].Get())
		{
			Oldest->Destroy();
		}
		ActiveCursedGrounds.RemoveAt(0);
	}

	if (CursedGroundManaCost > 0.0f)
	{
		ManaComponent->Consume(CursedGroundManaCost);
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Params.Owner      = this;
	Params.Instigator = this;

	ASOCursedGround* Ground = World->SpawnActor<ASOCursedGround>(CursedGroundClass, TargetLocation, FRotator::ZeroRotator, Params);
	if (!Ground)
	{
		return;
	}

	Ground->OwnerCharacter = this;
	ActiveCursedGrounds.Add(Ground);

	if (CursedGroundSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CursedGroundSFX, TargetLocation);
	}

	bCursedGroundOnCooldown = true;
	World->GetTimerManager().SetTimer(
		CursedGroundCooldownHandle,
		FTimerDelegate::CreateLambda([this]() { bCursedGroundOnCooldown = false; }),
		CursedGroundCooldown,
		false);
}

float ASOCharacter::GetCursedGroundCooldownRemaining() const
{
	if (const UWorld* World = GetWorld())
	{
		return FMath::Max(0.0f, World->GetTimerManager().GetTimerRemaining(CursedGroundCooldownHandle));
	}
	return 0.0f;
}

// ---------------------------------------------------------------------------
// Dodge Roll
// ---------------------------------------------------------------------------

void ASOCharacter::CastDodgeRoll(FVector TargetLocation)
{
	if (!IsAlive() || !DodgeRollComponent)
	{
		return;
	}
	if (DodgeRollComponent->Roll(TargetLocation, this))
	{
		if (DodgeRollSFX)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DodgeRollSFX, GetActorLocation());
		}
	}
}

float ASOCharacter::GetDodgeRollCooldownRemaining() const
{
	return DodgeRollComponent ? DodgeRollComponent->GetCooldownRemaining() : 0.0f;
}

// ---------------------------------------------------------------------------
// Legendary Uniques
// ---------------------------------------------------------------------------

void ASOCharacter::TriggerHitImpact(float ShakeScale)
{
	if (HitCameraShakeClass)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			PC->ClientStartCameraShake(HitCameraShakeClass, ShakeScale);
		}
	}

	if (HitStopDuration <= 0.0f || HitStopTimeDilation >= 1.0f)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UGameplayStatics::SetGlobalTimeDilation(World, HitStopTimeDilation);

	// Timers advance on dilated world time, so the requested real-world
	// HitStopDuration must be pre-multiplied by the dilation factor here,
	// otherwise the "restore" timer itself would run in slow motion too.
	World->GetTimerManager().ClearTimer(HitStopTimerHandle);
	World->GetTimerManager().SetTimer(
		HitStopTimerHandle,
		FTimerDelegate::CreateLambda([World]() { UGameplayStatics::SetGlobalTimeDilation(World, 1.0f); }),
		HitStopDuration * HitStopTimeDilation,
		false);
}

bool ASOCharacter::HasLegendaryEffect(ESOLegendaryEffect Effect) const
{
	if (Effect == ESOLegendaryEffect::None || !EquipmentComponent)
	{
		return false;
	}

	for (const TPair<ESOEquipSlot, TObjectPtr<USOItemData>>& Pair : EquipmentComponent->GetAllEquipped())
	{
		if (Pair.Value && Pair.Value->LegendaryEffect == Effect)
		{
			return true;
		}
	}
	return false;
}

// ---------------------------------------------------------------------------
// Waypoint Fast Travel
// ---------------------------------------------------------------------------

void ASOCharacter::ToggleWaypointMap()
{
	if (WaypointComponent)
	{
		WaypointComponent->ToggleMap();
	}
}

bool ASOCharacter::TravelToWaypoint(int32 Index)
{
	return WaypointComponent ? WaypointComponent->TravelToWaypoint(Index) : false;
}

void ASOCharacter::ToggleBestiary()
{
	if (BestiaryComponent)
	{
		BestiaryComponent->ToggleCodex();
	}
}

// ---------------------------------------------------------------------------
// Reality Slash
// ---------------------------------------------------------------------------

void ASOCharacter::CastRealitySlash(FVector TargetLocation)
{
	if (!IsAlive() || !RealitySlashComponent)
	{
		return;
	}
	if (RealitySlashComponent->Cast(TargetLocation, this))
	{
		if (RealitySlashSFX)
		{
			UGameplayStatics::PlaySoundAtLocation(this, RealitySlashSFX, GetActorLocation());
		}
	}
}

float ASOCharacter::GetRealitySlashCooldownRemaining() const
{
	return RealitySlashComponent ? RealitySlashComponent->GetCooldownRemaining() : 0.0f;
}

// ---------------------------------------------------------------------------
// Consumables
// ---------------------------------------------------------------------------

bool ASOCharacter::UsePotion()
{
	return ConsumableComponent ? ConsumableComponent->UsePotion(this) : false;
}

float ASOCharacter::GetPotionCooldownRemaining() const
{
	return ConsumableComponent ? ConsumableComponent->GetCooldownRemaining() : 0.0f;
}

// ---------------------------------------------------------------------------
// Achievements
// ---------------------------------------------------------------------------

void ASOCharacter::HandleBestiaryEntryUpdated(TSubclassOf<ASOEnemyCharacter> /*EnemyClass*/, int32 /*NewCount*/)
{
	if (!BestiaryComponent || !AchievementComponent)
	{
		return;
	}

	const int32 TotalKills = BestiaryComponent->GetTotalKills();
	if (TotalKills >= 1)
	{
		AchievementComponent->UnlockAchievement(TEXT("first_blood"), FText::FromString(TEXT("First Blood: Land your first kill")));
	}
	if (TotalKills >= 100)
	{
		AchievementComponent->UnlockAchievement(TEXT("centurion"), FText::FromString(TEXT("Centurion: Land 100 kills")));
	}
}

void ASOCharacter::HandleWaypointDiscovered(ASOWaypoint* /*Waypoint*/)
{
	if (!WaypointComponent || !AchievementComponent)
	{
		return;
	}

	if (WaypointComponent->GetDiscoveredWaypoints().Num() >= 5)
	{
		AchievementComponent->UnlockAchievement(TEXT("explorer"), FText::FromString(TEXT("Explorer: Discover 5 waypoints")));
	}
}

// ---------------------------------------------------------------------------
// Talent Respec
// ---------------------------------------------------------------------------

bool ASOCharacter::RespecTalents()
{
	if (!TalentComponent || TalentComponent->GetUnlockedNodes().Num() == 0)
	{
		return false;
	}
	if (RespecGoldCost > 0 && Gold < RespecGoldCost)
	{
		return false;
	}

	if (!TalentComponent->RespecAll())
	{
		return false;
	}

	if (RespecGoldCost > 0)
	{
		AddGold(-RespecGoldCost);
	}

	if (RespecSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, RespecSFX, GetActorLocation());
	}

	return true;
}

// ---------------------------------------------------------------------------
// Vendor transaction toast
// ---------------------------------------------------------------------------

void ASOCharacter::ShowTransactionToast(const FString& Message, float Duration)
{
	TransactionToastText = Message;
	if (UWorld* World = GetWorld())
	{
		TransactionToastExpireTime = World->GetTimeSeconds() + Duration;
	}
}

bool ASOCharacter::IsTransactionToastActive() const
{
	const UWorld* World = GetWorld();
	return World && World->GetTimeSeconds() < TransactionToastExpireTime;
}

// ---------------------------------------------------------------------------
// Legendary/Set drop toast
// ---------------------------------------------------------------------------

void ASOCharacter::ShowLegendaryDropToast(const FString& Message, float Duration)
{
	LegendaryDropToastText = Message;
	if (UWorld* World = GetWorld())
	{
		LegendaryDropToastExpireTime = World->GetTimeSeconds() + Duration;
	}
}

bool ASOCharacter::IsLegendaryDropToastActive() const
{
	const UWorld* World = GetWorld();
	return World && World->GetTimeSeconds() < LegendaryDropToastExpireTime;
}

// ---------------------------------------------------------------------------
// Vassals
// ---------------------------------------------------------------------------

void ASOCharacter::SummonVassal()
{
	if (VassalComponent)
	{
		VassalComponent->SummonSelectedVassal();
	}
}

void ASOCharacter::DismissVassal()
{
	if (VassalComponent)
	{
		VassalComponent->DismissVassal();
	}
}

void ASOCharacter::CycleVassal()
{
	if (VassalComponent)
	{
		VassalComponent->CycleVassalSelection();
	}
}
