#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOConsumableComponent.generated.h"

class ASOCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnPotionUsed, float, HealedAmount, float, ManaRestored);

/**
 * Diablo-style quick-use health/mana potion — a fixed charge pool with a
 * short cooldown to stop spam-chugging, refilled at waypoints rather than
 * bought per-use. Charges are restored via RefillCharges, which
 * USOWaypointComponent::TravelToWaypoint calls automatically on arrival.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOConsumableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOConsumableComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// ---------- Config ----------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable", meta = (ClampMin = "0", UIMin = "0", UIMax = "10"))
	int32 MaxCharges = 4;

	/** Fraction of MaxHealth restored per use. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HealFraction = 0.35f;

	/** Fraction of MaxMana restored per use. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ManaFraction = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "20.0"))
	float UseCooldown = 3.0f;

	// ---------- Runtime API ----------

	UFUNCTION(BlueprintPure, Category = "Consumable")
	int32 GetCurrentCharges() const { return CurrentCharges; }

	UFUNCTION(BlueprintPure, Category = "Consumable")
	float GetCooldownRemaining() const { return CooldownRemaining; }

	UFUNCTION(BlueprintPure, Category = "Consumable")
	bool CanUse() const { return CurrentCharges > 0 && CooldownRemaining <= 0.0f; }

	/** Consumes one charge, healing/restoring mana on Caster. Returns false if out of charges or on cooldown. */
	UFUNCTION(BlueprintCallable, Category = "Consumable")
	bool UsePotion(ASOCharacter* Caster);

	/** Tops charges back up to MaxCharges. Returns the number of charges actually restored. */
	UFUNCTION(BlueprintCallable, Category = "Consumable")
	int32 RefillCharges();

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "Consumable")
	FSOOnPotionUsed OnPotionUsed;

	UFUNCTION(BlueprintImplementableEvent, Category = "Consumable")
	void OnPotionUsedBP(float HealedAmount, float ManaRestored);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable|Audio")
	TObjectPtr<class USoundBase> PotionUseSFX;

private:
	int32 CurrentCharges    = 0;
	float CooldownRemaining = 0.0f;
};
