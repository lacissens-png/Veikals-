#include "SOBossCharacter.h"

#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "SODamageType.h"
#include "SOHealthComponent.h"
#include "TimerManager.h"

ASOBossCharacter::ASOBossCharacter()
{
	// Bosses are chunkier by default. Designers can override.
	MovementSpeed   = 300.0f;
	SightRadius     = 2400.0f;
	LoseSightRadius = 3200.0f;
	AttackRange     = 240.0f;
	AttackDamage    = 18.0f;
	AttackCooldown  = 2.2f;
	CorpseLifetime  = 8.0f;
	XPReward        = 250;
}

void ASOBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	BaseAttackDamage  = AttackDamage;
	BaseMovementSpeed = MovementSpeed;

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &ASOBossCharacter::HandleHealthChanged);

		// Bosses start with far more HP than a grunt. Simple 5x baseline until designers override in a BP.
		if (HealthComponent->MaxHealth < 400.0f)
		{
			HealthComponent->MaxHealth = 500.0f;
			HealthComponent->Revive(HealthComponent->MaxHealth);
		}
	}

	CurrentPhase = ESOBossPhase::Phase1;

	if (EnrageTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(EnrageTimerHandle, this, &ASOBossCharacter::TriggerEnrage, EnrageTime, false);
	}
}

void ASOBossCharacter::HandleHealthChanged(USOHealthComponent* Comp, float /*Old*/, float NewHealth, float /*Delta*/,
                                           AController* /*InstigatedBy*/, AActor* /*DamageCauser*/)
{
	if (!Comp || Comp->IsDead())
	{
		return;
	}

	const float Pct = Comp->GetHealthPercent();

	if (CurrentPhase == ESOBossPhase::Phase1 && Pct <= 0.66f)
	{
		EnterPhase(ESOBossPhase::Phase2);
	}
	else if (CurrentPhase == ESOBossPhase::Phase2 && Pct <= 0.33f)
	{
		EnterPhase(ESOBossPhase::Phase3);
	}
}

void ASOBossCharacter::EnterPhase(ESOBossPhase NewPhase)
{
	if (NewPhase == CurrentPhase)
	{
		return;
	}

	const ESOBossPhase OldPhase = CurrentPhase;
	CurrentPhase = NewPhase;

	// Rescale from base values so re-entering a phase doesn't compound.
	switch (NewPhase)
	{
	case ESOBossPhase::Phase1:
		AttackDamage  = BaseAttackDamage;
		MovementSpeed = BaseMovementSpeed;
		break;
	case ESOBossPhase::Phase2:
		AttackDamage  = BaseAttackDamage  * Phase2DamageMult;
		MovementSpeed = BaseMovementSpeed * Phase2SpeedMult;
		break;
	case ESOBossPhase::Phase3:
		AttackDamage  = BaseAttackDamage  * Phase3DamageMult;
		MovementSpeed = BaseMovementSpeed * Phase3SpeedMult;
		AoECadence    = Phase3AoECadence;
		break;
	}

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = MovementSpeed;
	}

	OnBossPhaseChanged.Broadcast(OldPhase, NewPhase);
	OnPhaseEnteredBP(NewPhase);
}

bool ASOBossCharacter::PerformAttack(AActor* Target)
{
	if (!Target || !IsAlive())
	{
		return false;
	}

	// Every (AoECadence+1)-th swing is an AoE. AoECadence = 2 → hit, hit, AoE, repeat.
	const int32 Cadence = FMath::Max(0, AoECadence);
	if (SwingsSinceLastAoE > Cadence)
	{
		SwingsSinceLastAoE = 0;
		PerformTelegraphedAoE(Target);
		return true;
	}

	++SwingsSinceLastAoE;
	return Super::PerformAttack(Target);
}

void ASOBossCharacter::PerformTelegraphedAoE(AActor* Target)
{
	if (!Target)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Center = Target->GetActorLocation();
	OnTelegraphAoEStart(Center, TelegraphedAoERadius, TelegraphWindup);

	if (bDrawTelegraphDebug)
	{
		DrawDebugCircle(World, Center, TelegraphedAoERadius, 32, FColor::Red,
		                false, TelegraphWindup, 0, 4.0f, FVector(1, 0, 0), FVector(0, 1, 0), false);
	}

	TWeakObjectPtr<AActor> TargetPtr(Target);

	FTimerHandle Handle;
	World->GetTimerManager().SetTimer(
		Handle,
		FTimerDelegate::CreateUObject(this, &ASOBossCharacter::TelegraphedAoEResolve, TargetPtr, Center),
		TelegraphWindup,
		false);
}

void ASOBossCharacter::TelegraphedAoEResolve(TWeakObjectPtr<AActor> /*Target*/, FVector Center)
{
	if (!IsAlive())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TSubclassOf<UDamageType> DTClass = AoEDamageType
		? AoEDamageType
		: TSubclassOf<UDamageType>(USODamageType::StaticClass());

	FCollisionQueryParams Params(SCENE_QUERY_STAT(SOBossAoE), /*bTraceComplex=*/ false, /*Ignore=*/ this);

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByChannel(
		Overlaps,
		Center,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(TelegraphedAoERadius),
		Params);

	int32 HitCount = 0;
	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* HitActor = Result.GetActor();
		if (!HitActor || HitActor == this)
		{
			continue;
		}
		// Only damage actors carrying a live health component (other bosses/enemies excluded, since they're on our team).
		USOHealthComponent* HitHealth = HitActor->FindComponentByClass<USOHealthComponent>();
		if (!HitHealth || HitHealth->IsDead())
		{
			continue;
		}
		if (HitActor->IsA(ASOEnemyCharacter::StaticClass()))
		{
			continue;
		}

		UGameplayStatics::ApplyDamage(HitActor, TelegraphedAoEDamage, GetController(), this, DTClass);
		++HitCount;
	}

	if (bDrawTelegraphDebug)
	{
		DrawDebugSphere(World, Center, TelegraphedAoERadius, 24, FColor(255, 80, 20), false, 0.5f, 0, 2.0f);
	}

	OnTelegraphAoEResolve(Center, TelegraphedAoERadius, HitCount);
}

void ASOBossCharacter::TriggerEnrage()
{
	if (bEnraged || !IsAlive())
	{
		return;
	}
	bEnraged = true;

	// Enrage means the fight is meant to be ending soon — force the hardest phase first
	// (EnterPhase always rescales from Base* values, so doing this before the multiply
	// below keeps the two effects additive instead of one clobbering the other).
	if (CurrentPhase != ESOBossPhase::Phase3)
	{
		EnterPhase(ESOBossPhase::Phase3);
	}

	AttackDamage  *= EnrageDamageMult;
	MovementSpeed *= EnrageSpeedMult;

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = MovementSpeed;
	}

	if (EnrageSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EnrageSFX, GetActorLocation());
	}

	OnBossEnraged();
}

float ASOBossCharacter::GetEnrageTimeRemaining() const
{
	if (bEnraged)
	{
		return 0.0f;
	}
	if (const UWorld* World = GetWorld())
	{
		return FMath::Max(0.0f, World->GetTimerManager().GetTimerRemaining(EnrageTimerHandle));
	}
	return 0.0f;
}
