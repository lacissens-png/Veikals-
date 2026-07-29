#include "SOTrap.h"

#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SOCharacter.h"
#include "SODamageType.h"
#include "SOEnemyCharacter.h"
#include "SONecroticDamageType.h"
#include "SOShadowDamageType.h"
#include "SOStatusEffectComponent.h"
#include "TimerManager.h"

ASOTrap::ASOTrap()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetSphereRadius(TriggerRadius);
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TriggerSphere->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetRootComponent(TriggerSphere);

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ASOTrap::OnSphereBeginOverlap);
}

void ASOTrap::BeginPlay()
{
	Super::BeginPlay();

	// Update the sphere radius in case it was changed in the editor.
	TriggerSphere->SetSphereRadius(TriggerRadius);

	if (ArmDelay > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ArmHandle,
			this, &ASOTrap::Arm,
			ArmDelay,
			/*bLoop=*/ false);
	}
	else
	{
		Arm();
	}
}

void ASOTrap::Arm()
{
	bArmed = true;
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASOTrap::OnSphereBeginOverlap(UPrimitiveComponent* /*OverlappedComponent*/,
                                    AActor* OtherActor,
                                    UPrimitiveComponent* /*OtherComp*/,
                                    int32 /*OtherBodyIndex*/,
                                    bool /*bFromSweep*/,
                                    const FHitResult& /*SweepResult*/)
{
	if (!bArmed || bTriggered)
	{
		return;
	}

	ASOEnemyCharacter* Enemy = Cast<ASOEnemyCharacter>(OtherActor);
	if (!Enemy || !Enemy->IsAlive())
	{
		return;
	}

	bTriggered = true;

	// Disable collision immediately so additional enemies don't double-trigger.
	TriggerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AController* InstigatorCtrl = nullptr;
	if (OwnerCharacter.IsValid())
	{
		InstigatorCtrl = OwnerCharacter->GetController();
	}

	switch (TrapType)
	{
	case ESOTrapType::ShadowSnare:
		if (Damage > 0.0f)
		{
			UGameplayStatics::ApplyDamage(Enemy, Damage, InstigatorCtrl, this, USOShadowDamageType::StaticClass());
		}
		if (Enemy->StatusEffectComponent)
		{
			Enemy->StatusEffectComponent->ApplyEffect(ESOStatusEffectType::Slowed, SlowDuration, 0.0f);
		}
		break;

	case ESOTrapType::ArcaneMine:
		if (Damage > 0.0f)
		{
			// No Arcane/Lightning entry exists in ESODamageCategory, so this
			// stays on the Physical-default base type rather than inventing a
			// school for it.
			UGameplayStatics::ApplyDamage(Enemy, Damage, InstigatorCtrl, this, USODamageType::StaticClass());
		}
		if (Enemy->StatusEffectComponent)
		{
			Enemy->StatusEffectComponent->ApplyEffect(ESOStatusEffectType::Shocked, ShockedDuration, 0.0f);
		}
		break;

	case ESOTrapType::NecroticSpore:
		if (Damage > 0.0f)
		{
			UGameplayStatics::ApplyDamage(Enemy, Damage, InstigatorCtrl, this, USONecroticDamageType::StaticClass());
		}
		if (Enemy->StatusEffectComponent)
		{
			Enemy->StatusEffectComponent->ApplyEffect(
				ESOStatusEffectType::Poisoned,
				PoisonDuration,
				PoisonDamagePerTick,
				/*TickInterval=*/ 1.0f,
				/*MaxStacks=*/ 3);
		}
		break;
	}

	SetLifeSpan(DestroyDelay);
}
