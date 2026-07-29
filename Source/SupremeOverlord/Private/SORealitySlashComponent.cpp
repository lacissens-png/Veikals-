#include "SORealitySlashComponent.h"

#include "Kismet/GameplayStatics.h"
#include "SOBossCharacter.h"
#include "SOCharacter.h"
#include "SODamageType.h"
#include "SOEliteComponent.h"
#include "SOEnemyCharacter.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"
#include "SORealitySlashDamageType.h"

USORealitySlashComponent::USORealitySlashComponent()
{
	PrimaryComponentTick.bCanEverTick          = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval          = 0.05f;
}

void USORealitySlashComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining = FMath::Max(0.0f, CooldownRemaining - DeltaTime);
	}
}

bool USORealitySlashComponent::CanCast() const
{
	return CooldownRemaining <= 0.0f;
}

bool USORealitySlashComponent::Cast(FVector CursorLocation, ASOCharacter* Caster)
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

	ASOEnemyCharacter* Target  = nullptr;
	float              BestDistSq = Range * Range;

	for (AActor* A : All)
	{
		ASOEnemyCharacter* Enemy = ::Cast<ASOEnemyCharacter>(A);
		if (!Enemy || !Enemy->IsAlive())
		{
			continue;
		}
		const float DistSq = FVector::DistSquared(CursorLocation, Enemy->GetActorLocation());
		if (DistSq <= BestDistSq)
		{
			BestDistSq = DistSq;
			Target     = Enemy;
		}
	}

	if (!Target)
	{
		return false;
	}

	if (ManaCost > 0.0f)
	{
		Caster->ManaComponent->Consume(ManaCost);
	}

	AController* InstigatorCtrl = Caster->GetController();

	const bool bIsBoss  = Target->IsA(ASOBossCharacter::StaticClass());
	const USOEliteComponent* Elite = Target->FindComponentByClass<USOEliteComponent>();
	const bool bResists = bIsBoss || (Elite && Elite->IsElite());

	bool bInstantKill = false;

	if (bResists)
	{
		TSubclassOf<UDamageType> DTClass = FallbackDamageType
			? FallbackDamageType
			: TSubclassOf<UDamageType>(USORealitySlashDamageType::StaticClass());

		// The instant-kill branch below can't meaningfully "crit" - only the
		// boss/elite fallback-damage path is a real crit candidate.
		const bool  bCrit  = Caster->RollCriticalHit();
		const float Damage = BossFallbackDamage * (bCrit ? Caster->GetCritDamageMultiplier() : 1.0f);
		UGameplayStatics::ApplyDamage(Target, Damage, InstigatorCtrl, Caster, DTClass);

		if (bCrit)
		{
			Caster->OnCriticalHit.Broadcast(Target, Damage);
		}
	}
	else if (Target->HealthComponent)
	{
		Target->HealthComponent->Kill(InstigatorCtrl, Caster);
		bInstantKill = true;
	}

	CooldownRemaining = Cooldown;

	OnRealitySlash.Broadcast(Target, bInstantKill);
	OnRealitySlashBP(Target, bInstantKill);

	return true;
}
