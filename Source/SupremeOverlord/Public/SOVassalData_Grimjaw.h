#pragma once

#include "CoreMinimal.h"
#include "SOVassalData.h"
#include "SOVassalData_Grimjaw.generated.h"

/**
 * Grimjaw, Iron Warden — raw muscle with no passive buff, recruited for the
 * fight itself rather than the aura it grants. Highest health and damage of
 * the three example vassals; a straightforward brawler.
 *
 * A ready-to-use native example — right-click this class in the Content
 * Browser and "Create Blueprint Class" to give it a mesh/animations, or
 * reference it directly for quick testing.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOVassalData_Grimjaw : public USOVassalData
{
	GENERATED_BODY()

public:
	USOVassalData_Grimjaw();
};
