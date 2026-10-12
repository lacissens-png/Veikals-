// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "EclipseDamageExecution.generated.h"

/**
 * Armor/penetration step for all GAS damage. Reads SetByCaller Data.Damage and
 * Data.ArmorPenetration, captures the target's Armor, and outputs IncomingDamage
 * using Eclipse::ComputeFinalDamage - the exact formula the swarm path uses.
 */
UCLASS()
class ECLIPSEGAME_API UEclipseDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UEclipseDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
