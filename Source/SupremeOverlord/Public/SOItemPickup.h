#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOItemPickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USOItemData;
class ASOCharacter;

/**
 * Ground-loot actor carrying one USOItemData. On overlap it equips the
 * carried item on the picking-up character (weapons only for now) and
 * self-destroys. Rarity tints the mesh via a dynamic material instance so
 * you can eyeball drops at a glance without opening the tooltip.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOItemPickup : public AActor
{
	GENERATED_BODY()

public:
	ASOItemPickup();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Applies the carried item to Picker. Returns true if the item was consumed. */
	bool ApplyToCharacter(ASOCharacter* Picker);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|ItemPickup")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|ItemPickup")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	/** The item this pickup represents. Set at spawn time via SetItem. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "SupremeOverlord|ItemPickup", Transient)
	TObjectPtr<USOItemData> CarriedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|ItemPickup|Cosmetic")
	float SpinRateDegPerSec = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|ItemPickup|Cosmetic", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "50.0"))
	float BobAmplitude = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|ItemPickup|Cosmetic", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float BobRateHz = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|ItemPickup", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "600.0"))
	float DespawnAfter = 90.0f;

	/** Assigns the item and re-tints the mesh to the item's rarity color. Safe to call after spawn. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|ItemPickup")
	void SetItem(USOItemData* NewItem);

	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|ItemPickup")
	void OnPickedUp(ASOCharacter* Picker);

private:
	FVector SpawnBaseLocation = FVector::ZeroVector;
	float   TimeAlive         = 0.0f;
	bool    bClaimed          = false;
};
