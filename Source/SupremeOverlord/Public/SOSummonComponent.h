#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOSummonComponent.generated.h"

class ASOMinion;
class ASOCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnMinionSummoned, ASOMinion*, Minion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnMinionDied,     ASOMinion*, Minion);

/**
 * Manages the player's active minion army.
 * Attach to ASOCharacter. Wire MinionClass to a BP_Minion subclass in the editor.
 *
 * Key limits enforced on SummonMinion:
 *  - GetActiveCount() >= MaxMinions  → returns false (army full)
 *  - SummonCooldownRemaining > 0     → returns false (on cooldown)
 *  - ManaComponent->UseMana fails    → returns false (insufficient mana)
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOSummonComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOSummonComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// ---------- Config ----------

	/** Minion class spawned by SummonMinion. Assign a BP_Minion subclass in the editor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summoning")
	TSubclassOf<ASOMinion> MinionClass;

	/** Maximum simultaneous live minions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summoning", meta = (ClampMin = "1", UIMin = "1", UIMax = "20"))
	int32 MaxMinions = 3;

	/** Mana drained per summon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summoning", meta = (ClampMin = "0.0"))
	float ManaCostPerSummon = 30.0f;

	/** Minimum seconds between consecutive summons. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Summoning", meta = (ClampMin = "0.0"))
	float SummonCooldown = 1.5f;

	// ---------- Runtime API ----------

	/**
	 * Attempts to spawn a minion near TargetLocation.
	 * Returns false if the army is full, cooldown is active, mana is insufficient,
	 * or MinionClass is not set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	bool SummonMinion(FVector TargetLocation, ASOCharacter* Caster);

	/** Destroys every active minion immediately. */
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	void DismissAll();

	/** Counts live minions (prunes stale weak ptrs first). */
	UFUNCTION(BlueprintPure, Category = "Summoning")
	int32 GetActiveCount();

	UFUNCTION(BlueprintPure, Category = "Summoning")
	float GetSummonCooldownRemaining() const { return SummonCooldownRemaining; }

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "Summoning")
	FSOOnMinionSummoned OnMinionSummoned;

	UPROPERTY(BlueprintAssignable, Category = "Summoning")
	FSOOnMinionDied OnMinionDied;

private:
	TArray<TWeakObjectPtr<ASOMinion>> ActiveMinions;
	float SummonCooldownRemaining = 0.0f;
};
