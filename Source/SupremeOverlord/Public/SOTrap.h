#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOTrap.generated.h"

class USphereComponent;
class UDecalComponent;
class ASOCharacter;
class ASOEnemyCharacter;

UENUM(BlueprintType)
enum class ESOTrapType : uint8
{
	ShadowSnare   UMETA(DisplayName = "Shadow Snare"),    // Slows + moderate damage
	ArcaneMine    UMETA(DisplayName = "Arcane Mine"),     // Burst damage + Shocked (+25% damage taken)
	NecroticSpore UMETA(DisplayName = "Necrotic Spore"),  // Applies Poison DOT
};

/**
 * Placeable trap actor. Placed by ASOCharacter via C key, armed after ArmDelay.
 * Triggers on the first ASOEnemyCharacter that enters TriggerRadius.
 * Applies type-specific status effects and destroys itself after triggering.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOTrap : public AActor
{
	GENERATED_BODY()

public:
	ASOTrap();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);

	void Arm();

public:
	// ---------- Components ----------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap|Components")
	TObjectPtr<USphereComponent> TriggerSphere;

	// ---------- Config ----------

	/** Which trap variant this is. Set by ASOCharacter on spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	ESOTrapType TrapType = ESOTrapType::ShadowSnare;

	/** Seconds after placement before the trap becomes active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap", meta = (ClampMin = "0.0"))
	float ArmDelay = 0.8f;

	/** Radius (cm) that triggers the trap on enemy overlap. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap", meta = (ClampMin = "10.0"))
	float TriggerRadius = 150.0f;

	/** Damage applied on trigger (burst for ArcaneMine, initial hit for others). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap", meta = (ClampMin = "0.0"))
	float Damage = 25.0f;

	/** ShadowSnare: duration the Slowed effect lasts. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|ShadowSnare", meta = (ClampMin = "0.1"))
	float SlowDuration = 3.0f;

	/** ArcaneMine: duration the Shocked effect lasts (enemies take +25% damage). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|ArcaneMine", meta = (ClampMin = "0.1"))
	float ShockedDuration = 2.5f;

	/** NecroticSpore: duration of the Poison DOT. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|NecroticSpore", meta = (ClampMin = "0.1"))
	float PoisonDuration = 5.0f;

	/** NecroticSpore: damage per tick of the Poison. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap|NecroticSpore", meta = (ClampMin = "0.0"))
	float PoisonDamagePerTick = 8.0f;

	/** How long the corpse lingers after triggering. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap", meta = (ClampMin = "0.0"))
	float DestroyDelay = 0.15f;

	/** Set by ASOCharacter on spawn. Used for instigator attribution. */
	UPROPERTY(Transient)
	TWeakObjectPtr<ASOCharacter> OwnerCharacter;

	UFUNCTION(BlueprintPure, Category = "Trap")
	bool IsArmed() const { return bArmed; }

private:
	bool bArmed     = false;
	bool bTriggered = false;

	FTimerHandle ArmHandle;
};
