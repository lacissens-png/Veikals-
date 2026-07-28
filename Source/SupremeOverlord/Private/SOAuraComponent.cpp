#include "SOAuraComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "SODamageType.h"
#include "SOEnemyCharacter.h"
#include "SOStatusEffectComponent.h"
#include "TimerManager.h"

USOAuraComponent::USOAuraComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOAuraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PulseInterval > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			PulseHandle,
			this, &USOAuraComponent::Pulse,
			PulseInterval,
			/*bLoop=*/ true);
	}
}

void USOAuraComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PulseHandle);
	}
	Super::EndPlay(Reason);
}

int32 USOAuraComponent::GetEnemiesInRange(TArray<ASOEnemyCharacter*>& OutEnemies) const
{
	OutEnemies.Reset();

	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return 0;
	}

	const FVector  Center   = Owner->GetActorLocation();
	const float    RadiusSq = AuraRadius * AuraRadius;

	TArray<AActor*> All;
	UGameplayStatics::GetAllActorsOfClass(Owner, ASOEnemyCharacter::StaticClass(), All);

	for (AActor* A : All)
	{
		ASOEnemyCharacter* Enemy = Cast<ASOEnemyCharacter>(A);
		if (!Enemy || !Enemy->IsAlive())
		{
			continue;
		}
		if (FVector::DistSquared(Center, Enemy->GetActorLocation()) <= RadiusSq)
		{
			OutEnemies.Add(Enemy);
		}
	}
	return OutEnemies.Num();
}

void USOAuraComponent::Pulse()
{
	if (!bAuraActive)
	{
		return;
	}

	TArray<ASOEnemyCharacter*> Enemies;
	GetEnemiesInRange(Enemies);

	AController* InstigatorCtrl = nullptr;
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		InstigatorCtrl = OwnerPawn->GetController();
	}

	for (ASOEnemyCharacter* Enemy : Enemies)
	{
		if (Enemy->StatusEffectComponent)
		{
			// SlowDuration > PulseInterval keeps enemies slowed continuously while in range.
			Enemy->StatusEffectComponent->ApplyEffect(
				ESOStatusEffectType::Slowed,
				SlowDuration,
				0.0f);
		}

		if (DamagePerPulse > 0.0f)
		{
			UGameplayStatics::ApplyDamage(
				Enemy,
				DamagePerPulse,
				InstigatorCtrl,
				GetOwner(),
				USODamageType::StaticClass());
		}
	}

	OnAuraPulse.Broadcast(Enemies.Num());
	OnAuraPulseBP(Enemies.Num());
}
