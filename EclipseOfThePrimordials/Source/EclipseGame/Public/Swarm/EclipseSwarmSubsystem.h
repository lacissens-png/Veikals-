// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// EclipseSwarmSubsystem.h
// -----------------------------------------------------------------------------
// HIGH-DENSITY MOB SIMULATION (100-600 concurrent mobs)
//
// Why not one ACharacter per mob?
//   An ACharacter + CMC + ASC + skeletal mesh costs ~10-20 KB and a tick of
//   several microseconds; 300 of them blow the frame budget on the game thread
//   alone, before replication. Instead, mobs are rows in Structure-of-Arrays:
//
//     Position[i], Velocity[i], Health[i], Statuses[i], ArchetypeIndex[i] ...
//
//   Each system pass walks only the arrays it needs (cache-friendly), the
//   movement pass runs in ParallelFor, and rendering is one Instanced Static
//   Mesh draw per archetype (VAT materials for animation).
//
// FRAME PIPELINE (server)
//   1. GatherPlayers        - few pawns, linear
//   2. RebuildGrid          - Eclipse::SpatialHashGrid2D, O(N)
//   3. UpdateAuras (4 Hz)   - captains buff neighbours via grid queries
//   4. SimulateStatuses     - DoT damage, timers (Eclipse::StatusContainer)
//   5. SimulateMovement     - ParallelFor: seek + separation, distance LOD
//   6. ResolveAttacks       - batched: one damage GE per player per frame
//   7. UpdateLinkGroups     - Puppet Master's Thread expiry
//   8. ProcessDeaths        - deferred: slots freed only here, never mid-pass
//   9. PushNetSnapshot      - throttled (SwarmNetUpdateHz) to AEclipseSwarmNetProxy
//  10. UpdateRendering      - non-dedicated only; clients render from the proxy
//
// AUTHORITY
//   Every mutating API early-outs unless the world is the server. Clients never
//   simulate mobs; they interpolate replicated snapshots. All damage, deaths,
//   loot and ritual events originate here, on the server.
//
// MEMORY
//   Arrays are reserved to SwarmSimHardCap at BeginPlay, so steady-state
//   spawning/despawning performs zero heap allocations. Slots are recycled via
//   a free list with generational serials (FEclipseMobHandle).
// -----------------------------------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "EclipseCore/SpatialHash.h"
#include "EclipseCore/StatusEffects.h"
#include "Subsystems/WorldSubsystem.h"
#include "Swarm/EclipseSwarmTypes.h"

#include "EclipseSwarmSubsystem.generated.h"

class AEclipseSwarmNetProxy;
class UAbilitySystemComponent;
class UGameplayEffect;
class UInstancedStaticMeshComponent;

UCLASS()
class ECLIPSEGAME_API UEclipseSwarmSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	// --- USubsystem ------------------------------------------------------------
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	// --- Server: lifecycle -----------------------------------------------------
	FEclipseMobHandle SpawnMob(UEclipseMobArchetype* Archetype, const FVector& Location);
	void DespawnMob(FEclipseMobHandle Handle);

	// --- Server: combat --------------------------------------------------------
	FEclipseMobDamageResult ApplyDamageToMob(FEclipseMobHandle Handle, const FEclipseDamageSpec& Spec);
	Eclipse::ApplyOutcome ApplyStatusToMob(FEclipseMobHandle Handle, const FEclipseStatusSpec& Spec, AActor* Instigator);
	void ApplyImpulseToMob(FEclipseMobHandle Handle, const FVector& Impulse);

	/**
	 * Puppet Master's Thread: damage dealt to any member is mirrored at
	 * ShareFraction onto every other living member. A mob belongs to at most
	 * one group; joining a new group leaves the old one.
	 * @return group id, or INDEX_NONE.
	 */
	int32 CreateDamageLinkGroup(const TArray<FEclipseMobHandle>& Members, float ShareFraction, float Duration);

	// --- Queries (server; also valid on listen-server host) ---------------------
	int32 QueryMobsInRadius(const FVector& Center, float Radius, TArray<FEclipseMobHandle>& Out) const;
	/** Swept circle from Start to End (2D). Used by Reality Slash and beams. */
	int32 QueryMobsInCapsule(const FVector& Start, const FVector& End, float Radius, TArray<FEclipseMobHandle>& Out) const;
	/** Closest mob within Range whose direction is within HalfAngleDeg of Forward. */
	FEclipseMobHandle FindNearestMobInCone(const FVector& Origin, const FVector& Forward, float Range, float HalfAngleDeg) const;

	bool IsAlive(FEclipseMobHandle Handle) const;
	bool GetMobInfo(FEclipseMobHandle Handle, FEclipseMobInfo& Out) const;
	int32 GetNumAliveMobs() const { return NumAlive; }

	/** Server: fired from ProcessDeaths (loot director, quest counters, Biomass, ...). */
	FEclipseOnMobKilled OnMobKilled;

	// --- Client rendering bridge -------------------------------------------------
	void RegisterNetProxy(AEclipseSwarmNetProxy* Proxy);
	int32 FindOrAddArchetypeIndex(UEclipseMobArchetype* Archetype);
	UEclipseMobArchetype* GetArchetype(int32 Index) const { return Archetypes.IsValidIndex(Index) ? Archetypes[Index] : nullptr; }

private:
	enum EMobFlags : uint8
	{
		MF_Alive = 1 << 0,
		MF_PendingDeath = 1 << 1,
		MF_Despawned = 1 << 2, // Removed by design (no loot / kill credit).
	};

	struct FPlayerTarget
	{
		TWeakObjectPtr<AActor> Actor;
		TWeakObjectPtr<UAbilitySystemComponent> ASC;
		FVector2f Position;
		float PendingDamage = 0.f;
	};

	struct FDamageLinkGroup
	{
		TArray<FEclipseMobHandle> Members;
		float ShareFraction = 0.7f;
		float Remaining = 0.f;
	};

	bool HasAuthority() const;
	bool ResolveHandle(FEclipseMobHandle Handle, int32& OutIndex) const;
	FEclipseMobHandle MakeHandle(int32 Index) const { return FEclipseMobHandle{ Index, static_cast<int32>(Serial[Index]) }; }
	const UEclipseMobArchetype& Arch(int32 Index) const { return *Archetypes[ArchetypeIndex[Index]]; }
	int32 AllocateSlot();
	void ReserveAll(int32 Capacity);

	void GatherPlayers();
	void RebuildGrid();
	void UpdateAuras();
	void SimulateStatuses(float DeltaTime);
	void SimulateMovement(float DeltaTime);
	void ResolveAttacks(float DeltaTime);
	void UpdateLinkGroups(float DeltaTime);
	void ProcessDeaths();
	void PushNetSnapshot(float DeltaTime);
	void UpdateRendering();

	float ApplyDamageInternal(int32 Index, float Amount, float ArmorPen, EEclipseAffinity Affinity, AActor* Instigator);
	void LeaveLinkGroup(int32 Index);

	// ---- SoA mob storage (index == slot) ----------------------------------------
	TArray<FVector3f> Position;
	TArray<FVector2f> Velocity;
	TArray<FVector2f> Impulse;          // External knockback velocity, decays.
	TArray<float> Health;
	TArray<float> AttackCooldown;
	TArray<float> SimAccumulator;       // Distance-LOD time slicing.
	TArray<float> AuraDamageMult;
	TArray<float> AuraDamageReduction;
	TArray<float> AuraSpeedMult;
	TArray<uint16> ArchetypeIndex;
	TArray<uint32> Serial;
	TArray<uint8> Flags;
	TArray<int32> LinkGroup;
	TArray<int32> TargetPlayer;
	TArray<Eclipse::StatusContainer> Statuses;
	TArray<TWeakObjectPtr<AActor>> LastInstigator;

	TArray<int32> FreeSlots;
	/** Slots freed this frame. Recycled only after the next grid rebuild, so a stale
	 *  grid entry can never alias a mob spawned into a reused slot. */
	TArray<int32> DeferredFreeSlots;
	TArray<int32> PendingDeaths;
	int32 NumAlive = 0;
	int32 HardCap = 600;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UEclipseMobArchetype>> Archetypes;

	Eclipse::SpatialHashGrid2D Grid{ 400.f, 4096 };
	TArray<FPlayerTarget> Players;
	TSparseArray<FDamageLinkGroup> LinkGroups;

	float AuraAccumulator = 0.f;
	float NetAccumulator = 0.f;

	UPROPERTY(Transient)
	TSubclassOf<UGameplayEffect> MobDamageEffect;

	UPROPERTY(Transient)
	TObjectPtr<AEclipseSwarmNetProxy> NetProxy;

	// ---- Rendering (never on dedicated server) ----------------------------------
	UPROPERTY(Transient)
	TObjectPtr<AActor> RenderHost;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UInstancedStaticMeshComponent>> RenderISMs; // Indexed by archetype.

	TArray<TArray<FTransform>> RenderScratch;
};
