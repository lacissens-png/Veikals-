#pragma once

#include "CoreMinimal.h"
#include "SODamageType.h"
#include "SOFrostDamageType.generated.h"

/** Frost-school damage type — see USORealitySlashDamageType for the template this follows. */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOFrostDamageType : public USODamageType
{
	GENERATED_BODY()

public:
	USOFrostDamageType();
};
