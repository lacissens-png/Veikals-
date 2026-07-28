#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOStatusEffectComponent.generated.h"

UENUM(BlueprintType)
enum class ESOStatusEffectType : uint8
{
	Burning   UMETA(DisplayName = "Burning"),   // fire DOT
	Poisoned  UMETA(DisplayName = "Poisoned"),  // nature DOT, stacks
	Frozen    UMETA(DisplayName = "Frozen"),    // movement stopped
	Slowed    UMETA(DisplayName = "Slowed"),    // movement reduced
	Shocked   UMETA(DisplayName = "Shocked"),   // +25 % damage taken
	Blessed   UMETA(DisplayName = "Blessed"),   // heal over time
	Cursed    UMETA(DisplayName = "Cursed"),    // -25 % damage dealt
};

USTRUCT(BlueprintType)
struct FSOStatusEffect
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "StatusEffects")
	ESOStatusEffectType Type = ESOStatusEffectType::Burning;

	/** Damage dealt (or health restored if negative) per tick. */
	UPROPERTY(BlueprintReadOnly, Category = "StatusEffects")
	float DamagePerTick = 5.0f;

	UPROPERTY(BlueprintReadOnly, Category = "StatusEffects")
	float TickInterval = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "StatusEffects")
	float RemainingDuration = 3.0f;

	UPROPERTY(BlueprintReadOnly, Category = "StatusEffects")
	int32 CurrentStacks = 1;

	UPROPERTY(BlueprintReadOnly, Category = "StatusEffects")
	int32 MaxStacks = 1;

	/** Accumulated time since the last damage tick. Not replicated. */
	float TimeSinceLastTick = 0.0f;
};

// ---------------------------------------------------------------------------
// Delegates
// ---------------------------------------------------------------------------
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnStatusEffectApplied, ESOStatusEffectType, Type, int32, Stacks);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FSOOnStatusEffectRemoved,  ESOStatusEffectType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnStatusEffectTicked,  ESOStatusEffectType, Type, float, DamageDealt);

/**
 * Manages ticking status effects on the owning actor.
 * Add to any character (player or enemy) to support debuffs/buffs.
 *
 * Movement debuffs (Frozen, Slowed) directly modify the owner's
 * CharacterMovementComponent speed relative to the cached base value.
 * Shocked / Cursed effects expose damage multipliers that combat code
 * should query via GetIncomingDamageMultiplier() / GetOutgoingDamageMultiplier().
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOStatusEffectComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// ---------- API ----------

	/**
	 * Applies or refreshes/stacks an effect on the owner.
	 * If the effect already exists and CurrentStacks < MaxStacks, a new stack is added
	 * (damage per tick scales with stacks). Otherwise the duration is refreshed.
	 */
	UFUNCTION(BlueprintCallable, Category = "StatusEffects")
	void ApplyEffect(ESOStatusEffectType Type,
	                 float Duration,
	                 float DamagePerTick,
	                 float TickInterval = 1.0f,
	                 int32 MaxStacks    = 1);

	UFUNCTION(BlueprintCallable, Category = "StatusEffects")
	void RemoveEffect(ESOStatusEffectType Type);

	UFUNCTION(BlueprintCallable, Category = "StatusEffects")
	void ClearAllEffects();

	UFUNCTION(BlueprintPure, Category = "StatusEffects")
	bool HasEffect(ESOStatusEffectType Type) const;

	UFUNCTION(BlueprintPure, Category = "StatusEffects")
	float GetEffectRemainingTime(ESOStatusEffectType Type) const;

	UFUNCTION(BlueprintPure, Category = "StatusEffects")
	int32 GetEffectStacks(ESOStatusEffectType Type) const;

	UFUNCTION(BlueprintPure, Category = "StatusEffects")
	const TArray<FSOStatusEffect>& GetActiveEffects() const { return ActiveEffects; }

	/** Returns the multiplier that incoming damage should be scaled by (e.g. 1.25 when Shocked). */
	UFUNCTION(BlueprintPure, Category = "StatusEffects")
	float GetIncomingDamageMultiplier() const;

	/** Returns the multiplier that outgoing damage should be scaled by (e.g. 0.75 when Cursed). */
	UFUNCTION(BlueprintPure, Category = "StatusEffects")
	float GetOutgoingDamageMultiplier() const;

	// ---------- Tuning ----------

	/** How much Slowed reduces movement speed (0.5 = half speed). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffects|Movement", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SlowFraction = 0.5f;

	/** Incoming-damage multiplier while Shocked. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffects|Combat", meta = (ClampMin = "1.0"))
	float ShockedDamageMultiplier = 1.25f;

	/** Outgoing-damage multiplier while Cursed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffects|Combat", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CursedDamageMultiplier = 0.75f;

	// ---------- Delegates ----------
	UPROPERTY(BlueprintAssignable, Category = "StatusEffects")
	FSOOnStatusEffectApplied OnStatusEffectApplied;

	UPROPERTY(BlueprintAssignable, Category = "StatusEffects")
	FSOOnStatusEffectRemoved OnStatusEffectRemoved;

	UPROPERTY(BlueprintAssignable, Category = "StatusEffects")
	FSOOnStatusEffectTicked OnStatusEffectTicked;

	// ---------- BP hooks ----------
	UFUNCTION(BlueprintImplementableEvent, Category = "StatusEffects")
	void OnEffectAppliedBP(ESOStatusEffectType Type, int32 Stacks);

	UFUNCTION(BlueprintImplementableEvent, Category = "StatusEffects")
	void OnEffectRemovedBP(ESOStatusEffectType Type);

	UFUNCTION(BlueprintImplementableEvent, Category = "StatusEffects")
	void OnEffectTickBP(ESOStatusEffectType Type, float DamageDealt);

private:
	UPROPERTY(VisibleInstanceOnly, Category = "StatusEffects", Transient)
	TArray<FSOStatusEffect> ActiveEffects;

	/** Walk speed cached from the CharacterMovementComponent on BeginPlay. */
	float BaseWalkSpeed = 0.0f;

	FSOStatusEffect* FindEffect(ESOStatusEffectType Type);
	void ApplyMovementModifiers();
};
