#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOCorruptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSOOnOverlordModeStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSOOnOverlordModeEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnCorruptionChanged, float, NewFraction);

/**
 * Tracks a corruption meter that fills from enemy kills and decays over time.
 * When full (CurrentCorruption >= MaxCorruption) the player may activate
 * Overlord Mode (Z key): a timed burst that multiplies outgoing damage.
 *
 * Add to ASOCharacter.  ASOEnemyCharacter::HandleDeath calls AddCorruption.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOCorruptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOCorruptionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// ---------- Config ----------

	/** Maximum corruption before Overlord Mode becomes available. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption", meta = (ClampMin = "1.0"))
	float MaxCorruption = 100.0f;

	/** Corruption lost per second when not in Overlord Mode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption", meta = (ClampMin = "0.0"))
	float DecayRate = 3.0f;

	/** How long Overlord Mode lasts (seconds). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption", meta = (ClampMin = "0.1"))
	float OverlordModeDuration = 10.0f;

	/** Outgoing damage multiplier while Overlord Mode is active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Corruption", meta = (ClampMin = "1.0"))
	float OverlordDamageMultiplier = 1.5f;

	// ---------- Runtime API ----------

	/** Adds corruption (from kills). Clamps to [0, MaxCorruption]. */
	UFUNCTION(BlueprintCallable, Category = "Corruption")
	void AddCorruption(float Amount);

	/** Load support: sets the meter directly (clamped to [0, MaxCorruption]), bypassing the Overlord-Mode-active guard AddCorruption has. */
	UFUNCTION(BlueprintCallable, Category = "Corruption")
	void SetCorruption(float Amount);

	/**
	 * Activates Overlord Mode. No-op if corruption is not full or already active.
	 * Corruption is consumed (set to 0) when the mode expires.
	 */
	UFUNCTION(BlueprintCallable, Category = "Corruption")
	void ActivateOverlordMode();

	UFUNCTION(BlueprintPure, Category = "Corruption")
	float GetCorruption() const { return CurrentCorruption; }

	UFUNCTION(BlueprintPure, Category = "Corruption")
	float GetCorruptionFraction() const;

	UFUNCTION(BlueprintPure, Category = "Corruption")
	bool IsOverlordModeActive() const { return bOverlordModeActive; }

	UFUNCTION(BlueprintPure, Category = "Corruption")
	float GetOverlordModeTimeRemaining() const { return OverlordModeTimeRemaining; }

	/** Returns the outgoing damage multiplier (OverlordDamageMultiplier when active, 1.0 otherwise). */
	UFUNCTION(BlueprintPure, Category = "Corruption")
	float GetOutgoingDamageMultiplier() const;

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "Corruption")
	FSOOnOverlordModeStarted OnOverlordModeStarted;

	UPROPERTY(BlueprintAssignable, Category = "Corruption")
	FSOOnOverlordModeEnded OnOverlordModeEnded;

	UPROPERTY(BlueprintAssignable, Category = "Corruption")
	FSOOnCorruptionChanged OnCorruptionChanged;

	UFUNCTION(BlueprintImplementableEvent, Category = "Corruption")
	void OnOverlordModeStartedBP();

	UFUNCTION(BlueprintImplementableEvent, Category = "Corruption")
	void OnOverlordModeEndedBP();

private:
	float CurrentCorruption     = 0.0f;
	bool  bOverlordModeActive   = false;
	float OverlordModeTimeRemaining = 0.0f;
};
