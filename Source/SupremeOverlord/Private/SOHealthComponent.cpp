#include "SOHealthComponent.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "SOStatusEffectComponent.h"
#include "SOVassalComponent.h"

USOHealthComponent::USOHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void USOHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// StartingHealth == 0 means "start at full" — the common case.
	CurrentHealth = (StartingHealth > 0.0f) ? FMath::Min(StartingHealth, MaxHealth) : MaxHealth;
	bIsDead       = (CurrentHealth <= 0.0f);

	if (AActor* Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &USOHealthComponent::HandleAnyDamage);
	}
}

void USOHealthComponent::HandleAnyDamage(AActor* /*DamagedActor*/, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead || bInvulnerable || Damage <= 0.0f)
	{
		return;
	}

	const USODamageType* SODamageType   = Cast<USODamageType>(DamageType);
	const bool           bIgnoreResists = SODamageType && SODamageType->bIgnoresResistances;

	float ScaledDamage = bIgnoreResists ? Damage : (Damage * IncomingDamageMultiplier);

	if (SODamageType && !bIgnoreResists && SODamageType->Category != ESODamageCategory::TrueDamage)
	{
		ScaledDamage *= FMath::Max(0.0f, 1.0f - GetResistance(SODamageType->Category));
	}

	// Shocked (defender) / Cursed (attacker) are combat-state multipliers, not
	// elemental resistances — they apply on top regardless of damage category.
	if (const USOStatusEffectComponent* DefenderStatus = GetOwner() ? GetOwner()->FindComponentByClass<USOStatusEffectComponent>() : nullptr)
	{
		ScaledDamage *= DefenderStatus->GetIncomingDamageMultiplier();
	}

	if (const USOVassalComponent* DefenderVassals = GetOwner() ? GetOwner()->FindComponentByClass<USOVassalComponent>() : nullptr)
	{
		ScaledDamage *= DefenderVassals->GetIncomingDamageMultiplier();
	}

	APawn* InstigatorPawn      = InstigatedBy ? InstigatedBy->GetPawn() : nullptr;
	AActor* AttackerCandidate  = DamageCauser ? DamageCauser : static_cast<AActor*>(InstigatorPawn);
	if (const USOStatusEffectComponent* AttackerStatus = AttackerCandidate ? AttackerCandidate->FindComponentByClass<USOStatusEffectComponent>() : nullptr)
	{
		ScaledDamage *= AttackerStatus->GetOutgoingDamageMultiplier();
	}

	if (ScaledDamage <= 0.0f)
	{
		return;
	}

	if (SODamageType)
	{
		LastHitReactStrength = SODamageType->HitReactStrength;
	}

	ApplyHealthDelta(-ScaledDamage, InstigatedBy, DamageCauser);
}

float USOHealthComponent::GetResistance(ESODamageCategory Category) const
{
	if (const float* Found = ElementalResistances.Find(Category))
	{
		return FMath::Clamp(*Found, 0.0f, 0.9f);
	}
	return 0.0f;
}

void USOHealthComponent::SetResistance(ESODamageCategory Category, float Value)
{
	ElementalResistances.Add(Category, FMath::Clamp(Value, 0.0f, 0.9f));
}

float USOHealthComponent::Heal(float HealAmount, AController* Instigator, AActor* Healer)
{
	if (bIsDead || HealAmount <= 0.0f)
	{
		return 0.0f;
	}

	const float OldHealth = CurrentHealth;
	ApplyHealthDelta(HealAmount, Instigator, Healer);
	return CurrentHealth - OldHealth;
}

void USOHealthComponent::Kill(AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead)
	{
		return;
	}

	// Bypass invulnerability by hand-writing the transition.
	ApplyHealthDelta(-CurrentHealth, InstigatedBy, DamageCauser);
}

void USOHealthComponent::Revive(float ReviveHealth)
{
	const float OldHealth = CurrentHealth;
	CurrentHealth = (ReviveHealth < 0.0f) ? MaxHealth : FMath::Clamp(ReviveHealth, 0.0f, MaxHealth);
	bIsDead       = (CurrentHealth <= 0.0f);

	OnHealthChanged.Broadcast(this, OldHealth, CurrentHealth, CurrentHealth - OldHealth, nullptr, nullptr);
}

void USOHealthComponent::ApplyHealthDelta(float Delta, AController* InstigatedBy, AActor* DamageCauser)
{
	const float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(OldHealth + Delta, 0.0f, MaxHealth);

	const float ActualDelta = CurrentHealth - OldHealth;
	if (FMath::IsNearlyZero(ActualDelta))
	{
		return;
	}

	OnHealthChanged.Broadcast(this, OldHealth, CurrentHealth, ActualDelta, InstigatedBy, DamageCauser);

	if (!bIsDead && CurrentHealth <= 0.0f)
	{
		bIsDead = true;
		OnDeath.Broadcast(this, InstigatedBy, DamageCauser);
	}
}
