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
