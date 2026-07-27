#include "SOAttributesComponent.h"

#include "SOCharacter.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"

USOAttributesComponent::USOAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	Strength      = FMath::Max(0, StartingStrength);
	Intellect     = FMath::Max(0, StartingIntellect);
	Vitality      = FMath::Max(0, StartingVitality);
	UnspentPoints = 0;

	// Broadcast initial values so any UI wired up in BeginPlay picks them up.
	OnAttributeChanged.Broadcast(this, ESOAttribute::Strength,  Strength,  Strength);
	OnAttributeChanged.Broadcast(this, ESOAttribute::Intellect, Intellect, Intellect);
	OnAttributeChanged.Broadcast(this, ESOAttribute::Vitality,  Vitality,  Vitality);
	OnUnspentPointsChanged.Broadcast(this, UnspentPoints);

	// Apply starting attribute effects to the owning character's stat pools.
	ApplyAttributeChange(ESOAttribute::Strength,  Strength);
	ApplyAttributeChange(ESOAttribute::Intellect, Intellect);
	ApplyAttributeChange(ESOAttribute::Vitality,  Vitality);
}

int32 USOAttributesComponent::GetAttribute(ESOAttribute Attribute) const
{
	switch (Attribute)
	{
	case ESOAttribute::Strength:  return Strength;
	case ESOAttribute::Intellect: return Intellect;
	case ESOAttribute::Vitality:  return Vitality;
	}
	return 0;
}

void USOAttributesComponent::GrantPoints(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}
	UnspentPoints += Amount;
	OnUnspentPointsChanged.Broadcast(this, UnspentPoints);
}

bool USOAttributesComponent::AllocatePoint(ESOAttribute Attribute)
{
	if (UnspentPoints <= 0)
	{
		return false;
	}

	switch (Attribute)
	{
	case ESOAttribute::Strength:  ++Strength;  break;
	case ESOAttribute::Intellect: ++Intellect; break;
	case ESOAttribute::Vitality:  ++Vitality;  break;
	default: return false;
	}

	--UnspentPoints;

	const int32 NewValue = GetAttribute(Attribute);
	OnAttributeChanged.Broadcast(this, Attribute, NewValue, 1);
	OnUnspentPointsChanged.Broadcast(this, UnspentPoints);

	ApplyAttributeChange(Attribute, 1);
	return true;
}

void USOAttributesComponent::ApplyAttributeChange(ESOAttribute Attribute, int32 Delta)
{
	if (Delta == 0)
	{
		return;
	}

	ASOCharacter* Owner = Cast<ASOCharacter>(GetOwner());
	if (!Owner)
	{
		return;
	}

	switch (Attribute)
	{
	case ESOAttribute::Strength:
		Owner->PrimaryAttackDamage += PrimaryDamagePerStrength * Delta;
		break;

	case ESOAttribute::Intellect:
		Owner->ShadowBoltBaseDamage += ShadowBoltDamagePerIntellect * Delta;
		if (USOManaComponent* Mana = Owner->ManaComponent)
		{
			Mana->MaxMana += MaxManaPerIntellect * Delta;
			if (Delta > 0)
			{
				Mana->Restore(MaxManaPerIntellect * Delta);
			}
		}
		break;

	case ESOAttribute::Vitality:
		if (USOHealthComponent* Health = Owner->HealthComponent)
		{
			Health->MaxHealth += MaxHealthPerVitality * Delta;
			if (Delta > 0)
			{
				Health->Heal(MaxHealthPerVitality * Delta);
			}
		}
		break;
	}
}
