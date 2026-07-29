#include "SOVassalActor.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "SODamageType.h"
#include "SOEnemyCharacter.h"
#include "SOHealthComponent.h"
#include "SOVassalComponent.h"
#include "SOVassalData.h"

ASOVassalActor::ASOVassalActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;

	HealthComponent = CreateDefaultSubobject<USOHealthComponent>(TEXT("HealthComponent"));

	bUseControllerRotationYaw = false;
	AutoPossessAI             = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->bOrientRotationToMovement = true;
		Move->MaxWalkSpeed              = 420.0f;
	}
}

void ASOVassalActor::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnDeath.AddDynamic(this, &ASOVassalActor::HandleDeath);
}

void ASOVassalActor::InitializeFromData(USOVassalData* Data)
{
	if (!Data)
	{
		return;
	}

	SourceData     = Data;
	AttackDamage   = Data->AttackDamage;
	AttackRange    = Data->AttackRange;
	AttackCooldown = Data->AttackCooldown;

	if (HealthComponent)
	{
		HealthComponent->MaxHealth = Data->MaxHealth;
		HealthComponent->Revive(Data->MaxHealth);
	}
}

void ASOVassalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsAlive())
	{
		return;
	}

	if (AttackCooldownRemaining > 0.0f)
	{
		AttackCooldownRemaining -= DeltaTime;
	}

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
		FollowOwner();
		return;
	}

	const float DistSq = FVector::DistSquared(GetActorLocation(), CurrentTarget->GetActorLocation());

	if (DistSq <= AttackRange * AttackRange)
	{
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
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentTarget.Get());
	}
}

bool ASOVassalActor::IsAlive() const
{
	return HealthComponent && !HealthComponent->IsDead();
}

AActor* ASOVassalActor::FindBestTarget() const
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, ASOEnemyCharacter::StaticClass(), Enemies);

	AActor* Best         = nullptr;
	float   BestDistSq   = AggroRange * AggroRange;
	const FVector MyLoc  = GetActorLocation();

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

void ASOVassalActor::FollowOwner()
{
	if (FollowOwnerDistance <= 0.0f || !OwnerVassalComponent.IsValid())
	{
		return;
	}

	AActor* Owner = OwnerVassalComponent->GetOwner();
	if (!Owner)
	{
		return;
	}

	const float DistSq = FVector::DistSquared(GetActorLocation(), Owner->GetActorLocation());
	if (DistSq > FollowOwnerDistance * FollowOwnerDistance)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), Owner);
	}
}

void ASOVassalActor::HandleDeath(USOHealthComponent* /*OwningComponent*/, AController* /*InstigatedBy*/, AActor* /*DamageCauser*/)
{
	SetActorEnableCollision(false);

	if (AController* Controller = GetController())
	{
		Controller->UnPossess();
	}

	if (DeathSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSFX, GetActorLocation());
	}

	OnVassalDied.Broadcast(this);

	if (USOVassalComponent* Owner = OwnerVassalComponent.Get())
	{
		Owner->HandleVassalDied(this);
	}

	SetLifeSpan(3.0f);
}
