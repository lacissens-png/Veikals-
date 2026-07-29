#pragma once

#include "CoreMinimal.h"
#include "SODamageType.h"
#include "SOShadowDamageType.generated.h"

/**
 * Shadow-school damage type — the game's signature school, used by Shadow Bolt
 * and the Shadow Snare trap. See USORealitySlashDamageType for the template
 * this follows.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOShadowDamageType : public USODamageType
{
	GENERATED_BODY()

public:
	USOShadowDamageType();
};
