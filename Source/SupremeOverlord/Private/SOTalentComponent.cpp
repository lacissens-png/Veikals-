#include "SOTalentComponent.h"

#include "SOCharacter.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"
#include "SOTalentNode.h"
#include "GameFramework/CharacterMovementComponent.h"

USOTalentComponent::USOTalentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOTalentComponent::BeginPlay()
{
	Super::BeginPlay();

	AvailableTalentPoints = FMath::Max(0, StartingTalentPoints);
	if (AvailableTalentPoints > 0)
	{
		OnTalentPointsChanged.Broadcast(AvailableTalentPoints, AvailableTalentPoints);
	}
}

bool USOTalentComponent::IsUnlocked(USOTalentNode* Node) const
{
	return Node && UnlockedNodes.Contains(Node);
}

bool USOTalentComponent::CanUnlock(USOTalentNode* Node, FString& OutReason) const
{
	if (!Node)
	{
		OutReason = TEXT("No node supplied.");
		return false;
	}
	if (IsUnlocked(Node))
	{
		OutReason = TEXT("Already unlocked.");
		return false;
	}
	if (AvailableTalentPoints < Node->PointCost)
	{
		OutReason = FString::Printf(TEXT("Needs %d point(s), have %d."), Node->PointCost, AvailableTalentPoints);
		return false;
	}
	for (USOTalentNode* Prereq : Node->Prerequisites)
	{
		if (!IsUnlocked(Prereq))
		{
			OutReason = FString::Printf(TEXT("Prerequisite '%s' not unlocked."),
			                            Prereq ? *Prereq->DisplayName.ToString() : TEXT("(null)"));
			return false;
		}
	}
	OutReason.Reset();
	return true;
}

void USOTalentComponent::GrantPoints(int32 Amount)
{
	if (Amount == 0)
	{
		return;
	}
	AvailableTalentPoints = FMath::Max(0, AvailableTalentPoints + Amount);
	OnTalentPointsChanged.Broadcast(AvailableTalentPoints, Amount);
}

bool USOTalentComponent::UnlockNode(USOTalentNode* Node)
{
	FString Reason;
	if (!CanUnlock(Node, Reason))
	{
		return false;
	}

	AvailableTalentPoints -= Node->PointCost;
	UnlockedNodes.Add(Node);
	ApplyNodeEffects(Node);

	OnTalentPointsChanged.Broadcast(AvailableTalentPoints, -Node->PointCost);
	OnTalentUnlocked.Broadcast(Node);
	return true;
}

bool USOTalentComponent::RespecAll()
{
	if (UnlockedNodes.Num() == 0)
	{
		return false;
	}

	int32 RefundedPoints = 0;
	for (USOTalentNode* Node : UnlockedNodes)
	{
		if (!Node)
		{
			continue;
		}
		RevertNodeEffects(Node);
		RefundedPoints += Node->PointCost;
	}

	UnlockedNodes.Reset();
	AvailableTalentPoints += RefundedPoints;

	OnTalentPointsChanged.Broadcast(AvailableTalentPoints, RefundedPoints);
	return true;
}

void USOTalentComponent::RestoreFromSave(const TArray<USOTalentNode*>& Nodes, int32 Points)
{
	// Deliberately does NOT call ApplyNodeEffects/RespecAll: every stat a node
	// affects (MaxHealth, MaxMana, PrimaryAttackDamage, ShadowBoltBaseDamage,
	// MovementSpeed, ManaRegen) is already restored to its final, bonus-
	// inclusive value elsewhere in ASOCharacter::LoadGameFromSlotName — replaying
	// the per-node effects here would double-count them on top of that total.
	UnlockedNodes.Reset();
	for (USOTalentNode* Node : Nodes)
	{
		if (Node && !UnlockedNodes.Contains(Node))
		{
			UnlockedNodes.Add(Node);
			OnTalentUnlocked.Broadcast(Node);
		}
	}

	AvailableTalentPoints = FMath::Max(0, Points);
	OnTalentPointsChanged.Broadcast(AvailableTalentPoints, 0);
}

void USOTalentComponent::ApplyNodeEffects(USOTalentNode* Node)
{
	if (!Node)
	{
		return;
	}
	ASOCharacter* Owner = Cast<ASOCharacter>(GetOwner());
	if (!Owner)
	{
		return;
	}

	for (const FSOTalentEffect& Effect : Node->Effects)
	{
		switch (Effect.EffectType)
		{
		case ESOTalentEffect::FlatMaxHealth:
			if (USOHealthComponent* HP = Owner->HealthComponent)
			{
				HP->MaxHealth += Effect.Magnitude;
				if (Effect.Magnitude > 0.0f)
				{
					HP->Heal(Effect.Magnitude);
				}
			}
			break;

		case ESOTalentEffect::FlatMaxMana:
			if (USOManaComponent* MP = Owner->ManaComponent)
			{
				MP->MaxMana += Effect.Magnitude;
				if (Effect.Magnitude > 0.0f)
				{
					MP->Restore(Effect.Magnitude);
				}
			}
			break;

		case ESOTalentEffect::FlatPrimaryDamage:
			Owner->PrimaryAttackDamage += Effect.Magnitude;
			break;

		case ESOTalentEffect::FlatShadowBoltDamage:
			Owner->ShadowBoltBaseDamage += Effect.Magnitude;
			break;

		case ESOTalentEffect::FlatLifeDrainHealFrac:
			Owner->LifeDrainHealFraction = FMath::Clamp(Owner->LifeDrainHealFraction + Effect.Magnitude, 0.0f, 5.0f);
			break;

		case ESOTalentEffect::FlatManaRegen:
			if (USOManaComponent* MP = Owner->ManaComponent)
			{
				MP->RegenPerSecond = FMath::Max(0.0f, MP->RegenPerSecond + Effect.Magnitude);
			}
			break;

		case ESOTalentEffect::MultMovementSpeed:
			Owner->MovementSpeed *= (1.0f + Effect.Magnitude);
			if (UCharacterMovementComponent* Move = Owner->GetCharacterMovement())
			{
				Move->MaxWalkSpeed = Owner->MovementSpeed;
			}
			break;

		case ESOTalentEffect::None:
		default:
			break;
		}
	}
}

void USOTalentComponent::RevertNodeEffects(USOTalentNode* Node)
{
	if (!Node)
	{
		return;
	}
	ASOCharacter* Owner = Cast<ASOCharacter>(GetOwner());
	if (!Owner)
	{
		return;
	}

	// Mirrors ApplyNodeEffects with every operation inverted.
	for (const FSOTalentEffect& Effect : Node->Effects)
	{
		switch (Effect.EffectType)
		{
		case ESOTalentEffect::FlatMaxHealth:
			if (USOHealthComponent* HP = Owner->HealthComponent)
			{
				HP->MaxHealth = FMath::Max(1.0f, HP->MaxHealth - Effect.Magnitude);
				// A respec dropping MaxHealth below the character's current
				// health would otherwise leave a lingering overheal shield.
				HP->ClampCurrentHealthToMax();
			}
			break;

		case ESOTalentEffect::FlatMaxMana:
			if (USOManaComponent* MP = Owner->ManaComponent)
			{
				MP->MaxMana = FMath::Max(0.0f, MP->MaxMana - Effect.Magnitude);
				MP->ClampCurrentManaToMax();
			}
			break;

		case ESOTalentEffect::FlatPrimaryDamage:
			Owner->PrimaryAttackDamage = FMath::Max(0.0f, Owner->PrimaryAttackDamage - Effect.Magnitude);
			break;

		case ESOTalentEffect::FlatShadowBoltDamage:
			Owner->ShadowBoltBaseDamage = FMath::Max(0.0f, Owner->ShadowBoltBaseDamage - Effect.Magnitude);
			break;

		case ESOTalentEffect::FlatLifeDrainHealFrac:
			Owner->LifeDrainHealFraction = FMath::Clamp(Owner->LifeDrainHealFraction - Effect.Magnitude, 0.0f, 5.0f);
			break;

		case ESOTalentEffect::FlatManaRegen:
			if (USOManaComponent* MP = Owner->ManaComponent)
			{
				MP->RegenPerSecond = FMath::Max(0.0f, MP->RegenPerSecond - Effect.Magnitude);
			}
			break;

		case ESOTalentEffect::MultMovementSpeed:
			Owner->MovementSpeed /= (1.0f + Effect.Magnitude);
			if (UCharacterMovementComponent* Move = Owner->GetCharacterMovement())
			{
				Move->MaxWalkSpeed = Owner->MovementSpeed;
			}
			break;

		case ESOTalentEffect::None:
		default:
			break;
		}
	}
}
