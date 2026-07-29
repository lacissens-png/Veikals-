#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOWaypointComponent.generated.h"

class ASOCharacter;
class ASOWaypoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnWaypointDiscovered, ASOWaypoint*, Waypoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnWaypointMapToggled, bool,         bOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnWaypointTravel,     ASOWaypoint*, Waypoint);

/**
 * Tracks discovered waypoints and handles fast travel between them — the
 * Diablo-style "step on the pillar, then teleport back to it later" loop.
 * Add to ASOCharacter; ASOWaypoint calls DiscoverWaypoint on overlap.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOWaypointComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOWaypointComponent();

protected:
	virtual void BeginPlay() override;

public:
	// ---------- Runtime API ----------

	/**
	 * Registers Waypoint as discovered (deduped). Safe to call repeatedly for
	 * the same waypoint. bSilent skips delegates/BP hooks — used when
	 * restoring from a save file so load doesn't replay every discovery FX/SFX.
	 */
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	bool DiscoverWaypoint(ASOWaypoint* Waypoint, bool bSilent = false);

	/** Save support: every discovered waypoint's stable ID, in discovery order. */
	UFUNCTION(BlueprintPure, Category = "Waypoint")
	TArray<FName> GetDiscoveredWaypointIDs() const;

	/** Save support: LastWaypoint's stable ID, or NAME_None if no waypoint has been discovered yet. */
	UFUNCTION(BlueprintPure, Category = "Waypoint")
	FName GetLastWaypointID() const;

	/**
	 * Load support: finds a placed ASOWaypoint in World whose GetStableID()
	 * matches StableID and discovers it silently. Returns false if no match
	 * is found in the current level.
	 */
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	bool RestoreDiscoveredWaypointByID(FName StableID, UWorld* World);

	/** Load support: re-points LastWaypoint at the already-discovered waypoint with StableID, without teleporting. */
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	void RestoreLastWaypointByID(FName StableID);

	UFUNCTION(BlueprintPure, Category = "Waypoint")
	const TArray<ASOWaypoint*>& GetDiscoveredWaypoints() const { return DiscoveredWaypoints; }

	/** Most recently discovered or traveled-to waypoint. Null until the first one is discovered. Used as the respawn point. */
	UFUNCTION(BlueprintPure, Category = "Waypoint")
	ASOWaypoint* GetLastWaypoint() const { return LastWaypoint; }

	UFUNCTION(BlueprintPure, Category = "Waypoint")
	bool IsMapOpen() const { return bMapOpen; }

	/** Opens/closes the waypoint map overlay. */
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	void ToggleMap();

	/**
	 * Teleports the owning character to the Index'th discovered waypoint (0-based).
	 * Closes the map on success. Returns false if the index is out of range or the owner isn't a character.
	 */
	UFUNCTION(BlueprintCallable, Category = "Waypoint")
	bool TravelToWaypoint(int32 Index);

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "Waypoint")
	FSOOnWaypointDiscovered OnWaypointDiscovered;

	UPROPERTY(BlueprintAssignable, Category = "Waypoint")
	FSOOnWaypointMapToggled OnWaypointMapToggled;

	UPROPERTY(BlueprintAssignable, Category = "Waypoint")
	FSOOnWaypointTravel OnWaypointTravel;

	UFUNCTION(BlueprintImplementableEvent, Category = "Waypoint")
	void OnWaypointDiscoveredBP(ASOWaypoint* Waypoint);

	UFUNCTION(BlueprintImplementableEvent, Category = "Waypoint")
	void OnWaypointTravelBP(ASOWaypoint* Waypoint);

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<ASOWaypoint>> DiscoveredWaypoints;

	UPROPERTY(Transient)
	TObjectPtr<ASOWaypoint> LastWaypoint;

	bool bMapOpen = false;
};
