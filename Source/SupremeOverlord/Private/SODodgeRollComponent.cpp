#include "SODodgeRollComponent.h"

#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SOCharacter.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"

USODodgeRollComponent::USODodgeRollComponent()
{
	PrimaryComponentTick.bCanEverTick          = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void USODodgeRollComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CooldownRemaining > 0.0f)
	{
		CooldownRemaining = FMath::Max(0.0f, CooldownRemaining - DeltaTime);
	}

	if (!bIsRolling)
	{
		return;
	}

	ASOCharacter* Caster = RollingCaster.Get();
	if (!Caster)
	{
		EndRoll();
		return;
	}

	RollTimeRemaining -= DeltaTime;
	if (RollTimeRemaining <= 0.0f)
	{
		EndRoll();
		return;
	}

	if (UCharacterMovementComponent* Move = Caster->GetCharacterMovement())
	{
		Move->Velocity = RollDirectionCached * RollSpeed;
	}
}

void USODodgeRollComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	if (bIsRolling)
	{
		EndRoll();
	}
	Super::EndPlay(Reason);
}

bool USODodgeRollComponent::CanRoll() const
{
	return CooldownRemaining <= 0.0f && !bIsRolling;
}

bool USODodgeRollComponent::Roll(FVector CursorLocation, ASOCharacter* Caster)
{
	if (!CanRoll() || !Caster || !Caster->IsAlive())
	{
		return false;
	}

	if (ManaCost > 0.0f && (!Caster->ManaComponent || !Caster->ManaComponent->HasEnough(ManaCost)))
	{
		return false;
	}

	FVector Velocity = Caster->GetVelocity();
	Velocity.Z = 0.0f;

	FVector Direction;
	if (!Velocity.IsNearlyZero())
	{
		Direction = Velocity.GetSafeNormal();
	}
	else
	{
		FVector Offset = CursorLocation - Caster->GetActorLocation();
		Offset.Z = 0.0f;
		Direction = Offset.IsNearlyZero() ? Caster->GetActorForwardVector() : Offset.GetSafeNormal();
	}

	if (ManaCost > 0.0f)
	{
		Caster->ManaComponent->Consume(ManaCost);
	}

	RollingCaster        = Caster;
	RollDirectionCached  = Direction;
	RollSpeed            = RollDistance / RollDuration;
	RollTimeRemaining    = RollDuration;
	bIsRolling           = true;
	CooldownRemaining    = Cooldown;

	if (Caster->HealthComponent)
	{
		Caster->HealthComponent->bInvulnerable = true;
	}

	const FVector FromLocation = Caster->GetActorLocation();
	OnDodgeRoll.Broadcast(FromLocation, Direction);
	OnDodgeRollBP(FromLocation, Direction);

	return true;
}

void USODodgeRollComponent::EndRoll()
{
	bIsRolling = false;

	if (ASOCharacter* Caster = RollingCaster.Get())
	{
		if (Caster->HealthComponent)
		{
			Caster->HealthComponent->bInvulnerable = false;
		}
	}

	RollingCaster.Reset();
}
