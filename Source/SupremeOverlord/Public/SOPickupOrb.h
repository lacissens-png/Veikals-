#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOPickupOrb.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ASOCharacter;

UENUM(BlueprintType)
enum class ESOOrbType : uint8
{
	Health UMETA(DisplayName = "Health"),
	Mana   UMETA(DisplayName = "Mana"),
	Gold   UMETA(DisplayName = "Gold")
};

/**
 * Floating pickup orb spawned when an enemy dies. Rotates in place and, once
 * the player enters MagnetRadius, homes in until touched. On overlap it
 * applies its payload (Amount) based on OrbType, fires OnPickedUp for BP
 * VFX/SFX, and self-destroys.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOPickupOrb : public AActor
{
	GENERATED_BODY()

public:
	ASOPickupOrb();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Attempts to apply the orb's payload to Picker. Returns true if anything landed. */
	bool ApplyPayloadTo(ASOCharacter* Picker);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Pickup")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Pickup")
	TObjectPtr<UStaticMeshComponent> OrbMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Pickup")
	ESOOrbType OrbType = ESOOrbType::Health;

	/** How much HP/Mana to restore, or how much gold to grant. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Pickup", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10000.0"))
	float Amount = 25.0f;

	/** Radius the orb must be within before it starts homing toward the player. 0 disables magnetize. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Pickup|Magnet", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2000.0"))
	float MagnetRadius = 350.0f;

	/** Interp speed used to home in on the player once inside MagnetRadius. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Pickup|Magnet", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "50.0"))
	float MagnetInterpSpeed = 8.0f;

	/** Degrees per second the mesh spins for a bit of visual life. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Pickup|Cosmetic")
	float SpinRateDegPerSec = 180.0f;

	/** Height (cm) above the spawn point the orb bobs to. Zero disables bob. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Pickup|Cosmetic", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "50.0"))
	float BobAmplitude = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Pickup|Cosmetic", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float BobRateHz = 1.5f;

	/** Seconds after which the orb despawns if never picked up. 0 = never. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Pickup", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "300.0"))
	float DespawnAfter = 30.0f;

	/** BP hook for pickup VFX/SFX. Called just before Destroy(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Pickup")
	void OnPickedUp(ASOCharacter* Picker);

private:
	FVector SpawnBaseLocation = FVector::ZeroVector;
	float   TimeAlive         = 0.0f;
	bool    bClaimed          = false;
};
