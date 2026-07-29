#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SOEnemyAIController.generated.h"

class ASOEnemyCharacter;

UENUM(BlueprintType)
enum class ESOEnemyAIState : uint8
{
	Idle       UMETA(DisplayName = "Idle"),
	Chasing    UMETA(DisplayName = "Chasing"),
	Attacking  UMETA(DisplayName = "Attacking"),
	Fleeing    UMETA(DisplayName = "Fleeing"),
	Dead       UMETA(DisplayName = "Dead")
};

/**
 * Minimal native-C++ AI controller for the melee grunt.
 *   Idle    -> player enters SightRadius              -> Chasing
 *   Chasing -> distance <= AttackRange                -> Attacking
 *   Chasing -> distance > LoseSightRadius             -> Idle
 *   Attacking -> distance > AttackRange               -> Chasing
 *   Attacking -> cooldown elapsed                     -> PerformAttack, restart cooldown
 *   Any     -> owner dies                             -> Dead
 *
 * When Owner->bFleeFromPlayer is set, the chase/attack states above are
 * bypassed entirely in favor of Idle <-> Fleeing (run directly away from the
 * target once it enters SightRadius) — used by rare enemies that never fight.
 *
 * Kept intentionally single-file so no BehaviorTree/Blackboard assets are
 * required to test combat. Swap to a BT later without touching the enemy
 * character class.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASOEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void Tick(float DeltaSeconds) override;

	/** How often (seconds) to re-evaluate the AI. Cheap, but no reason to run every frame. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SupremeOverlord|AI", meta = (ClampMin = "0.05", UIMin = "0.05", UIMax = "1.0"))
	float ThinkInterval = 0.15f;

	/** Debug: draws state + target lines in the world. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|AI|Debug")
	bool bDrawDebug = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "SupremeOverlord|AI|Debug", Transient)
	ESOEnemyAIState CurrentState = ESOEnemyAIState::Idle;

private:
	/** Runs one AI evaluation tick. Called on a ThinkInterval timer. */
	void ThinkTick();

	/** Chooses the best hostile target. For now it's just Player 0 - swap later for a proper aggro list. */
	APawn* ResolveTargetPawn() const;

	void EnterState(ESOEnemyAIState NewState);
	void DoAttack(ASOEnemyCharacter* EnemyOwner, APawn* Target);

	FTimerHandle ThinkTimerHandle;
	FTimerHandle AttackCooldownHandle;

	/** True while an attack has landed and we're waiting for the next cooldown to elapse. */
	bool bAttackOnCooldown = false;
};
