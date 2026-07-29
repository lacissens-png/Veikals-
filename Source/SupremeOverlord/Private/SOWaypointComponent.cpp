#include "SOWaypointComponent.h"

#include "EngineUtils.h"
#include "SOCharacter.h"
#include "SOConsumableComponent.h"
#include "SOWaypoint.h"

USOWaypointComponent::USOWaypointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOWaypointComponent::BeginPlay()
{
	Super::BeginPlay();

	// Silently register every placed "home base" waypoint (bStartDiscovered)
	// so it's on the map from the very first frame, without needing to walk
	// into it first. Scanning here (rather than from the waypoint actor's own
	// BeginPlay) sidesteps any actor BeginPlay ordering hazard against the
	// player pawn being possessed yet.
	if (UWorld* World = GetWorld())
	{
		for (TActorIterator<ASOWaypoint> It(World); It; ++It)
		{
			if (It->GetStartDiscovered())
			{
				DiscoverWaypoint(*It, /*bSilent=*/ true);
			}
		}
	}
}

bool USOWaypointComponent::DiscoverWaypoint(ASOWaypoint* Waypoint, bool bSilent)
{
	if (!Waypoint || DiscoveredWaypoints.Contains(Waypoint))
	{
		return false;
	}

	DiscoveredWaypoints.Add(Waypoint);
	LastWaypoint = Waypoint;

	if (!bSilent)
	{
		OnWaypointDiscovered.Broadcast(Waypoint);
		OnWaypointDiscoveredBP(Waypoint);
	}
	return true;
}

TArray<FName> USOWaypointComponent::GetDiscoveredWaypointIDs() const
{
	TArray<FName> IDs;
	IDs.Reserve(DiscoveredWaypoints.Num());
	for (const ASOWaypoint* Waypoint : DiscoveredWaypoints)
	{
		if (Waypoint)
		{
			IDs.Add(Waypoint->GetStableID());
		}
	}
	return IDs;
}

FName USOWaypointComponent::GetLastWaypointID() const
{
	return LastWaypoint ? LastWaypoint->GetStableID() : NAME_None;
}

bool USOWaypointComponent::RestoreDiscoveredWaypointByID(FName StableID, UWorld* World)
{
	if (StableID.IsNone() || !World)
	{
		return false;
	}

	for (TActorIterator<ASOWaypoint> It(World); It; ++It)
	{
		if (It->GetStableID() == StableID)
		{
			return DiscoverWaypoint(*It, true);
		}
	}
	return false;
}

void USOWaypointComponent::RestoreLastWaypointByID(FName StableID)
{
	for (ASOWaypoint* Waypoint : DiscoveredWaypoints)
	{
		if (Waypoint && Waypoint->GetStableID() == StableID)
		{
			LastWaypoint = Waypoint;
			return;
		}
	}
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
