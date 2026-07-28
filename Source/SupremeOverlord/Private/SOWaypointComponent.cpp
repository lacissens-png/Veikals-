#include "SOWaypointComponent.h"

#include "SOCharacter.h"
#include "SOConsumableComponent.h"
#include "SOWaypoint.h"

USOWaypointComponent::USOWaypointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USOWaypointComponent::DiscoverWaypoint(ASOWaypoint* Waypoint)
{
	if (!Waypoint || DiscoveredWaypoints.Contains(Waypoint))
	{
		return false;
	}

	DiscoveredWaypoints.Add(Waypoint);
	LastWaypoint = Waypoint;
	OnWaypointDiscovered.Broadcast(Waypoint);
	OnWaypointDiscoveredBP(Waypoint);
	return true;
}

void USOWaypointComponent::ToggleMap()
{
	bMapOpen = !bMapOpen;
	OnWaypointMapToggled.Broadcast(bMapOpen);
}

bool USOWaypointComponent::TravelToWaypoint(int32 Index)
{
	if (!DiscoveredWaypoints.IsValidIndex(Index))
	{
		return false;
	}

	ASOWaypoint* Waypoint = DiscoveredWaypoints[Index];
	if (!Waypoint)
	{
		return false;
	}

	ASOCharacter* Owner = Cast<ASOCharacter>(GetOwner());
	if (!Owner || !Owner->IsAlive())
	{
		return false;
	}

	if (!Owner->TeleportTo(Waypoint->GetTravelDestination(), Owner->GetActorRotation(), false, false))
	{
		return false;
	}

	bMapOpen = false;
	OnWaypointMapToggled.Broadcast(false);
	LastWaypoint = Waypoint;

	// Diablo-style QoL: arriving at a waypoint tops off potion charges.
	if (Owner->ConsumableComponent)
	{
		Owner->ConsumableComponent->RefillCharges();
	}

	OnWaypointTravel.Broadcast(Waypoint);
	OnWaypointTravelBP(Waypoint);
	return true;
}
