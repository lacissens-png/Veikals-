#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOCursedGround.generated.h"

class USphereComponent;
class ASOCharacter;
class ASOEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnCursedGroundTick, int32, EnemiesAffected, float, DamageDealt);

/**
 * Persistent hazard zone placed at the cursor (H key).
 * Every TickInterval it damages and slows every live ASOEnemyCharacter standing
 * inside AreaRadius, then expires after Duration seconds.
 *
 * Unlike ASOTrap this does not consume itself on the first contact — it is a
 * zoning tool meant to deny ground and soften packs the minions walk into.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOCursedGround : public AActor
{
	GENERATED_BODY()

public:
	ASOCursedGround();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	/** Timer callback — damages and debuffs everything inside the area. */
	void ApplyGroundEffects();

public:
	// ---------- Components ----------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CursedGround|Components")
	TObjectPtr<USphereComponent> AreaSphere;

	// ---------- Config ----------

	/** Radius (cm) of the hazard zone. Applied to AreaSphere on BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursedGround", meta = (ClampMin = "50.0", UIMin = "50.0", UIMax = "2000.0"))
	float AreaRadius = 300.0f;

	/** Total lifetime (s) before the zone expires. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursedGround", meta = (ClampMin = "0.5", UIMin = "1.0", UIMax = "60.0"))
	float Duration = 8.0f;

	/** Seconds between damage pulses. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursedGround", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "5.0"))
	float TickInterval = 0.75f;

	/** Damage applied to each enemy per pulse. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursedGround", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "500.0"))
	float DamagePerTick = 10.0f;

	/** When true, each pulse also refreshes a Slowed effect on everything inside. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursedGround")
	bool bAppliesSlow = true;

	/**
	 * Duration of the Slowed effect refreshed on every pulse.
	 * Keep it above TickInterval so enemies stay slowed while standing in the zone.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursedGround", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "10.0"))
	float SlowDuration = 1.5f;

	/** When true, each pulse also applies Cursed (-25% outgoing damage) to enemies inside. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursedGround")
	bool bAppliesCurse = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CursedGround", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "10.0"))
	float CurseDuration = 2.0f;

	/** Set by ASOCharacter on spawn — used for damage attribution and XP credit. */
	UPROPERTY(Transient)
	TWeakObjectPtr<ASOCharacter> OwnerCharacter;

	// ---------- Runtime ----------

	/** Seconds left before the zone expires. */
	UFUNCTION(BlueprintPure, Category = "CursedGround")
	float GetRemainingDuration() const;

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "CursedGround")
	FSOOnCursedGroundTick OnCursedGroundTick;

	UFUNCTION(BlueprintImplementableEvent, Category = "CursedGround")
	void OnCursedGroundTickBP(int32 EnemiesAffected, float DamageDealt);

private:
	FTimerHandle PulseHandle;
};
