#pragma once

#include "CoreMinimal.h"
#include "SOVassalData.h"
#include "SOVassalData_Nyx.generated.h"

/**
 * Nyx, the Silent Fang — a swift assassin whose presence quickens every
 * strike. AttackSpeed vassal: fragile, fast, best summoned to burst down a
 * priority target quickly rather than to hold a line.
 *
 * A ready-to-use native example — right-click this class in the Content
 * Browser and "Create Blueprint Class" to give it a mesh/animations, or
 * reference it directly for quick testing.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOVassalData_Nyx : public USOVassalData
{
	GENERATED_BODY()

public:
	USOVassalData_Nyx();
};
