#include "SOCorpseExplosionComponent.h"

#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "SOCharacter.h"
#include "SODamageType.h"
#include "SOEnemyCharacter.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"

USOCorpseExplosionComponent::USOCorpseExplosionComponent()
{
	PrimaryComponentTick.bCanEverTick          = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval          = 0.05f;
}

void USOCorpseExplosionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining = FMath::Max(0.0f, CooldownRemaining - DeltaTime);
	}
}

bool USOCorpseExplosionComponent::CanCast() const
{
	return CooldownRemaining <= 0.0f;
}

bool USOCorpseExplosionComponent::Cast(FVector CursorLocation, ASOCharacter* Caster)
{
	if (!CanCast() || !Caster || !Caster->IsAlive())
	{
		return false;
	}

	if (ManaCost > 0.0f && (!Caster->ManaComponent || !Caster->ManaComponent->HasEnough(ManaCost)))
	{
		return false;
	}

	TArray<AActor*> All;
	UGameplayStatics::GetAllActorsOfClass(this, ASOEnemyCharacter::StaticClass(), All);

	// Nearest corpse to the cursor.
	ASOEnemyCharacter* Corpse       = nullptr;
	float              BestDistSq   = SearchRange * SearchRange;

	for (AActor* A : All)
	{
		ASOEnemyCharacter* Enemy = ::Cast<ASOEnemyCharacter>(A);
		if (!Enemy || Enemy->IsAlive())
		{
			continue;
		}

		const float DistSq = FVector::DistSquared(CursorLocation, Enemy->GetActorLocation());
		if (DistSq <= BestDistSq)
		{
			BestDistSq = DistSq;
			Corpse     = Enemy;
		}
	}

	if (!Corpse)
	{
		return false;
	}

	if (ManaCost > 0.0f && !Caster->ManaComponent->Consume(ManaCost))
	{
		return false;
	}

	const FVector Epicenter = Corpse->GetActorLocation();
	const float   RadiusSq  = ExplosionRadius * ExplosionRadius;

	AController* InstigatorCtrl = Caster->GetController();

	int32 EnemiesHit  = 0;
	float TotalDamage = 0.0f;

	for (AActor* A : All)
	{
		ASOEnemyCharacter* Enemy = ::Cast<ASOEnemyCharacter>(A);
		if (!Enemy || Enemy == Corpse || !Enemy->IsAlive() || !Enemy->HealthComponent)
		{
			continue;
		}
		if (FVector::DistSquared(Epicenter, Enemy->GetActorLocation()) > RadiusSq)
		{
			continue;
		}

		const float Damage = Enemy->HealthComponent->MaxHealth * HPFraction;
		UGameplayStatics::ApplyDamage(Enemy, Damage, InstigatorCtrl, Caster, USODamageType::StaticClass());

		++EnemiesHit;
		TotalDamage += Damage;
	}

	Corpse->Destroy();

	CooldownRemaining = Cooldown;

	OnCorpseExploded.Broadcast(EnemiesHit, TotalDamage);
	OnCorpseExplodedBP(EnemiesHit, TotalDamage);

	return true;
}
