#pragma once

#include "CoreMinimal.h"
#include "SODamageType.h"
#include "SONecroticDamageType.generated.h"

/** Necrotic-school damage type — see USORealitySlashDamageType for the template this follows. */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USONecroticDamageType : public USODamageType
{
	GENERATED_BODY()

public:
	USONecroticDamageType();
};
