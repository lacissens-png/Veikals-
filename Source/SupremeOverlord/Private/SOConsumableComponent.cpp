#include "SOConsumableComponent.h"

#include "Kismet/GameplayStatics.h"
#include "SOCharacter.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"

USOConsumableComponent::USOConsumableComponent()
{
	PrimaryComponentTick.bCanEverTick          = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval          = 0.05f;
}

void USOConsumableComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentCharges = MaxCharges;
}

void USOConsumableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining = FMath::Max(0.0f, CooldownRemaining - DeltaTime);
	}
}

bool USOConsumableComponent::UsePotion(ASOCharacter* Caster)
{
	if (!CanUse() || !Caster || !Caster->IsAlive())
	{
		return false;
	}

	float HealedAmount   = 0.0f;
	float ManaRestored   = 0.0f;

	if (Caster->HealthComponent && HealFraction > 0.0f)
	{
		HealedAmount = Caster->HealthComponent->Heal(Caster->HealthComponent->MaxHealth * HealFraction);
	}
	if (Caster->ManaComponent && ManaFraction > 0.0f)
	{
		ManaRestored = Caster->ManaComponent->Restore(Caster->ManaComponent->MaxMana * ManaFraction);
	}

	--CurrentCharges;
	CooldownRemaining = UseCooldown;

	if (PotionUseSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PotionUseSFX, Caster->GetActorLocation());
	}

	OnPotionUsed.Broadcast(HealedAmount, ManaRestored);
	OnPotionUsedBP(HealedAmount, ManaRestored);

	return true;
}

int32 USOConsumableComponent::RefillCharges()
{
	const int32 Restored = FMath::Max(0, MaxCharges - CurrentCharges);
	CurrentCharges = MaxCharges;
	return Restored;
}
