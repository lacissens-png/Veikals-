#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SODamageType.h"
#include "SOHealthComponent.generated.h"

class AController;
class UDamageType;

/**
 * Broadcast whenever the health value changes.
 *   OwningComponent - the component that fired the event (handy for UI wiring).
 *   OldHealth       - value before the change.
 *   NewHealth       - value after the change (clamped to [0, MaxHealth]).
 *   Delta           - NewHealth - OldHealth (negative on damage, positive on heal).
 *   InstigatedBy    - controller responsible for the change (nullable for self-damage / world damage).
 *   DamageCauser    - actor that caused the change (nullable).
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FSOOnHealthChanged,
	USOHealthComponent*, OwningComponent,
	float,               OldHealth,
	float,               NewHealth,
	float,               Delta,
	AController*,        InstigatedBy,
	AActor*,             DamageCauser);

/** Broadcast the frame the owner drops to 0 HP. Fires exactly once until Revive() is called. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSOOnDeath,
	USOHealthComponent*, OwningComponent,
	AController*,        InstigatedBy,
	AActor*,             DamageCauser);

UCLASS(ClassGroup = (SupremeOverlord), meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOHealthComponent();

protected:
	virtual void BeginPlay() override;

	/** Bound to the owning actor's OnTakeAnyDamage so any Kismet/AI damage naturally flows through this component. */
	UFUNCTION()
	void HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public:
	/** Maximum health. Changing this in the editor also clamps CurrentHealth on BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Health", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float MaxHealth = 100.0f;

	/** Health carried into play. If left at 0 (default), we snap to MaxHealth on BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Health", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float StartingHealth = 0.0f;

	/** When true, incoming damage is discarded. Useful for cinematics, iframes, or debug god-mode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Health")
	bool bInvulnerable = false;

	/** Multiplier applied to all incoming damage (before clamping). 1.0 = normal, 0.5 = resistant, 2.0 = vulnerable. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Health", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float IncomingDamageMultiplier = 1.0f;

	/**
	 * Fractional damage reduction per ESODamageCategory (0.25 = 25% less damage
	 * taken from that school). Looked up in HandleAnyDamage against the incoming
	 * USODamageType::Category; True-category damage and any USODamageType with
	 * bIgnoresResistances set always bypass this (and IncomingDamageMultiplier).
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Health|Resistance")
	TMap<ESODamageCategory, float> ElementalResistances;

	/** Resistance to Category, clamped to [0, 0.9]. 0 if no entry is set. */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Health|Resistance")
	float GetResistance(ESODamageCategory Category) const;

	/** Sets (or replaces) the resistance to Category, clamped to [0, 0.9]. Useful for gear/buffs granting resistance at runtime. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Health|Resistance")
	void SetResistance(ESODamageCategory Category, float Value);

	/** Fires on every change to CurrentHealth (damage, heal, kill, revive). */
	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Health")
	FSOOnHealthChanged OnHealthChanged;

	/** Fires the moment the owner reaches 0 HP. */
	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Health")
	FSOOnDeath OnDeath;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Health")
	float GetHealthPercent() const { return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Health")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Health")
	bool IsAlive() const { return !bIsDead; }

	/**
	 * USODamageType::HitReactStrength from the most recent damage instance
	 * that actually landed (defaults to 0.5 if none has landed, or the last
	 * hit used a plain UDamageType). Intended for hit-react animation/VFX
	 * intensity — ASOCharacter::HandleHealthChanged already scales its
	 * hit-stop camera shake by this.
	 */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Health")
	float GetLastHitReactStrength() const { return LastHitReactStrength; }

	/** Applies healing (clamped to MaxHealth). Ignored when dead. Returns the actual amount healed. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Health")
	float Heal(float HealAmount, AController* Instigator = nullptr, AActor* Healer = nullptr);

	/** Immediately drops HP to 0 and fires OnDeath. Bypasses invulnerability. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Health")
	void Kill(AController* InstigatedBy = nullptr, AActor* DamageCauser = nullptr);

	/** Restores health (defaults to full) and clears the dead state, firing OnHealthChanged. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Health")
	void Revive(float ReviveHealth = -1.0f);

private:
	/** Applies the actual delta and broadcasts. Handles the death transition when Health hits 0. */
	void ApplyHealthDelta(float Delta, AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Health", Transient)
	float CurrentHealth = 0.0f;

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Health", Transient)
	bool bIsDead = false;

	float LastHitReactStrength = 0.5f;
};
