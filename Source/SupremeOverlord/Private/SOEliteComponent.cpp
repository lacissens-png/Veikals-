#include "SOEliteComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SOEnemyCharacter.h"
#include "SOHealthComponent.h"

USOEliteComponent::USOEliteComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOEliteComponent::OnRegister()
{
	Super::OnRegister();

	// Register runs before BeginPlay - a great spot to seed the enemy's health/mana
	// so the max pool reflects our modifiers from the very first frame.
	if (bRandomizeOnBeginPlay)
	{
		RollRandomAffixes();
	}
	ApplyAffixesToOwner();
}

void USOEliteComponent::RollRandomAffixes()
{
	static const TArray<ESOEliteAffix> AllAffixes = {
		ESOEliteAffix::Fortified,
		ESOEliteAffix::Vicious,
		ESOEliteAffix::Swift,
		ESOEliteAffix::Enriched,
		ESOEliteAffix::FarSighted
	};

	TArray<ESOEliteAffix> Pool = AllAffixes;
	const int32 ToPick = FMath::Clamp(RollCount, 1, Pool.Num());

	int32 NewMask = 0;
	for (int32 i = 0; i < ToPick; ++i)
	{
		const int32 Idx = FMath::RandRange(0, Pool.Num() - 1);
		NewMask |= static_cast<int32>(Pool[Idx]);
		Pool.RemoveAt(Idx);
	}
	AffixMask = NewMask;
}

void USOEliteComponent::ApplyAffixesToOwner()
{
	ASOEnemyCharacter* Enemy = Cast<ASOEnemyCharacter>(GetOwner());
	if (!Enemy || AffixMask == 0)
	{
		return;
	}

	if (HasAffix(ESOEliteAffix::Fortified))
	{
		if (USOHealthComponent* HP = Enemy->HealthComponent)
		{
			HP->MaxHealth *= FortifiedHealthMult;
		}
	}
	if (HasAffix(ESOEliteAffix::Vicious))
	{
		Enemy->AttackDamage *= ViciousDamageMult;
	}
	if (HasAffix(ESOEliteAffix::Swift))
	{
		Enemy->MovementSpeed *= SwiftSpeedMult;
		if (UCharacterMovementComponent* Move = Enemy->GetCharacterMovement())
		{
			Move->MaxWalkSpeed = Enemy->MovementSpeed;
		}
	}
	if (HasAffix(ESOEliteAffix::Enriched))
	{
		Enemy->ItemDropChance = FMath::Min(1.0f, Enemy->ItemDropChance * EnrichedLootMult);
		for (FSOLootDrop& Drop : Enemy->LootTable)
		{
			Drop.MinCount = FMath::CeilToInt(Drop.MinCount * EnrichedLootMult);
			Drop.MaxCount = FMath::CeilToInt(Drop.MaxCount * EnrichedLootMult);
		}
		// Bigger XP bounty too - elites feel meaningful when they pop.
		Enemy->XPReward = FMath::RoundToInt(Enemy->XPReward * EnrichedLootMult);
	}
	if (HasAffix(ESOEliteAffix::FarSighted))
	{
		Enemy->SightRadius     *= FarSightedMult;
		Enemy->LoseSightRadius *= FarSightedMult;
	}

	OnAffixesApplied.Broadcast(AffixMask);
}
