#pragma once

#include "CoreMinimal.h"
#include "SODamageType.h"
#include "SORealitySlashDamageType.generated.h"

/**
 * Damage type for the Reality Slash fallback hit (see USORealitySlashComponent) —
 * "cuts through reality itself," so it ignores every resistance and
 * IncomingDamageMultiplier the target has (Category = True,
 * bIgnoresResistances = true). Also a template for authoring further
 * elemental USODamageType subclasses (BP_DamageType_Fire, etc.).
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USORealitySlashDamageType : public USODamageType
{
	GENERATED_BODY()

public:
	USORealitySlashDamageType();
};
