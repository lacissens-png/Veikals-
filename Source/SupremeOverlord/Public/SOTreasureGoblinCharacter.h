#pragma once

#include "CoreMinimal.h"
#include "SOEnemyCharacter.h"
#include "SOTreasureGoblinCharacter.generated.h"

/**
 * Rare, high-value enemy that never fights — it flees from the player at
 * elevated speed (bFleeFromPlayer, see ASOEnemyAIController) and detonates
 * into a guaranteed, oversized loot burst on death. Place sparingly (a wave
 * spawner "rare" slot, a hand-placed ambush) and let a generous LootTable /
 * ItemDropPool do the rest — the constructor just dials in sensible
 * treasure-goblin defaults so a designer doesn't have to configure every
 * flee/loot flag by hand on a plain ASOEnemyCharacter.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOTreasureGoblinCharacter : public ASOEnemyCharacter
{
	GENERATED_BODY()

public:
	ASOTreasureGoblinCharacter();
};
