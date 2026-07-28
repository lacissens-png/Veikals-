#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SODodgeRollComponent.generated.h"

class ASOCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnDodgeRoll, FVector, FromLocation, FVector, RollDirection);

/**
 * Free, cooldown-gated evade — the ARPG staple "get me out of here" button.
 *
 * Unlike Blink (mana-cost teleport toward the cursor), the roll is a short
 * ground dash: it rolls in the character's current movement direction when
 * moving, or toward the cursor when standing still, covering RollDistance
 * over RollDuration at a constant speed. The whole roll is invulnerable.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USODodgeRollComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USODodgeRollComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	// ---------- Config ----------

	/** Total ground distance (cm) covered by one roll. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll", meta = (ClampMin = "50.0", UIMin = "100.0", UIMax = "1500.0"))
	float RollDistance = 450.0f;

	/** Seconds the roll takes to cover RollDistance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll", meta = (ClampMin = "0.05", UIMin = "0.05", UIMax = "1.0"))
	float RollDuration = 0.28f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll", meta = (ClampMin = "0.0"))
	float ManaCost = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DodgeRoll", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "10.0"))
	float Cooldown = 1.2f;

	// ---------- Runtime API ----------

	UFUNCTION(BlueprintPure, Category = "DodgeRoll")
	bool CanRoll() const;

	UFUNCTION(BlueprintPure, Category = "DodgeRoll")
	bool IsRolling() const { return bIsRolling; }

	/**
	 * Rolls Caster away using its current movement direction, falling back to the
	 * direction toward CursorLocation when the character is standing still.
	 * Returns true when the roll started.
	 */
	UFUNCTION(BlueprintCallable, Category = "DodgeRoll")
	bool Roll(FVector CursorLocation, ASOCharacter* Caster);

	UFUNCTION(BlueprintPure, Category = "DodgeRoll")
	float GetCooldownRemaining() const { return CooldownRemaining; }

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "DodgeRoll")
	FSOOnDodgeRoll OnDodgeRoll;

	UFUNCTION(BlueprintImplementableEvent, Category = "DodgeRoll")
	void OnDodgeRollBP(const FVector& FromLocation, const FVector& RollDirection);

private:
	void EndRoll();

	float CooldownRemaining = 0.0f;

	bool    bIsRolling         = false;
	float   RollTimeRemaining  = 0.0f;
	float   RollSpeed          = 0.0f;
	FVector RollDirectionCached = FVector::ZeroVector;

	TWeakObjectPtr<ASOCharacter> RollingCaster;
};
