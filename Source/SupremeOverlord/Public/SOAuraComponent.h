#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOAuraComponent.generated.h"

class ASOEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnAuraPulse, int32, EnemiesAffected);

/**
 * Passive dark aura radiating from the caster.
 * Every PulseInterval seconds it finds all live ASOEnemyCharacters within
 * AuraRadius and:
 *   1. Applies Slowed status (duration slightly > PulseInterval so they
 *      stay slowed continuously while inside the aura).
 *   2. Optionally deals DamagePerPulse as necrotic damage.
 *
 * Add to ASOCharacter; toggle with bAuraActive.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOAuraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOAuraComponent();

	virtual void BeginPlay()  override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	// ---------- Config ----------

	/** Radius (cm) of the dark aura effect. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura", meta = (ClampMin = "100.0"))
	float AuraRadius = 600.0f;

	/** Duration the Slowed status is applied on each pulse. Must be > PulseInterval to maintain a continuous slow. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura", meta = (ClampMin = "0.1"))
	float SlowDuration = 0.65f;

	/** Damage per pulse to every enemy in range (0 = no damage). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura", meta = (ClampMin = "0.0"))
	float DamagePerPulse = 0.0f;

	/** How often the aura pulses. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura", meta = (ClampMin = "0.1"))
	float PulseInterval = 0.5f;

	/** While false the aura is dormant (no damage, no slow). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura")
	bool bAuraActive = true;

	// ---------- Runtime ----------

	UFUNCTION(BlueprintPure, Category = "Aura")
	int32 GetEnemiesInRange(TArray<ASOEnemyCharacter*>& OutEnemies) const;

	// ---------- Delegates ----------

	/** Fires every pulse, with the number of enemies currently affected. */
	UPROPERTY(BlueprintAssignable, Category = "Aura")
	FSOOnAuraPulse OnAuraPulse;

	UFUNCTION(BlueprintImplementableEvent, Category = "Aura")
	void OnAuraPulseBP(int32 EnemiesAffected);

private:
	FTimerHandle PulseHandle;
	void Pulse();
};
