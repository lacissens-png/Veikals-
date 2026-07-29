#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOWaypoint.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * Placeable fast-travel marker. Stepping into DiscoveryRadius registers this
 * waypoint with the overlapping ASOCharacter's USOWaypointComponent; from then
 * on it appears in the waypoint map (M) and can be teleported to.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOWaypoint : public AActor
{
	GENERATED_BODY()

public:
	ASOWaypoint();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                           AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp,
	                           int32 OtherBodyIndex,
	                           bool bFromSweep,
	                           const FHitResult& SweepResult);

public:
	// ---------- Components ----------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint|Components")
	TObjectPtr<USphereComponent> DiscoverySphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint|Components")
	TObjectPtr<UStaticMeshComponent> WaypointMesh;

	// ---------- Config ----------

	/** Name shown in the waypoint map list and HUD toasts. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	FText WaypointName;

	/** Radius (cm) that discovers the waypoint on overlap. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint", meta = (ClampMin = "50.0"))
	float DiscoveryRadius = 250.0f;

	/** If true, this waypoint is already discovered by every character that begins play (a "home base" waypoint). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	bool bStartDiscovered = false;

	/** World-space offset from the waypoint's location that travelers land at, so they don't spawn inside the marker mesh. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	FVector TravelOffset = FVector(150.0f, 0.0f, 0.0f);

	UFUNCTION(BlueprintPure, Category = "Waypoint")
	FVector GetTravelDestination() const { return GetActorLocation() + TravelOffset; }

	UFUNCTION(BlueprintPure, Category = "Waypoint")
	bool IsDiscovered() const { return bDiscovered; }

	UFUNCTION(BlueprintPure, Category = "Waypoint")
	bool GetStartDiscovered() const { return bStartDiscovered; }

	/**
	 * Stable save/load identifier. Leave unset to fall back to the actor's own
	 * internal FName (stable across sessions as long as the placed instance
	 * isn't renamed) — set explicitly if you ever need to rename the actor
	 * without invalidating existing save files.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint")
	FName WaypointID;

	UFUNCTION(BlueprintPure, Category = "Waypoint")
	FName GetStableID() const { return WaypointID.IsNone() ? GetFName() : WaypointID; }

	/** BP hook for VFX/SFX the moment this waypoint is first discovered. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Waypoint")
	void OnDiscovered(class ASOCharacter* Discoverer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoint|Audio")
	TObjectPtr<class USoundBase> DiscoverSFX;

private:
	bool bDiscovered = false;
};
