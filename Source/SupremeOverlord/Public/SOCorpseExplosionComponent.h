#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOCorpseExplosionComponent.generated.h"

class ASOCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnCorpseExploded, int32, EnemiesHit, float, TotalDamage);

/**
 * Consumes the nearest dead ASOEnemyCharacter within SearchRange of the cursor
 * and detonates it as a necrotic AoE.  Damage = target's MaxHealth * HPFraction,
 * applied to every live enemy inside ExplosionRadius.
 *
 * Add to ASOCharacter; call Cast(CursorLocation, Caster) from the X binding.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOCorpseExplosionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOCorpseExplosionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// ---------- Config ----------

	/** Max distance from the cursor to search for a usable corpse. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CorpseExplosion", meta = (ClampMin = "50.0"))
	float SearchRange = 400.0f;

	/** Blast radius centered on the consumed corpse. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CorpseExplosion", meta = (ClampMin = "50.0"))
	float ExplosionRadius = 350.0f;

	/** Fraction of each enemy's MaxHealth dealt as damage (1.0 = instant kill). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CorpseExplosion",
	          meta = (ClampMin = "0.01", ClampMax = "5.0", UIMin = "0.01", UIMax = "2.0"))
	float HPFraction = 0.40f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CorpseExplosion", meta = (ClampMin = "0.0"))
	float ManaCost = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CorpseExplosion",
	          meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "30.0"))
	float Cooldown = 3.0f;

	// ---------- Runtime API ----------

	UFUNCTION(BlueprintPure, Category = "CorpseExplosion")
	bool CanCast() const;

	/**
	 * Finds the nearest corpse within SearchRange of CursorLocation, detonates it,
	 * and destroys the corpse.  Returns true when a cast was performed.
	 */
	UFUNCTION(BlueprintCallable, Category = "CorpseExplosion")
	bool Cast(FVector CursorLocation, ASOCharacter* Caster);

	UFUNCTION(BlueprintPure, Category = "CorpseExplosion")
	float GetCooldownRemaining() const { return CooldownRemaining; }

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "CorpseExplosion")
	FSOOnCorpseExploded OnCorpseExploded;

	UFUNCTION(BlueprintImplementableEvent, Category = "CorpseExplosion")
	void OnCorpseExplodedBP(int32 EnemiesHit, float TotalDamage);

private:
	float CooldownRemaining = 0.0f;
};
