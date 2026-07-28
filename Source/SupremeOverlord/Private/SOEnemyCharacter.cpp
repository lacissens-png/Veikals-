#include "SOEnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "SOCharacter.h"
#include "SODamageType.h"
#include "SODifficultySubsystem.h"
#include "SOCorruptionComponent.h"
#include "SOEnemyAIController.h"
#include "SOExperienceComponent.h"
#include "SOQuestComponent.h"
#include "SOHealthComponent.h"
#include "SOStatusEffectComponent.h"
#include "SOItemData.h"
#include "SOItemPickup.h"
#include "SOLootRoller.h"
#include "SOPickupOrb.h"

ASOEnemyCharacter::ASOEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	HealthComponent         = CreateDefaultSubobject<USOHealthComponent>(TEXT("HealthComponent"));
	StatusEffectComponent   = CreateDefaultSubobject<USOStatusEffectComponent>(TEXT("StatusEffectComponent"));

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate              = FRotator(0.0f, 640.0f, 0.0f);
		Movement->MaxWalkSpeed              = MovementSpeed;
		Movement->bConstrainToPlane         = true;
		Movement->bSnapToPlaneAtStart       = true;
	}

	// Ensure the AI controller runs as soon as the enemy is placed/spawned.
	AIControllerClass    = ASOEnemyAIController::StaticClass();
	AutoPossessAI        = EAutoPossessAI::PlacedInWorldOrSpawned;
	AutoPossessPlayer    = EAutoReceiveInput::Disabled;

	// Sensible default so the enemy blocks the player but pawns don't push through each other.
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionProfileName(TEXT("Pawn"));
	}
}

void ASOEnemyCharacter::BeginPlay()
{
	// Applied before Super::BeginPlay() so USOHealthComponent's own BeginPlay
	// (triggered inside it) snaps CurrentHealth to the already-scaled MaxHealth.
	if (const USODifficultySubsystem* Difficulty = USODifficultySubsystem::Get(GetWorld()))
	{
		if (HealthComponent)
		{
			HealthComponent->MaxHealth *= Difficulty->GetEnemyHealthMultiplier();
		}
		AttackDamage   *= Difficulty->GetEnemyDamageMultiplier();
		XPReward        = FMath::RoundToInt(XPReward * Difficulty->GetXPMultiplier());
		ItemDropChance  = FMath::Min(1.0f, ItemDropChance * Difficulty->GetItemDropChanceMultiplier());
	}

	Super::BeginPlay();

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = MovementSpeed * (bFleeFromPlayer ? FleeSpeedMultiplier : 1.0f);
	}

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ASOEnemyCharacter::HandleDeath);
	}
}

bool ASOEnemyCharacter::IsAlive() const
{
	return HealthComponent ? HealthComponent->IsAlive() : true;
}

bool ASOEnemyCharacter::PerformAttack(AActor* Target)
{
	if (!Target || !IsAlive() || AttackDamage <= 0.0f)
	{
		return false;
	}

	TSubclassOf<UDamageType> DTClass = AttackDamageType ? AttackDamageType : TSubclassOf<UDamageType>(USODamageType::StaticClass());

	const float Applied = UGameplayStatics::ApplyDamage(
		Target,
		AttackDamage,
		GetController(),
		this,
		DTClass);

	if (AttackSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackSFX, GetActorLocation());
	}

	return Applied > 0.0f;
}

void ASOEnemyCharacter::HandleDeath(USOHealthComponent* /*OwningComponent*/, AController* InstigatedBy, AActor* /*DamageCauser*/)
{
	if (AController* MyController = GetController())
	{
		MyController->StopMovement();
		MyController->UnPossess();
	}

	// Award XP and advance kill quests for the killing player.
	if (InstigatedBy)
	{
		if (ASOCharacter* Killer = Cast<ASOCharacter>(InstigatedBy->GetPawn()))
		{
			if (XPReward > 0)
			{
				if (USOExperienceComponent* XP = Killer->ExperienceComponent)
				{
					XP->GainXP(XPReward);
				}
			}

			if (Killer->QuestComponent)
			{
				Killer->QuestComponent->NotifyEnemyKilled(this);
			}

			if (CorruptionValue > 0.0f && Killer->CorruptionComponent)
			{
				Killer->CorruptionComponent->AddCorruption(CorruptionValue);
			}
		}
	}

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->DisableMovement();
		Movement->StopMovementImmediately();
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	DropLoot();
	RollItemDrop();

	if (DeathSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSFX, GetActorLocation());
	}

	if (CorpseLifetime > 0.0f)
	{
		SetLifeSpan(CorpseLifetime);
	}
}

void ASOEnemyCharacter::DropLoot()
{
	UWorld* World = GetWorld();
	if (!World || LootTable.Num() == 0)
	{
		return;
	}

	const FVector CorpseLocation = GetActorLocation();
	const USODifficultySubsystem* Difficulty = USODifficultySubsystem::Get(World);
	const int32 Rolls = FMath::Max(1, LootRollCount);

	for (int32 RollIndex = 0; RollIndex < Rolls; ++RollIndex)
	{
		for (const FSOLootDrop& Drop : LootTable)
		{
			if (!Drop.OrbClass || Drop.DropChance <= 0.0f)
			{
				continue;
			}

			if (FMath::FRand() > Drop.DropChance)
			{
				continue;
			}

			const int32 SafeMin = FMath::Max(1, Drop.MinCount);
			const int32 SafeMax = FMath::Max(SafeMin, Drop.MaxCount);
			const int32 Count   = FMath::RandRange(SafeMin, SafeMax);

			for (int32 i = 0; i < Count; ++i)
			{
				// Uniformly sample a disk of radius LootSpreadRadius.
				const float Angle    = FMath::FRandRange(0.0f, 2.0f * PI);
				const float DiskR    = LootSpreadRadius * FMath::Sqrt(FMath::FRand());
				const FVector Offset = FVector(FMath::Cos(Angle) * DiskR, FMath::Sin(Angle) * DiskR, LootSpawnHeight);
				const FVector SpawnLoc = CorpseLocation + Offset;

				FActorSpawnParameters Params;
				Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				Params.Owner = this;

				if (ASOPickupOrb* Orb = World->SpawnActor<ASOPickupOrb>(Drop.OrbClass, SpawnLoc, FRotator::ZeroRotator, Params))
				{
					if (Difficulty && Orb->OrbType == ESOOrbType::Gold)
					{
						Orb->Amount *= Difficulty->GetGoldMultiplier();
					}
				}
			}
		}
	}
}

void ASOEnemyCharacter::RollItemDrop()
{
	UWorld* World = GetWorld();
	if (!World || !ItemPickupClass || ItemDropPool.Num() == 0
		|| (!bGuaranteedItemDrop && ItemDropChance <= 0.0f))
	{
		return;
	}

	if (!bGuaranteedItemDrop && FMath::FRand() > ItemDropChance)
	{
		return;
	}

	// Weighted-random pick from the pool. Rows with null Item or non-positive weight are skipped.
	float TotalWeight = 0.0f;
	for (const FSOItemDrop& Entry : ItemDropPool)
	{
		if (Entry.Item && Entry.Weight > 0.0f)
		{
			TotalWeight += Entry.Weight;
		}
	}
	if (TotalWeight <= 0.0f)
	{
		return;
	}

	float Roll = FMath::FRandRange(0.0f, TotalWeight);
	USOItemData* PickedItem = nullptr;
	for (const FSOItemDrop& Entry : ItemDropPool)
	{
		if (!Entry.Item || Entry.Weight <= 0.0f)
		{
			continue;
		}
		Roll -= Entry.Weight;
		if (Roll <= 0.0f)
		{
			PickedItem = Entry.Item;
			break;
		}
	}
	if (!PickedItem)
	{
		// Numerical fallback — pick the first eligible row.
		for (const FSOItemDrop& Entry : ItemDropPool)
		{
			if (Entry.Item && Entry.Weight > 0.0f)
			{
				PickedItem = Entry.Item;
				break;
			}
		}
	}

	if (!PickedItem)
	{
		return;
	}

	const FVector CorpseLocation = GetActorLocation();
	const float   Angle          = FMath::FRandRange(0.0f, 2.0f * PI);
	const float   DiskR          = LootSpreadRadius * FMath::Sqrt(FMath::FRand());
	const FVector Offset         = FVector(FMath::Cos(Angle) * DiskR, FMath::Sin(Angle) * DiskR, LootSpawnHeight);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Params.Owner = this;

	if (ASOItemPickup* Pickup = World->SpawnActor<ASOItemPickup>(ItemPickupClass, CorpseLocation + Offset, FRotator::ZeroRotator, Params))
	{
		// Roll a distinct, rarity-affixed instance so drops of the same template still vary.
		USOItemData* RolledItem = USOLootRoller::RollItemInstance(PickedItem, GetTransientPackage(), PickedItem->ItemLevel);
		Pickup->SetItem(RolledItem ? RolledItem : PickedItem);
	}
}
