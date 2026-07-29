#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOManaComponent.generated.h"

/**
 * Broadcast whenever the mana value changes.
 *   OldMana - value before the change.
 *   NewMana - value after the change (clamped to [0, MaxMana]).
 *   Delta   - NewMana - OldMana (negative on consume, positive on regen/restore).
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSOOnManaChanged,
	USOManaComponent*, OwningComponent,
	float,             OldMana,
	float,             NewMana,
	float,             Delta);

UCLASS(ClassGroup = (SupremeOverlord), meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOManaComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Mana", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float MaxMana = 100.0f;

	/** Starting mana. If left at 0 (default), we snap to MaxMana on BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Mana", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float StartingMana = 0.0f;

	/** Mana regenerated per second. Zero disables regen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Mana", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "200.0"))
	float RegenPerSecond = 10.0f;

	/**
	 * Seconds of "regen lockout" after Consume() succeeds. Prevents shots from
	 * feeling free the moment you fire because regen refills them next tick.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Mana", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float RegenDelayAfterConsume = 1.0f;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Mana")
	FSOOnManaChanged OnManaChanged;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Mana")
	float GetCurrentMana() const { return CurrentMana; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Mana")
	float GetManaPercent() const { return MaxMana > 0.0f ? CurrentMana / MaxMana : 0.0f; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Mana")
	bool HasEnough(float Cost) const { return CurrentMana + KINDA_SMALL_NUMBER >= Cost; }

	/** Attempts to spend Cost mana. Returns true if it succeeded and applies the regen delay. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Mana")
	bool Consume(float Cost);

	/** Adds Amount mana (clamped to MaxMana). Returns the actual amount added. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Mana")
	float Restore(float Amount);

	/** Snaps mana to MaxMana. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Mana")
	void RefillToMax();

	/**
	 * Call after mutating MaxMana downward (unequipping a mana-bonus item,
	 * respec'ing away a mana talent, etc). Same overheal-style gap as
	 * USOHealthComponent::ClampCurrentHealthToMax - MaxMana has no automatic
	 * clamp of its own. No-op if CurrentMana is already at or below MaxMana.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Mana")
	void ClampCurrentManaToMax();

private:
	void ApplyManaDelta(float Delta);

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Mana", Transient)
	float CurrentMana = 0.0f;

	/** Seconds remaining before regen resumes after the last Consume. */
	float RegenLockoutRemaining = 0.0f;
};
