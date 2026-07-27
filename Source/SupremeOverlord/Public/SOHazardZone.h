#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SODamageType.h"
#include "SOHazardZone.generated.h"

class UBoxComponent;
class UDecalComponent;
class UParticleSystem;
class UParticleSystemComponent;

UENUM(BlueprintType)
enum class ESOHazardType : uint8
{
	Fire      UMETA(DisplayName = "Fire"),
	Poison    UMETA(DisplayName = "Poison"),
	Frost     UMETA(DisplayName = "Frost"),
	Lightning UMETA(DisplayName = "Lightning"),
	Necrotic  UMETA(DisplayName = "Necrotic"),
};

/**
 * A placeable hazard zone that ticks periodic damage onto any pawn inside it.
 * Set HazardType in the BP subclass to drive damage category and visual colour.
 */
UCLASS(Blueprintable)
class SUPREMEOVERLORD_API ASOHazardZone : public AActor
{
	GENERATED_BODY()

public:
	ASOHazardZone();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// ---------- Shape ----------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard|Components")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard|Components")
	TObjectPtr<UDecalComponent> GroundDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hazard|Components")
	TObjectPtr<UParticleSystemComponent> AmbientFX;

	// ---------- Config ----------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard")
	ESOHazardType HazardType = ESOHazardType::Fire;

	/** Damage dealt per tick to each actor inside. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard", meta = (ClampMin = "0"))
	float DamagePerTick = 10.0f;

	/** Seconds between damage ticks. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard", meta = (ClampMin = "0.1"))
	float TickInterval = 0.5f;

	/** If > 0 the zone despawns after this many seconds. 0 = permanent. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard", meta = (ClampMin = "0"))
	float Lifetime = 0.0f;

	/** Optional particle system played on each damage tick per actor hit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard|FX")
	TObjectPtr<UParticleSystem> HitFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard|FX")
	TObjectPtr<UMaterialInterface> DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hazard|FX")
	TObjectPtr<UParticleSystem> AmbientParticles;

	// ---------- Overlap tracking ----------
	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActorsInside;

	FTimerHandle DamageTickHandle;
	FTimerHandle LifetimeHandle;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                    bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void DamageTick();

	/** Returns the USODamageType subclass matching HazardType. */
	TSubclassOf<UDamageType> GetDamageTypeClass() const;

	/** Returns a representative colour for the decal tint (BP can override). */
	UFUNCTION(BlueprintPure, Category = "Hazard")
	FLinearColor GetHazardColor() const;

	/** BP hook fired when the zone expires naturally. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Hazard")
	void OnZoneExpired();
};
