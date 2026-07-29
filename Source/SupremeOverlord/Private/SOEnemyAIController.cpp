#include "SOEnemyAIController.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "SOCharacter.h"
#include "SOEnemyCharacter.h"
#include "SOHealthComponent.h"
#include "TimerManager.h"

ASOEnemyAIController::ASOEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	bAttachToPawn                 = true;
}

void ASOEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CurrentState      = ESOEnemyAIState::Idle;
	bAttackOnCooldown = false;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(ThinkTimerHandle, this, &ASOEnemyAIController::ThinkTick, ThinkInterval, true);
	}
}

void ASOEnemyAIController::OnUnPossess()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ThinkTimerHandle);
		World->GetTimerManager().ClearTimer(AttackCooldownHandle);
	}

	Super::OnUnPossess();
}

void ASOEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

APawn* ASOEnemyAIController::ResolveTargetPawn() const
{
	// One player for now; extend later with an aggro/threat table.
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		return nullptr;
	}

	if (const ASOCharacter* AsHero = Cast<ASOCharacter>(PlayerPawn))
	{
		if (!AsHero->IsAlive())
		{
			return nullptr;
		}
	}
	return PlayerPawn;
}

void ASOEnemyAIController::EnterState(ESOEnemyAIState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	CurrentState = NewState;

	// Leaving Chasing/Attacking must clear any active path so the pawn actually stops.
	if (NewState == ESOEnemyAIState::Idle || NewState == ESOEnemyAIState::Attacking || NewState == ESOEnemyAIState::Dead)
	{
		StopMovement();
	}
}

void ASOEnemyAIController::ThinkTick()
{
	ASOEnemyCharacter* EnemyOwner = Cast<ASOEnemyCharacter>(GetPawn());
	if (!EnemyOwner)
	{
		return;
	}

	if (!EnemyOwner->IsAlive())
	{
		EnterState(ESOEnemyAIState::Dead);
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(ThinkTimerHandle);
		}
		return;
	}

	APawn* Target = ResolveTargetPawn();
	if (!Target)
	{
		EnterState(ESOEnemyAIState::Idle);
		return;
	}

	const float DistSq   = FVector::DistSquared(EnemyOwner->GetActorLocation(), Target->GetActorLocation());
	const float SightSq  = FMath::Square(EnemyOwner->SightRadius);
	const float LoseSq   = FMath::Square(EnemyOwner->LoseSightRadius);
	const float AttackSq = FMath::Square(EnemyOwner->AttackRange);

	if (EnemyOwner->bFleeFromPlayer)
	{
		if (DistSq <= SightSq)
		{
			EnterState(ESOEnemyAIState::Fleeing);

			const FVector AwayDir = (EnemyOwner->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal2D();
			const FVector FleeDir = AwayDir.IsNearlyZero() ? EnemyOwner->GetActorForwardVector() : AwayDir;
			const FVector FleeDestination = EnemyOwner->GetActorLocation() + FleeDir * EnemyOwner->FleeDistance;

			MoveToLocation(FleeDestination, /*AcceptanceRadius=*/ 50.0f, /*bStopOnOverlap=*/ false, /*bUsePathfinding=*/ true);
		}
		else
		{
			EnterState(ESOEnemyAIState::Idle);
		}
		return;
	}

	if (bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), EnemyOwner->GetActorLocation(), EnemyOwner->AttackRange, 16, FColor::Red,   false, ThinkInterval * 1.1f, 0, 1.5f);
		DrawDebugSphere(GetWorld(), EnemyOwner->GetActorLocation(), EnemyOwner->SightRadius, 24, FColor::Yellow, false, ThinkInterval * 1.1f, 0, 1.0f);
		DrawDebugLine  (GetWorld(), EnemyOwner->GetActorLocation(), Target->GetActorLocation(), FColor::Cyan, false, ThinkInterval * 1.1f, 0, 1.0f);
	}

	switch (CurrentState)
	{
	case ESOEnemyAIState::Idle:
		if (DistSq <= SightSq)
		{
			EnterState(ESOEnemyAIState::Chasing);
		}
		break;

	case ESOEnemyAIState::Chasing:
		if (DistSq > LoseSq)
		{
			EnterState(ESOEnemyAIState::Idle);
			break;
		}
		if (DistSq <= AttackSq)
		{
			EnterState(ESOEnemyAIState::Attacking);
			DoAttack(EnemyOwner, Target);
			break;
		}
		MoveToActor(Target, /*AcceptanceRadius=*/ EnemyOwner->AttackRange * 0.9f, /*bStopOnOverlap=*/ true, /*bUsePathfinding=*/ true);
		break;

	case ESOEnemyAIState::Attacking:
		if (DistSq > AttackSq)
		{
			EnterState(ESOEnemyAIState::Chasing);
			break;
		}
		// Face the target while in range so the swing looks correct once we add animations.
		if (APawn* MyPawn = GetPawn())
		{
			const FVector ToTarget = (Target->GetActorLocation() - MyPawn->GetActorLocation()).GetSafeNormal2D();
			if (!ToTarget.IsNearlyZero())
			{
				const FRotator DesiredRot = ToTarget.Rotation();
				MyPawn->SetActorRotation(FRotator(0.0f, DesiredRot.Yaw, 0.0f));
			}
		}
		DoAttack(EnemyOwner, Target);
		break;

	case ESOEnemyAIState::Dead:
	default:
		break;
	}
}

void ASOEnemyAIController::DoAttack(ASOEnemyCharacter* EnemyOwner, APawn* Target)
{
	if (bAttackOnCooldown || !EnemyOwner || !Target)
	{
		return;
	}

	if (EnemyOwner->PerformAttack(Target))
	{
		bAttackOnCooldown = true;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(
				AttackCooldownHandle,
				FTimerDelegate::CreateLambda([this]() { bAttackOnCooldown = false; }),
				EnemyOwner->AttackCooldown,
				false);
		}
	}
}
