#pragma once

#include "CoreMinimal.h"
#include "SODamageType.h"
#include "SOFireDamageType.generated.h"

/** Fire-school damage type — see USORealitySlashDamageType for the template this follows. */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOFireDamageType : public USODamageType
{
	GENERATED_BODY()

public:
	USOFireDamageType();
};
