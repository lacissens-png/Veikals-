#include "SOCursedGround.h"

#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "SOCharacter.h"
#include "SODamageType.h"
#include "SOEnemyCharacter.h"
#include "SOStatusEffectComponent.h"
#include "TimerManager.h"

ASOCursedGround::ASOCursedGround()
{
	PrimaryActorTick.bCanEverTick = false;

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetSphereRadius(AreaRadius);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionObjectType(ECC_WorldDynamic);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetRootComponent(AreaSphere);
}

void ASOCursedGround::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->SetSphereRadius(AreaRadius);

	SetLifeSpan(Duration);

	GetWorld()->GetTimerManager().SetTimer(
		PulseHandle,
		this, &ASOCursedGround::ApplyGroundEffects,
		TickInterval,
		/*bLoop=*/ true,
		/*InFirstDelay=*/ 0.0f);
}

void ASOCursedGround::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PulseHandle);
	}
	Super::EndPlay(Reason);
}

float ASOCursedGround::GetRemainingDuration() const
{
	return GetLifeSpan();
}

void ASOCursedGround::ApplyGroundEffects()
{
	TArray<AActor*> Overlapping;
	AreaSphere->GetOverlappingActors(Overlapping, ASOEnemyCharacter::StaticClass());

	AController* InstigatorCtrl = nullptr;
	AActor*      DamageCauser   = this;
	if (ASOCharacter* Owner = OwnerCharacter.Get())
	{
		InstigatorCtrl = Owner->GetController();
		DamageCauser   = Owner;
	}

	int32 Affected    = 0;
	float TotalDamage = 0.0f;

	for (AActor* A : Overlapping)
	{
		ASOEnemyCharacter* Enemy = Cast<ASOEnemyCharacter>(A);
		if (!Enemy || !Enemy->IsAlive())
		{
			continue;
		}

		if (DamagePerTick > 0.0f)
		{
			UGameplayStatics::ApplyDamage(Enemy, DamagePerTick, InstigatorCtrl, DamageCauser,
			                              USODamageType::StaticClass());
			TotalDamage += DamagePerTick;
		}

		if (Enemy->StatusEffectComponent)
		{
			if (bAppliesSlow)
			{
				Enemy->StatusEffectComponent->ApplyEffect(ESOStatusEffectType::Slowed, SlowDuration, 0.0f);
			}
			if (bAppliesCurse)
			{
				Enemy->StatusEffectComponent->ApplyEffect(ESOStatusEffectType::Cursed, CurseDuration, 0.0f);
			}
		}

		++Affected;
	}

	OnCursedGroundTick.Broadcast(Affected, TotalDamage);
	OnCursedGroundTickBP(Affected, TotalDamage);
}
