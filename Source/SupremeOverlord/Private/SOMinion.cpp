#include "SOMinion.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SODamageType.h"
#include "SOEnemyCharacter.h"
#include "SOHealthComponent.h"
#include "SOStatusEffectComponent.h"
#include "SOSummonComponent.h"

ASOMinion::ASOMinion()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;

	HealthComponent       = CreateDefaultSubobject<USOHealthComponent>(TEXT("HealthComponent"));
	StatusEffectComponent = CreateDefaultSubobject<USOStatusEffectComponent>(TEXT("StatusEffectComponent"));

	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->bOrientRotationToMovement = true;
		Move->MaxWalkSpeed              = 380.0f;
	}
}

void ASOMinion::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnDeath.AddDynamic(this, &ASOMinion::HandleDeath);

	if (LifetimeDuration > 0.0f)
	{
		LifetimeRemaining = LifetimeDuration;
	}
}

void ASOMinion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsAlive())
	{
		return;
	}

	// Tick optional lifetime
	if (LifetimeDuration > 0.0f)
	{
		LifetimeRemaining -= DeltaTime;
		if (LifetimeRemaining <= 0.0f)
		{
			HandleDeath(HealthComponent, nullptr, nullptr);
			Destroy();
			return;
		}
	}

	// Tick attack cooldown
	if (AttackCooldownRemaining > 0.0f)
	{
		AttackCooldownRemaining -= DeltaTime;
	}

	// Revalidate current target
	if (CurrentTarget.IsValid())
	{
		ASOEnemyCharacter* Enemy = Cast<ASOEnemyCharacter>(CurrentTarget.Get());
		if (!Enemy || !Enemy->IsAlive())
		{
			CurrentTarget = nullptr;
		}
	}

	if (!CurrentTarget.IsValid())
	{
		CurrentTarget = FindBestTarget();
	}

	if (!CurrentTarget.IsValid())
	{
		return;
	}

	const float DistSq = FVector::DistSquared(GetActorLocation(), CurrentTarget->GetActorLocation());

	if (DistSq <= AttackRange * AttackRange)
	{
		// In melee range — attack if off cooldown
		if (AttackCooldownRemaining <= 0.0f)
		{
			ASOEnemyCharacter* TargetEnemy = Cast<ASOEnemyCharacter>(CurrentTarget.Get());
			const bool bWasAlive = TargetEnemy && TargetEnemy->IsAlive();

			UGameplayStatics::ApplyDamage(
				CurrentTarget.Get(),
				AttackDamage,
				GetController(),
				this,
				USODamageType::StaticClass());

			if (AttackSFX)
			{
				UGameplayStatics::PlaySoundAtLocation(this, AttackSFX, GetActorLocation());
			}

			// This swing was the killing blow — credit it toward the next tier.
			if (bWasAlive && !TargetEnemy->IsAlive())
			{
				NotifyKill();
			}

			AttackCooldownRemaining = AttackCooldown;
		}
	}
	else
	{
		// Chase the target
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentTarget.Get());
	}
}

bool ASOMinion::IsAlive() const
{
	return HealthComponent && !HealthComponent->IsDead();
}

AActor* ASOMinion::FindBestTarget() const
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, ASOEnemyCharacter::StaticClass(), Enemies);

	AActor* Best           = nullptr;
	float   BestDistSq     = AggroRange * AggroRange;
	const FVector MyLoc    = GetActorLocation();

	for (AActor* A : Enemies)
	{
		ASOEnemyCharacter* E = Cast<ASOEnemyCharacter>(A);
		if (!E || !E->IsAlive())
		{
			continue;
		}
		const float DistSq = FVector::DistSquared(MyLoc, E->GetActorLocation());
		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			Best       = E;
		}
	}
	return Best;
}

int32 ASOMinion::GetKillsUntilEvolve() const
{
	if (CurrentTier == ESOMinionTier::Champion)
	{
		return 0;
	}
	const int32 NextThreshold = KillsToEvolve * (static_cast<int32>(CurrentTier) + 1);
	return FMath::Max(0, NextThreshold - KillCount);
}

void ASOMinion::NotifyKill()
{
	++KillCount;

	if (CurrentTier != ESOMinionTier::Champion && GetKillsUntilEvolve() <= 0)
	{
		Evolve();
	}
}

void ASOMinion::Evolve()
{
	CurrentTier = static_cast<ESOMinionTier>(static_cast<uint8>(CurrentTier) + 1);

	AttackDamage *= TierStatMultiplier;

	if (HealthComponent)
	{
		HealthComponent->MaxHealth *= TierStatMultiplier;
		// Evolving is a reward — top the minion back up at its new ceiling.
		HealthComponent->Heal(HealthComponent->MaxHealth, GetController(), this);
	}

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed *= TierSpeedMultiplier;
	}

	SetActorScale3D(GetActorScale3D() * TierScaleMultiplier);

	if (EvolveSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EvolveSFX, GetActorLocation());
	}

	OnMinionEvolved.Broadcast(this, CurrentTier);
	OnMinionEvolvedBP(CurrentTier);
}

void ASOMinion::HandleDeath(USOHealthComponent*, AController*, AActor*)
{
	if (DeathSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSFX, GetActorLocation());
	}

	if (OwnerSummonComponent.IsValid())
	{
		OwnerSummonComponent->OnMinionDied.Broadcast(this);
	}

	// Disable collision so the corpse doesn't block movement, then auto-destroy
	SetActorEnableCollision(false);
	if (AController* C = GetController())
	{
		C->UnPossess();
	}
	SetLifeSpan(3.0f);
}
