#include "SOStatusEffectComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SOHealthComponent.h"

USOStatusEffectComponent::USOStatusEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f; // 20 Hz is fine for status ticking
}

void USOStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	bool bMovementDirty = false;
	TArray<ESOStatusEffectType> ToRemove;

	for (FSOStatusEffect& FX : ActiveEffects)
	{
		FX.RemainingDuration -= DeltaTime;

		if (FX.RemainingDuration <= 0.0f)
		{
			ToRemove.Add(FX.Type);
			bMovementDirty = true;
			continue;
		}

		// Should this tick deal damage / healing?
		if (FX.DamagePerTick == 0.0f || FX.TickInterval <= 0.0f)
		{
			continue;
		}

		FX.TimeSinceLastTick += DeltaTime;
		if (FX.TimeSinceLastTick < FX.TickInterval)
		{
			continue;
		}
		FX.TimeSinceLastTick -= FX.TickInterval;

		const float TickDamage = FX.DamagePerTick * FX.CurrentStacks;

		if (TickDamage > 0.0f)
		{
			// Positive = damage
			UGameplayStatics::ApplyDamage(Owner, TickDamage, nullptr, nullptr, UDamageType::StaticClass());
		}
		else if (TickDamage < 0.0f)
		{
			// Negative = heal via HealthComponent
			if (USOHealthComponent* HC = Owner->FindComponentByClass<USOHealthComponent>())
			{
				HC->Heal(-TickDamage);
			}
		}

		OnStatusEffectTicked.Broadcast(FX.Type, TickDamage);
		OnEffectTickBP(FX.Type, TickDamage);
	}

	for (ESOStatusEffectType Type : ToRemove)
	{
		ActiveEffects.RemoveAll([Type](const FSOStatusEffect& FX) { return FX.Type == Type; });
		OnStatusEffectRemoved.Broadcast(Type);
		OnEffectRemovedBP(Type);
	}

	if (bMovementDirty)
	{
		ApplyMovementModifiers();
	}
}

void USOStatusEffectComponent::ApplyEffect(ESOStatusEffectType Type,
                                            float Duration,
                                            float DamagePerTick,
                                            float TickInterval,
                                            int32 MaxStacks)
{
	FSOStatusEffect* Existing = FindEffect(Type);
	if (Existing)
	{
		// Refresh duration; add a stack if below the cap
		Existing->RemainingDuration = FMath::Max(Existing->RemainingDuration, Duration);
		if (Existing->CurrentStacks < Existing->MaxStacks)
		{
			Existing->CurrentStacks++;
		}
		OnStatusEffectApplied.Broadcast(Type, Existing->CurrentStacks);
		OnEffectAppliedBP(Type, Existing->CurrentStacks);
	}
	else
	{
		FSOStatusEffect NewFX;
		NewFX.Type              = Type;
		NewFX.DamagePerTick     = DamagePerTick;
		NewFX.TickInterval      = FMath::Max(TickInterval, 0.1f);
		NewFX.RemainingDuration = Duration;
		NewFX.CurrentStacks     = 1;
		NewFX.MaxStacks         = FMath::Max(MaxStacks, 1);
		ActiveEffects.Add(NewFX);

		OnStatusEffectApplied.Broadcast(Type, 1);
		OnEffectAppliedBP(Type, 1);
	}

	if (Type == ESOStatusEffectType::Frozen || Type == ESOStatusEffectType::Slowed)
	{
		ApplyMovementModifiers();
	}
}

void USOStatusEffectComponent::RemoveEffect(ESOStatusEffectType Type)
{
	const int32 Removed = ActiveEffects.RemoveAll(
		[Type](const FSOStatusEffect& FX) { return FX.Type == Type; });

	if (Removed > 0)
	{
		OnStatusEffectRemoved.Broadcast(Type);
		OnEffectRemovedBP(Type);
		if (Type == ESOStatusEffectType::Frozen || Type == ESOStatusEffectType::Slowed)
		{
			ApplyMovementModifiers();
		}
	}
}

void USOStatusEffectComponent::ClearAllEffects()
{
	TArray<ESOStatusEffectType> Types;
	for (const FSOStatusEffect& FX : ActiveEffects)
	{
		Types.Add(FX.Type);
	}
	ActiveEffects.Empty();
	for (ESOStatusEffectType T : Types)
	{
		OnStatusEffectRemoved.Broadcast(T);
		OnEffectRemovedBP(T);
	}
	ApplyMovementModifiers();
}

bool USOStatusEffectComponent::HasEffect(ESOStatusEffectType Type) const
{
	for (const FSOStatusEffect& FX : ActiveEffects)
	{
		if (FX.Type == Type) return true;
	}
	return false;
}

float USOStatusEffectComponent::GetEffectRemainingTime(ESOStatusEffectType Type) const
{
	for (const FSOStatusEffect& FX : ActiveEffects)
	{
		if (FX.Type == Type) return FX.RemainingDuration;
	}
	return 0.0f;
}

int32 USOStatusEffectComponent::GetEffectStacks(ESOStatusEffectType Type) const
{
	for (const FSOStatusEffect& FX : ActiveEffects)
	{
		if (FX.Type == Type) return FX.CurrentStacks;
	}
	return 0;
}

float USOStatusEffectComponent::GetIncomingDamageMultiplier() const
{
	return HasEffect(ESOStatusEffectType::Shocked) ? ShockedDamageMultiplier : 1.0f;
}

float USOStatusEffectComponent::GetOutgoingDamageMultiplier() const
{
	return HasEffect(ESOStatusEffectType::Cursed) ? CursedDamageMultiplier : 1.0f;
}

// ---------------------------------------------------------------------------
// Private
// ---------------------------------------------------------------------------
FSOStatusEffect* USOStatusEffectComponent::FindEffect(ESOStatusEffectType Type)
{
	for (FSOStatusEffect& FX : ActiveEffects)
	{
		if (FX.Type == Type) return &FX;
	}
	return nullptr;
}

void USOStatusEffectComponent::ApplyMovementModifiers()
{
	ACharacter* Char = Cast<ACharacter>(GetOwner());
	if (!Char)
	{
		return;
	}
	UCharacterMovementComponent* Move = Char->GetCharacterMovement();
	if (!Move)
	{
		return;
	}

	const bool bDebuffed = HasEffect(ESOStatusEffectType::Frozen) || HasEffect(ESOStatusEffectType::Slowed);

	if (bDebuffed)
	{
		// Snapshot the current (un-debuffed) speed only on the transition into
		// Frozen/Slowed — capturing it here (rather than once at BeginPlay)
		// means any equipment/talent/elite speed change picked up since the
		// last debuff still restores correctly when this one wears off.
		if (!bMovementModifierActive)
		{
			BaseWalkSpeed           = Move->MaxWalkSpeed;
			bMovementModifierActive = true;
		}

		Move->MaxWalkSpeed = HasEffect(ESOStatusEffectType::Frozen)
			? 0.0f
			: BaseWalkSpeed * (1.0f - SlowFraction);
	}
	else if (bMovementModifierActive)
	{
		Move->MaxWalkSpeed      = BaseWalkSpeed;
		bMovementModifierActive = false;
	}
}
