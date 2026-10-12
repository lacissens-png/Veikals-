// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// Replicates swarm snapshots to clients with a FastArraySerializer.
//
// BANDWIDTH
//   Per mob per change: id (4) + quantized position (~6) + yaw/hp/arch/status
//   (4 bytes) = ~14 bytes before delta compression. Only CHANGED items are
//   sent, and the server pushes at SwarmNetUpdateHz (10 Hz default), so 300 mobs
//   in motion cost ~40 KB/s per client. Clients interpolate between snapshots.
//
// SCALING FURTHER (documented, not implemented here):
//   * Split into spatial-cell proxies so per-connection relevancy culls far cells.
//   * With Iris, replace the fast array with a custom NetSerializer bit-packing
//     positions relative to the cell origin (~4 bytes/mob).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "EclipseSwarmNetProxy.generated.h"

class UEclipseMobArchetype;
struct FEclipseSwarmNetArray;

USTRUCT()
struct FEclipseSwarmNetItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY() int32 MobId = 0;
	UPROPERTY() FVector_NetQuantize Position = FVector::ZeroVector;
	UPROPERTY() uint8 Yaw = 0;            // 256 steps.
	UPROPERTY() uint8 HealthPct = 255;    // 0..255.
	UPROPERTY() uint8 ArchetypeIndex = 0;
	UPROPERTY() uint8 StatusBits = 0;     // Eclipse::EStatusEffect bitmask (8 effects).

	// ---- Client-only interpolation state (not replicated) ----
	FVector RenderPosition = FVector::ZeroVector;
	float RenderYaw = 0.f;

	void PostReplicatedAdd(const FEclipseSwarmNetArray& InArraySerializer);
	void PostReplicatedChange(const FEclipseSwarmNetArray& InArraySerializer) {}
	void PreReplicatedRemove(const FEclipseSwarmNetArray& InArraySerializer) {}
};

USTRUCT()
struct FEclipseSwarmNetArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY() TArray<FEclipseSwarmNetItem> Items;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FEclipseSwarmNetItem, FEclipseSwarmNetArray>(Items, DeltaParms, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FEclipseSwarmNetArray> : public TStructOpsTypeTraitsBase2<FEclipseSwarmNetArray>
{
	enum { WithNetDeltaSerializer = true };
};

UCLASS(NotPlaceable, Transient)
class ECLIPSEGAME_API AEclipseSwarmNetProxy : public AActor
{
	GENERATED_BODY()

public:
	AEclipseSwarmNetProxy();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// ---- Server writer API (called by UEclipseSwarmSubsystem) ----
	void SetArchetypes(const TArray<TObjectPtr<UEclipseMobArchetype>>& InArchetypes);
	void BeginSnapshot();
	void WriteMob(int32 MobId, const FVector& Position, float YawDeg, float Health01, uint16 ArchetypeIdx, uint8 StatusBits);
	void EndSnapshot();

	// ---- Client reader API ----
	const TArray<TObjectPtr<UEclipseMobArchetype>>& GetArchetypes() const { return Archetypes; }

	template <typename Fn>
	void ForEachRenderable(Fn&& Visitor) const
	{
		for (const FEclipseSwarmNetItem& Item : Mobs.Items)
		{
			Visitor(static_cast<int32>(Item.ArchetypeIndex), Item.RenderPosition, Item.RenderYaw);
		}
	}

private:
	UPROPERTY(Replicated)
	FEclipseSwarmNetArray Mobs;

	/** Asset references replicate as stable net GUIDs (path-based), not as objects. */
	UPROPERTY(Replicated)
	TArray<TObjectPtr<UEclipseMobArchetype>> Archetypes;

	// Server bookkeeping: MobId -> item index, and which items were touched this snapshot.
	TMap<int32, int32> IndexById;
	TBitArray<> Touched;
};
