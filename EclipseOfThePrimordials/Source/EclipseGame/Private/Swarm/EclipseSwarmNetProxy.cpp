// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Swarm/EclipseSwarmNetProxy.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Swarm/EclipseSwarmSubsystem.h"

void FEclipseSwarmNetItem::PostReplicatedAdd(const FEclipseSwarmNetArray& InArraySerializer)
{
	// Spawn in place; subsequent changes are interpolated toward.
	RenderPosition = Position;
	RenderYaw = Yaw * (360.f / 256.f);
}

AEclipseSwarmNetProxy::AEclipseSwarmNetProxy()
{
	bReplicates = true;
	bAlwaysRelevant = true; // See header for the spatial-cell relevancy upgrade path.
	NetUpdateFrequency = 20.f; // Upper bound; the subsystem decides when data actually changes.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicatingMovement(false);
}

void AEclipseSwarmNetProxy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEclipseSwarmNetProxy, Mobs);
	DOREPLIFETIME(AEclipseSwarmNetProxy, Archetypes);
}

void AEclipseSwarmNetProxy::BeginPlay()
{
	Super::BeginPlay();

	// Only clients need to interpolate.
	SetActorTickEnabled(!HasAuthority());
	if (UEclipseSwarmSubsystem* Swarm = GetWorld()->GetSubsystem<UEclipseSwarmSubsystem>())
	{
		Swarm->RegisterNetProxy(this);
	}
}

void AEclipseSwarmNetProxy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Exponential smoothing toward the latest snapshot. At 10 Hz updates a rate
	// of 12 converges ~70% per snapshot interval: smooth, with ~1 snapshot of lag.
	constexpr float InterpSpeed = 12.f;
	for (FEclipseSwarmNetItem& Item : Mobs.Items)
	{
		Item.RenderPosition = FMath::VInterpTo(Item.RenderPosition, FVector(Item.Position), DeltaSeconds, InterpSpeed);
		const float TargetYaw = Item.Yaw * (360.f / 256.f);
		Item.RenderYaw = FMath::FInterpTo(Item.RenderYaw, Item.RenderYaw + FMath::FindDeltaAngleDegrees(Item.RenderYaw, TargetYaw), DeltaSeconds, InterpSpeed);
	}
}

void AEclipseSwarmNetProxy::SetArchetypes(const TArray<TObjectPtr<UEclipseMobArchetype>>& InArchetypes)
{
	Archetypes = InArchetypes;
}

void AEclipseSwarmNetProxy::BeginSnapshot()
{
	Touched.Init(false, Mobs.Items.Num());
}

void AEclipseSwarmNetProxy::WriteMob(int32 MobId, const FVector& Position, float YawDeg, float Health01, uint16 ArchetypeIdx, uint8 StatusBits)
{
	const uint8 QYaw = static_cast<uint8>(FMath::RoundToInt(FRotator::ClampAxis(YawDeg) * (256.f / 360.f)) & 0xFF);
	const uint8 QHealth = static_cast<uint8>(FMath::Clamp(FMath::RoundToInt(Health01 * 255.f), 0, 255));
	const uint8 QArch = static_cast<uint8>(ArchetypeIdx);

	if (const int32* Found = IndexById.Find(MobId))
	{
		FEclipseSwarmNetItem& Item = Mobs.Items[*Found];
		Touched[*Found] = true;

		// Only dirty items that changed meaningfully (FastArray then sends just those).
		const bool bChanged = FVector::DistSquared(Item.Position, Position) > 25.f // > 5 cm
			|| Item.Yaw != QYaw || Item.HealthPct != QHealth || Item.StatusBits != StatusBits;
		if (bChanged)
		{
			Item.Position = Position;
			Item.Yaw = QYaw;
			Item.HealthPct = QHealth;
			Item.StatusBits = StatusBits;
			Mobs.MarkItemDirty(Item);
		}
		return;
	}

	FEclipseSwarmNetItem& NewItem = Mobs.Items.AddDefaulted_GetRef();
	NewItem.MobId = MobId;
	NewItem.Position = Position;
	NewItem.Yaw = QYaw;
	NewItem.HealthPct = QHealth;
	NewItem.ArchetypeIndex = QArch;
	NewItem.StatusBits = StatusBits;
	Mobs.MarkItemDirty(NewItem);
	IndexById.Add(MobId, Mobs.Items.Num() - 1);
	Touched.Add(true);
}

void AEclipseSwarmNetProxy::EndSnapshot()
{
	// Remove items for mobs that no longer exist (iterate backwards for RemoveAtSwap).
	bool bRemoved = false;
	for (int32 K = Mobs.Items.Num() - 1; K >= 0; --K)
	{
		if (Touched[K])
		{
			continue;
		}
		IndexById.Remove(Mobs.Items[K].MobId);
		const int32 Last = Mobs.Items.Num() - 1;
		if (K != Last)
		{
			IndexById[Mobs.Items[Last].MobId] = K;
			Touched[K] = Touched[Last];
		}
		Mobs.Items.RemoveAtSwap(K, 1, EAllowShrinking::No);
		Touched.RemoveAt(Last);
		bRemoved = true;
	}
	if (bRemoved)
	{
		Mobs.MarkArrayDirty();
	}
}
