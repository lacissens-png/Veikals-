#pragma once

#include "CoreMinimal.h"
#include "SOEnemyCharacter.h"
#include "SOCasterEnemyCharacter.generated.h"

class ASOShadowBoltProjectile;

/**
 * Ranged evil-mage grunt. Reuses ASOEnemyAIController — same
 * chase / attack / lose-sight state machine — but overrides
 * PerformAttack to spawn a projectile at the target instead of
 * applying damage directly.
 *
 * Push AttackRange up (default 1100 cm) so it stays away from melee
 * range, and let it fire on its usual AttackCooldown.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOCasterEnemyCharacter : public ASOEnemyCharacter
{
	GENERATED_BODY()

public:
	ASOCasterEnemyCharacter();

	virtual bool PerformAttack(AActor* Target) override;

	/** Projectile class thrown at targets. Typically a BP subclass of ASOShadowBoltProjectile. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Caster")
	TSubclassOf<ASOShadowBoltProjectile> BoltClass;

	/** Damage stamped onto the spawned bolt. Overrides the projectile's own default. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Caster", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float BoltDamage = 22.0f;

	/** Muzzle offset forward from the actor origin. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Caster", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "400.0"))
	float MuzzleForward = 80.0f;

	/** Muzzle offset up from the actor origin (mid-torso height by default). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Caster", meta = (ClampMin = "-100.0", UIMin = "-100.0", UIMax = "200.0"))
	float MuzzleHeight = 60.0f;

	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Caster")
	void OnCasterAttackFired(const FVector& MuzzleLocation, ASOShadowBoltProjectile* SpawnedBolt);
};
