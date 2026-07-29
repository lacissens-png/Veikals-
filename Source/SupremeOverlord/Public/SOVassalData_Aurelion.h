#pragma once

#include "CoreMinimal.h"
#include "SOVassalData.h"
#include "SOVassalData_Aurelion.generated.h"

/**
 * Aurelion, Ember Sentinel — a stoic guardian who shields the Overlord from
 * harm. DamageReduction vassal: tanky, low damage, meant to be summoned when
 * the fight calls for survivability over burst.
 *
 * A ready-to-use native example — right-click this class in the Content
 * Browser and "Create Blueprint Class" to give it a mesh/animations, or
 * reference it directly for quick testing.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOVassalData_Aurelion : public USOVassalData
{
	GENERATED_BODY()

public:
	USOVassalData_Aurelion();
};
