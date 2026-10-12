// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Swarm/EclipseSwarmSubsystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Async/ParallelFor.h"
#include "Combat/EclipseStatusComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "EclipseCore/DamageMath.h"
#include "EclipseGameSettings.h"
#include "EclipseGameplayTags.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Swarm/EclipseSwarmNetProxy.h"
#include "World/EclipseFieldAffinitySubsystem.h"

DECLARE_STATS_GROUP(TEXT("EclipseSwarm"), STATGROUP_EclipseSwarm, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Swarm Tick"), STAT_SwarmTick, STATGROUP_EclipseSwarm);
DECLARE_CYCLE_STAT(TEXT("Swarm Movement"), STAT_SwarmMovement, STATGROUP_EclipseSwarm);
DECLARE_CYCLE_STAT(TEXT("Swarm Render"), STAT_SwarmRender, STATGROUP_EclipseSwarm);
DECLARE_DWORD_COUNTER_STAT(TEXT("Alive Mobs"), STAT_SwarmAlive, STATGROUP_EclipseSwarm);

namespace EclipseSwarm
{
	// Distance LOD: far mobs simulate at lower rates (time-sliced, not frozen).
	constexpr float NearDistSq = 2500.f * 2500.f;
	constexpr float MidDistSq = 6000.f * 6000.f;
	constexpr float MidInterval = 1.f / 30.f;
	constexpr float FarInterval = 1.f / 10.f;

	constexpr int32 MaxSeparationNeighbours = 8;
	constexpr float SeparationWeight = 1.5f;
	constexpr float ImpulseDamping = 6.f;       // 1/s exponential decay of knockback velocity.
	constexpr float AuraUpdateInterval = 0.25f;
	constexpr int32 ParallelThreshold = 64;     // Below this ParallelFor overhead dominates.
}

// =============================================================================
// Lifecycle
// =============================================================================

bool UEclipseSwarmSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	return World && (World->IsGameWorld());
}

void UEclipseSwarmSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	const UEclipseGameSettings* Settings = UEclipseGameSettings::Get();
	HardCap = Settings->SwarmSimHardCap;

	if (HasAuthority())
	{
		ReserveAll(HardCap);
		MobDamageEffect = Settings->MobDamageEffect.LoadSynchronous();

		// One replicated proxy carries snapshots of every mob to clients.
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		NetProxy = InWorld.SpawnActor<AEclipseSwarmNetProxy>(Params);
		if (NetProxy)
		{
			NetProxy->SetArchetypes(Archetypes);
		}
	}

	if (InWorld.GetNetMode() != NM_DedicatedServer)
	{
		FActorSpawnParameters Params;
		Params.ObjectFlags |= RF_Transient;
		RenderHost = InWorld.SpawnActor<AActor>(Params);
		RenderHost->SetRootComponent(NewObject<USceneComponent>(RenderHost, TEXT("SwarmRenderRoot")));
		RenderHost->GetRootComponent()->RegisterComponent();
	}
}

void UEclipseSwarmSubsystem::Deinitialize()
{
	Players.Reset();
	LinkGroups.Reset();
	Super::Deinitialize();
}

TStatId UEclipseSwarmSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UEclipseSwarmSubsystem, STATGROUP_Tickables);
}

bool UEclipseSwarmSubsystem::HasAuthority() const
{
	const UWorld* World = GetWorld();
	return World && World->GetNetMode() != NM_Client;
}

void UEclipseSwarmSubsystem::ReserveAll(int32 Capacity)
{
	Position.Reserve(Capacity);
	Velocity.Reserve(Capacity);
	Impulse.Reserve(Capacity);
	Health.Reserve(Capacity);
	AttackCooldown.Reserve(Capacity);
	SimAccumulator.Reserve(Capacity);
	AuraDamageMult.Reserve(Capacity);
	AuraDamageReduction.Reserve(Capacity);
	AuraSpeedMult.Reserve(Capacity);
	ArchetypeIndex.Reserve(Capacity);
	Serial.Reserve(Capacity);
	Flags.Reserve(Capacity);
	LinkGroup.Reserve(Capacity);
	TargetPlayer.Reserve(Capacity);
	Statuses.Reserve(Capacity);
	LastInstigator.Reserve(Capacity);
	FreeSlots.Reserve(Capacity);
	DeferredFreeSlots.Reserve(Capacity);
	PendingDeaths.Reserve(Capacity);
}

int32 UEclipseSwarmSubsystem::AllocateSlot()
{
	if (FreeSlots.Num() > 0)
	{
		return FreeSlots.Pop(EAllowShrinking::No);
	}
	// Grow every column in lock-step. Reserved up-front, so no reallocation in practice.
	const int32 Index = Position.AddDefaulted();
	Velocity.AddDefaulted();
	Impulse.AddDefaulted();
	Health.AddDefaulted();
	AttackCooldown.AddDefaulted();
	SimAccumulator.AddDefaulted();
	AuraDamageMult.Add(1.f);
	AuraDamageReduction.AddDefaulted();
	AuraSpeedMult.Add(1.f);
	ArchetypeIndex.AddDefaulted();
	Serial.Add(1);
	Flags.AddDefaulted();
	LinkGroup.Add(INDEX_NONE);
	TargetPlayer.Add(INDEX_NONE);
	Statuses.AddDefaulted();
	LastInstigator.AddDefaulted();
	return Index;
}

int32 UEclipseSwarmSubsystem::FindOrAddArchetypeIndex(UEclipseMobArchetype* Archetype)
{
	int32 Index = Archetypes.IndexOfByKey(Archetype);
	if (Index == INDEX_NONE)
	{
		Index = Archetypes.Add(Archetype);
		check(Index < 256); // Net snapshot packs archetype into a byte.
		if (NetProxy)
		{
			NetProxy->SetArchetypes(Archetypes);
		}
	}
	return Index;
}

FEclipseMobHandle UEclipseSwarmSubsystem::SpawnMob(UEclipseMobArchetype* Archetype, const FVector& Location)
{
	if (!HasAuthority() || !Archetype || NumAlive >= HardCap)
	{
		return FEclipseMobHandle();
	}

	const int32 I = AllocateSlot();
	Position[I] = FVector3f(Location);
	Velocity[I] = FVector2f::ZeroVector;
	Impulse[I] = FVector2f::ZeroVector;
	Health[I] = Archetype->MaxHealth;
	// Stagger first attacks so a freshly spawned pack doesn't hit on the same frame.
	AttackCooldown[I] = FMath::FRandRange(0.f, Archetype->AttackInterval);
	SimAccumulator[I] = 0.f;
	AuraDamageMult[I] = 1.f;
	AuraDamageReduction[I] = 0.f;
	AuraSpeedMult[I] = 1.f;
	ArchetypeIndex[I] = static_cast<uint16>(FindOrAddArchetypeIndex(Archetype));
	Flags[I] = MF_Alive;
	LinkGroup[I] = INDEX_NONE;
	TargetPlayer[I] = INDEX_NONE;
	Statuses[I].ClearAll();
	LastInstigator[I].Reset();

	++NumAlive;
	return MakeHandle(I);
}

void UEclipseSwarmSubsystem::DespawnMob(FEclipseMobHandle Handle)
{
	int32 I;
	if (HasAuthority() && ResolveHandle(Handle, I))
	{
		// Silent removal (no kill credit / loot): mark and let ProcessDeaths recycle.
		LastInstigator[I].Reset();
		Health[I] = 0.f;
		Flags[I] |= MF_PendingDeath | MF_Despawned;
		PendingDeaths.Add(I);
	}
}

bool UEclipseSwarmSubsystem::ResolveHandle(FEclipseMobHandle Handle, int32& OutIndex) const
{
	OutIndex = Handle.Index;
	return Serial.IsValidIndex(Handle.Index)
		&& Serial[Handle.Index] == static_cast<uint32>(Handle.Serial)
		&& (Flags[Handle.Index] & MF_Alive)
		&& !(Flags[Handle.Index] & MF_PendingDeath);
}

bool UEclipseSwarmSubsystem::IsAlive(FEclipseMobHandle Handle) const
{
	int32 I;
	return ResolveHandle(Handle, I);
}

bool UEclipseSwarmSubsystem::GetMobInfo(FEclipseMobHandle Handle, FEclipseMobInfo& Out) const
{
	int32 I;
	if (!ResolveHandle(Handle, I))
	{
		return false;
	}
	const UEclipseMobArchetype& A = Arch(I);
	Out.Handle = Handle;
	Out.Tier = A.Tier;
	Out.Location = FVector(Position[I]);
	Out.Health = Health[I];
	Out.MaxHealth = A.MaxHealth;
	Out.Archetype = Archetypes[ArchetypeIndex[I]];
	return true;
}

// =============================================================================
// Combat API
// =============================================================================

float UEclipseSwarmSubsystem::ApplyDamageInternal(int32 I, float Amount, float ArmorPen, EEclipseAffinity Affinity, AActor* Instigator)
{
	const UEclipseMobArchetype& A = Arch(I);

	float Outgoing = 1.f - AuraDamageReduction[I]; // Captain aura protection.
	if (const UEclipseFieldAffinitySubsystem* Field = GetWorld()->GetSubsystem<UEclipseFieldAffinitySubsystem>())
	{
		Outgoing *= Field->GetDamageScalar(FVector(Position[I]), Affinity); // Creation domain etc.
	}

	const float Final = Eclipse::ComputeFinalDamage(Amount, A.Armor, ArmorPen, Statuses[I].GetDamageTakenMultiplier(), Outgoing);
	if (Final <= 0.f)
	{
		return 0.f;
	}

	const float Dealt = FMath::Min(Final, Health[I]);
	Health[I] -= Final;
	if (Instigator)
	{
		LastInstigator[I] = Instigator;
	}
	if (Health[I] <= 0.f)
	{
		Flags[I] |= MF_PendingDeath; // Freed in ProcessDeaths, never mid-frame.
		PendingDeaths.Add(I);
	}
	return Dealt;
}

FEclipseMobDamageResult UEclipseSwarmSubsystem::ApplyDamageToMob(FEclipseMobHandle Handle, const FEclipseDamageSpec& Spec)
{
	FEclipseMobDamageResult Result;
	int32 I;
	if (!HasAuthority() || !ResolveHandle(Handle, I))
	{
		return Result;
	}

	AActor* Instigator = Spec.Instigator.Get();
	const UEclipseMobArchetype& A = Arch(I);

	if (Spec.bExecute && Eclipse::ShouldExecute(Health[I], A.MaxHealth, ToCore(A.Tier), Spec.ExecuteThreshold))
	{
		Result.DamageDealt = Health[I];
		Result.bExecuted = true;
		Health[I] = 0.f;
		LastInstigator[I] = Instigator;
		Flags[I] |= MF_PendingDeath;
		PendingDeaths.Add(I);
	}
	else
	{
		Result.DamageDealt = ApplyDamageInternal(I, Spec.Amount, Spec.ArmorPenetration, Spec.Affinity, Instigator);
	}
	Result.bKilled = (Flags[I] & MF_PendingDeath) != 0;

	// ---- Puppet Master's Thread mirroring -------------------------------------
	// Mirrored hits are flagged non-shareable, so A->B->A feedback is impossible.
	if (Spec.bCanBeShared && Result.DamageDealt > 0.f && LinkGroups.IsValidIndex(LinkGroup[I]))
	{
		const FDamageLinkGroup& Group = LinkGroups[LinkGroup[I]];
		const float Mirrored = Eclipse::ComputeLinkedDamageShare(Result.DamageDealt, Group.ShareFraction);
		for (const FEclipseMobHandle& Other : Group.Members)
		{
			int32 J;
			if (Other.Index != I && ResolveHandle(Other, J))
			{
				// Mirrored damage is pre-scaled by the share; armor of the receiver still applies.
				ApplyDamageInternal(J, Mirrored, Spec.ArmorPenetration, Spec.Affinity, Instigator);
			}
		}
	}
	return Result;
}

Eclipse::ApplyOutcome UEclipseSwarmSubsystem::ApplyStatusToMob(FEclipseMobHandle Handle, const FEclipseStatusSpec& Spec, AActor* Instigator)
{
	Eclipse::ApplyOutcome Outcome;
	int32 I;
	if (!HasAuthority() || !ResolveHandle(Handle, I) || !Spec.IsValid())
	{
		return Outcome;
	}

	// Swarm tiers never include Boss, so no stagger state is needed here.
	Eclipse::StatusTarget Target{ Statuses[I], ToCore(Arch(I).Tier) };
	Outcome = Eclipse::ApplyStatusEffect(Target, Spec.ToCore());

	if (Spec.Effect == EEclipseStatusEffect::Knockback && Outcome.Landed() && Instigator)
	{
		const FVector Dir = (FVector(Position[I]) - Instigator->GetActorLocation()).GetSafeNormal2D();
		ApplyImpulseToMob(Handle, Dir * Spec.Magnitude);
	}
	if (Instigator)
	{
		LastInstigator[I] = Instigator; // DoT kills credit the applier.
	}
	return Outcome;
}

void UEclipseSwarmSubsystem::ApplyImpulseToMob(FEclipseMobHandle Handle, const FVector& InImpulse)
{
	int32 I;
	if (HasAuthority() && ResolveHandle(Handle, I))
	{
		Impulse[I] += FVector2f(InImpulse.X, InImpulse.Y);
	}
}

int32 UEclipseSwarmSubsystem::CreateDamageLinkGroup(const TArray<FEclipseMobHandle>& Members, float ShareFraction, float Duration)
{
	if (!HasAuthority() || Members.Num() < 2)
	{
		return INDEX_NONE;
	}

	FDamageLinkGroup Group;
	Group.ShareFraction = FMath::Clamp(ShareFraction, 0.f, 1.f);
	Group.Remaining = Duration;
	for (const FEclipseMobHandle& H : Members)
	{
		int32 I;
		if (ResolveHandle(H, I))
		{
			LeaveLinkGroup(I);
			Group.Members.Add(H);
		}
	}
	if (Group.Members.Num() < 2)
	{
		return INDEX_NONE;
	}

	const int32 GroupId = LinkGroups.Add(MoveTemp(Group));
	for (const FEclipseMobHandle& H : LinkGroups[GroupId].Members)
	{
		LinkGroup[H.Index] = GroupId;
	}
	return GroupId;
}

void UEclipseSwarmSubsystem::LeaveLinkGroup(int32 I)
{
	const int32 GroupId = LinkGroup[I];
	LinkGroup[I] = INDEX_NONE;
	if (LinkGroups.IsValidIndex(GroupId))
	{
		LinkGroups[GroupId].Members.RemoveAllSwap([I](const FEclipseMobHandle& H) { return H.Index == I; });
		if (LinkGroups[GroupId].Members.Num() < 2)
		{
			for (const FEclipseMobHandle& H : LinkGroups[GroupId].Members)
			{
				LinkGroup[H.Index] = INDEX_NONE;
			}
			LinkGroups.RemoveAt(GroupId);
		}
	}
}

// =============================================================================
// Queries
// =============================================================================

int32 UEclipseSwarmSubsystem::QueryMobsInRadius(const FVector& Center, float Radius, TArray<FEclipseMobHandle>& Out) const
{
	const int32 Before = Out.Num();
	Grid.QueryRadius(Center.X, Center.Y, Radius, [&](const Eclipse::SpatialHashGrid2D::Entry& E, float)
	{
		const int32 I = static_cast<int32>(E.Index);
		// Grid was built at frame start; re-check liveness (a mob may have died since).
		if ((Flags[I] & MF_Alive) && !(Flags[I] & MF_PendingDeath))
		{
			Out.Add(MakeHandle(I));
		}
	});
	return Out.Num() - Before;
}

int32 UEclipseSwarmSubsystem::QueryMobsInCapsule(const FVector& Start, const FVector& End, float Radius, TArray<FEclipseMobHandle>& Out) const
{
	// Broad phase: circle bounding the segment. Narrow phase: point-segment distance.
	const FVector2f A(Start.X, Start.Y), B(End.X, End.Y);
	const FVector2f Mid = (A + B) * 0.5f;
	const float HalfLen = (B - A).Size() * 0.5f;
	const FVector2f AB = B - A;
	const float ABLenSq = FMath::Max(AB.SizeSquared(), KINDA_SMALL_NUMBER);

	const int32 Before = Out.Num();
	Grid.QueryRadius(Mid.X, Mid.Y, HalfLen + Radius, [&](const Eclipse::SpatialHashGrid2D::Entry& E, float)
	{
		const int32 I = static_cast<int32>(E.Index);
		if (!(Flags[I] & MF_Alive) || (Flags[I] & MF_PendingDeath))
		{
			return;
		}
		const FVector2f P(E.X, E.Y);
		const float T = FMath::Clamp(FVector2f::DotProduct(P - A, AB) / ABLenSq, 0.f, 1.f);
		const float R = Radius + Arch(I).CollisionRadius;
		if ((A + AB * T - P).SizeSquared() <= R * R)
		{
			Out.Add(MakeHandle(I));
		}
	});
	return Out.Num() - Before;
}

FEclipseMobHandle UEclipseSwarmSubsystem::FindNearestMobInCone(const FVector& Origin, const FVector& Forward, float Range, float HalfAngleDeg) const
{
	const FVector2f Fwd = FVector2f(Forward.X, Forward.Y).GetSafeNormal();
	const float CosHalf = FMath::Cos(FMath::DegreesToRadians(HalfAngleDeg));
	FEclipseMobHandle Best;
	float BestDistSq = TNumericLimits<float>::Max();

	Grid.QueryRadius(Origin.X, Origin.Y, Range, [&](const Eclipse::SpatialHashGrid2D::Entry& E, float DistSq)
	{
		const int32 I = static_cast<int32>(E.Index);
		if (!(Flags[I] & MF_Alive) || (Flags[I] & MF_PendingDeath) || DistSq >= BestDistSq)
		{
			return;
		}
		const FVector2f Dir = FVector2f(E.X - Origin.X, E.Y - Origin.Y).GetSafeNormal();
		if (DistSq < 1.f || FVector2f::DotProduct(Dir, Fwd) >= CosHalf)
		{
			BestDistSq = DistSq;
			Best = MakeHandle(I);
		}
	});
	return Best;
}

// =============================================================================
// Tick pipeline
// =============================================================================

void UEclipseSwarmSubsystem::Tick(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_SwarmTick);

	if (HasAuthority())
	{
		GatherPlayers();
		RebuildGrid();

		AuraAccumulator += DeltaTime;
		if (AuraAccumulator >= EclipseSwarm::AuraUpdateInterval)
		{
			AuraAccumulator = 0.f;
			UpdateAuras();
		}

		SimulateStatuses(DeltaTime);
		SimulateMovement(DeltaTime);
		ResolveAttacks(DeltaTime);
		UpdateLinkGroups(DeltaTime);
		ProcessDeaths();
		PushNetSnapshot(DeltaTime);

		SET_DWORD_STAT(STAT_SwarmAlive, NumAlive);
	}

	if (RenderHost)
	{
		UpdateRendering();
	}
}

void UEclipseSwarmSubsystem::GatherPlayers()
{
	Players.Reset();
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APawn* Pawn = It->Get() ? It->Get()->GetPawn() : nullptr;
		if (!Pawn)
		{
			continue;
		}
		FPlayerTarget& T = Players.AddDefaulted_GetRef();
		T.Actor = Pawn;
		T.ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn);
		const FVector L = Pawn->GetActorLocation();
		T.Position = FVector2f(L.X, L.Y);
	}
}

void UEclipseSwarmSubsystem::RebuildGrid()
{
	// The previous grid is about to be discarded: slots freed since it was built are now safe to reuse.
	FreeSlots.Append(DeferredFreeSlots);
	DeferredFreeSlots.Reset();

	Grid.Build(static_cast<Eclipse::u32>(Position.Num()), [this](Eclipse::u32 I, float& X, float& Y)
	{
		if (!(Flags[I] & MF_Alive) || (Flags[I] & MF_PendingDeath))
		{
			return false;
		}
		X = Position[I].X;
		Y = Position[I].Y;
		return true;
	});
}

void UEclipseSwarmSubsystem::UpdateAuras()
{
	for (int32 I = 0; I < Position.Num(); ++I)
	{
		AuraDamageMult[I] = 1.f;
		AuraDamageReduction[I] = 0.f;
		AuraSpeedMult[I] = 1.f;
	}

	for (int32 C = 0; C < Position.Num(); ++C)
	{
		if (!(Flags[C] & MF_Alive) || (Flags[C] & MF_PendingDeath) || !Arch(C).bProjectsAura)
		{
			continue;
		}
		const FEclipseEliteAura& Aura = Arch(C).Aura;
		Grid.QueryRadius(Position[C].X, Position[C].Y, Aura.Radius, [&](const Eclipse::SpatialHashGrid2D::Entry& E, float)
		{
			const int32 I = static_cast<int32>(E.Index);
			if (I == C || Arch(I).bProjectsAura)
			{
				return; // Captains don't buff each other (prevents elite-ball snowballing).
			}
			// Strongest aura wins per stat; auras never stack additively.
			AuraDamageMult[I] = FMath::Max(AuraDamageMult[I], 1.f + Aura.DamageBonus);
			AuraDamageReduction[I] = FMath::Max(AuraDamageReduction[I], Aura.DamageReduction);
			AuraSpeedMult[I] = FMath::Max(AuraSpeedMult[I], 1.f + Aura.MoveSpeedBonus);
		});
	}
}

void UEclipseSwarmSubsystem::SimulateStatuses(float DeltaTime)
{
	for (int32 I = 0; I < Position.Num(); ++I)
	{
		if (!(Flags[I] & MF_Alive) || (Flags[I] & MF_PendingDeath))
		{
			continue;
		}
		const float Dot = Statuses[I].Tick(DeltaTime);
		if (Dot > 0.f)
		{
			// DoTs are true damage (full penetration) and not shared through puppet links.
			ApplyDamageInternal(I, Dot, 1.f, EEclipseAffinity::Neutral, LastInstigator[I].Get());
		}
	}
}

void UEclipseSwarmSubsystem::SimulateMovement(float DeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_SwarmMovement);

	if (Players.Num() == 0)
	{
		return;
	}

	const int32 Num = Position.Num();
	const float Decay = FMath::Exp(-EclipseSwarm::ImpulseDamping * DeltaTime);

	// THREAD SAFETY: each iteration writes only row I. Neighbour reads go through
	// the grid's own position copies (built before this pass), never Position[J],
	// so there are no cross-row data races.
	ParallelFor(Num, [&](int32 I)
	{
		if (!(Flags[I] & MF_Alive) || (Flags[I] & MF_PendingDeath))
		{
			return;
		}
		const UEclipseMobArchetype& A = Arch(I);
		const FVector2f P(Position[I].X, Position[I].Y);

		// --- Target selection: nearest player (players are few; linear scan) ---
		int32 Best = INDEX_NONE;
		float BestDistSq = TNumericLimits<float>::Max();
		for (int32 PIdx = 0; PIdx < Players.Num(); ++PIdx)
		{
			const float D = (Players[PIdx].Position - P).SizeSquared();
			if (D < BestDistSq)
			{
				BestDistSq = D;
				Best = PIdx;
			}
		}
		TargetPlayer[I] = Best;

		// --- Distance LOD time slicing ---
		SimAccumulator[I] += DeltaTime;
		const float Interval = BestDistSq < EclipseSwarm::NearDistSq ? 0.f
			: (BestDistSq < EclipseSwarm::MidDistSq ? EclipseSwarm::MidInterval : EclipseSwarm::FarInterval);
		if (SimAccumulator[I] < Interval)
		{
			return;
		}
		const float StepDt = SimAccumulator[I];
		SimAccumulator[I] = 0.f;

		// --- Steering: seek + separation, scaled by status (stun/freeze => 0) ---
		const float SpeedScale = Statuses[I].GetMoveSpeedScale() * AuraSpeedMult[I];
		FVector2f Desired = FVector2f::ZeroVector;
		if (SpeedScale > 0.f)
		{
			const FVector2f ToTarget = Players[Best].Position - P;
			const float Dist = ToTarget.Size();
			if (Dist > A.AttackRange * 0.8f)
			{
				Desired = ToTarget / FMath::Max(Dist, 1.f);
			}

			FVector2f Separation = FVector2f::ZeroVector;
			int32 Neighbours = 0;
			const float SepRadius = A.CollisionRadius * 2.f;
			Grid.QueryRadius(P.X, P.Y, SepRadius, [&](const Eclipse::SpatialHashGrid2D::Entry& E, float D2)
			{
				if (static_cast<int32>(E.Index) == I || Neighbours >= EclipseSwarm::MaxSeparationNeighbours || D2 < 1.f)
				{
					return;
				}
				const float D = FMath::Sqrt(D2);
				Separation += (P - FVector2f(E.X, E.Y)) / D * (1.f - D / SepRadius);
				++Neighbours;
			});

			Desired = (Desired + Separation * EclipseSwarm::SeparationWeight).GetClampedToMaxSize(1.f) * (A.MoveSpeed * SpeedScale);
		}

		// Light smoothing avoids jitter when separation and seek fight each other.
		Velocity[I] = FMath::Lerp(Velocity[I], Desired, FMath::Clamp(StepDt * 10.f, 0.f, 1.f));
		Impulse[I] *= Decay;

		const FVector2f Move = (Velocity[I] + Impulse[I]) * StepDt;
		Position[I].X += Move.X;
		Position[I].Y += Move.Y;
		// Z: production samples a baked heightfield / flow-field here; omitted for brevity.
	}, Num < EclipseSwarm::ParallelThreshold ? EParallelForFlags::ForceSingleThread : EParallelForFlags::None);
}

void UEclipseSwarmSubsystem::ResolveAttacks(float DeltaTime)
{
	for (int32 I = 0; I < Position.Num(); ++I)
	{
		if (!(Flags[I] & MF_Alive) || (Flags[I] & MF_PendingDeath))
		{
			continue;
		}
		AttackCooldown[I] -= DeltaTime;
		const int32 T = TargetPlayer[I];
		if (AttackCooldown[I] > 0.f || !Players.IsValidIndex(T) || Statuses[I].IsDisabled())
		{
			continue;
		}
		const UEclipseMobArchetype& A = Arch(I);
		const float Reach = A.AttackRange + A.CollisionRadius;
		if ((Players[T].Position - FVector2f(Position[I].X, Position[I].Y)).SizeSquared() > Reach * Reach)
		{
			continue;
		}

		AttackCooldown[I] = A.AttackInterval;
		Players[T].PendingDamage += A.AttackDamage * AuraDamageMult[I];

		if (A.OnHitStatus.IsValid())
		{
			if (UEclipseStatusComponent* Status = Players[T].Actor.IsValid() ? Players[T].Actor->FindComponentByClass<UEclipseStatusComponent>() : nullptr)
			{
				Status->ApplyStatus(A.OnHitStatus, nullptr);
			}
		}
	}

	// Batch: one GE application per player per frame regardless of attacker count.
	for (FPlayerTarget& P : Players)
	{
		UAbilitySystemComponent* ASC = P.ASC.Get();
		if (P.PendingDamage <= 0.f || !ASC || !MobDamageEffect)
		{
			continue;
		}
		const FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(MobDamageEffect, 1.f, ASC->MakeEffectContext());
		if (Spec.IsValid())
		{
			Spec.Data->SetSetByCallerMagnitude(EclipseTags::Data_Damage, P.PendingDamage);
			Spec.Data->SetSetByCallerMagnitude(EclipseTags::Data_ArmorPenetration, 0.f);
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
		P.PendingDamage = 0.f;
	}
}

void UEclipseSwarmSubsystem::UpdateLinkGroups(float DeltaTime)
{
	TArray<int32, TInlineAllocator<16>> Expired;
	for (auto It = LinkGroups.CreateIterator(); It; ++It)
	{
		It->Remaining -= DeltaTime;
		if (It->Remaining <= 0.f)
		{
			Expired.Add(It.GetIndex());
		}
	}
	for (const int32 GroupId : Expired)
	{
		for (const FEclipseMobHandle& H : LinkGroups[GroupId].Members)
		{
			if (LinkGroup.IsValidIndex(H.Index) && LinkGroup[H.Index] == GroupId)
			{
				LinkGroup[H.Index] = INDEX_NONE;
			}
		}
		LinkGroups.RemoveAt(GroupId);
	}
}

void UEclipseSwarmSubsystem::ProcessDeaths()
{
	for (const int32 I : PendingDeaths)
	{
		if (!(Flags[I] & MF_Alive))
		{
			continue; // Already processed (e.g. killed twice in one frame).
		}

		FEclipseMobDeathInfo Info;
		Info.Mob.Handle = MakeHandle(I);
		Info.Mob.Tier = Arch(I).Tier;
		Info.Mob.Location = FVector(Position[I]);
		Info.Mob.MaxHealth = Arch(I).MaxHealth;
		Info.Mob.Archetype = Archetypes[ArchetypeIndex[I]];
		Info.Killer = LastInstigator[I];

		const bool bDespawned = (Flags[I] & MF_Despawned) != 0;
		LeaveLinkGroup(I);
		Flags[I] = 0;
		++Serial[I]; // Invalidates every outstanding handle to this slot.
		Statuses[I].ClearAll();
		DeferredFreeSlots.Add(I);
		--NumAlive;

		if (bDespawned)
		{
			continue;
		}
		OnMobKilled.Broadcast(Info); // Loot director, quests, ... (killer may be null for env kills).

		if (AActor* Killer = Info.Killer.Get())
		{
			// Ritual: OnEnemyKilled, delivered to the killer's ASC (runewords listen there).
			FGameplayEventData Payload;
			Payload.EventTag = EclipseTags::Event_Ritual_EnemyKilled;
			Payload.Instigator = Killer;
			Payload.EventMagnitude = static_cast<float>(Info.Mob.Tier);
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Killer, Payload.EventTag, Payload);
		}
	}
	PendingDeaths.Reset();
}

void UEclipseSwarmSubsystem::PushNetSnapshot(float DeltaTime)
{
	if (!NetProxy || GetWorld()->GetNetMode() == NM_Standalone)
	{
		return;
	}
	NetAccumulator += DeltaTime;
	const float Interval = 1.f / UEclipseGameSettings::Get()->SwarmNetUpdateHz;
	if (NetAccumulator < Interval)
	{
		return;
	}
	NetAccumulator = 0.f;

	NetProxy->BeginSnapshot();
	for (int32 I = 0; I < Position.Num(); ++I)
	{
		if ((Flags[I] & MF_Alive) && !(Flags[I] & MF_PendingDeath))
		{
			const FVector2f V = Velocity[I] + Impulse[I];
			const float Yaw = V.SizeSquared() > 1.f ? FMath::RadiansToDegrees(FMath::Atan2(V.Y, V.X)) : 0.f;
			NetProxy->WriteMob(MakeHandle(I).ToNetId(), FVector(Position[I]), Yaw,
				Health[I] / Arch(I).MaxHealth, ArchetypeIndex[I], static_cast<uint8>(Statuses[I].GetActiveMask() & 0xFF));
		}
	}
	NetProxy->EndSnapshot(); // Removes items not written this snapshot (dead mobs).
}

void UEclipseSwarmSubsystem::RegisterNetProxy(AEclipseSwarmNetProxy* Proxy)
{
	if (!HasAuthority())
	{
		NetProxy = Proxy; // Clients render from this.
	}
}

// =============================================================================
// Rendering
// =============================================================================

void UEclipseSwarmSubsystem::UpdateRendering()
{
	SCOPE_CYCLE_COUNTER(STAT_SwarmRender);

	const bool bFromSim = HasAuthority();

	// Archetype table: authoritative on the server, replicated on clients.
	if (!bFromSim && NetProxy)
	{
		const TArray<TObjectPtr<UEclipseMobArchetype>>& Remote = NetProxy->GetArchetypes();
		for (int32 K = Archetypes.Num(); K < Remote.Num(); ++K)
		{
			Archetypes.Add(Remote[K]);
		}
	}

	RenderScratch.SetNum(Archetypes.Num());
	for (TArray<FTransform>& List : RenderScratch)
	{
		List.Reset();
	}

	auto Emit = [this](int32 ArchIndex, const FVector& Loc, float Yaw)
	{
		if (RenderScratch.IsValidIndex(ArchIndex) && Archetypes[ArchIndex])
		{
			const float S = Archetypes[ArchIndex]->MeshScale;
			RenderScratch[ArchIndex].Emplace(FRotator(0.f, Yaw, 0.f), Loc, FVector(S));
		}
	};

	if (bFromSim)
	{
		for (int32 I = 0; I < Position.Num(); ++I)
		{
			if ((Flags[I] & MF_Alive) && !(Flags[I] & MF_PendingDeath))
			{
				const FVector2f V = Velocity[I];
				Emit(ArchetypeIndex[I], FVector(Position[I]), V.SizeSquared() > 1.f ? FMath::RadiansToDegrees(FMath::Atan2(V.Y, V.X)) : 0.f);
			}
		}
	}
	else if (NetProxy)
	{
		NetProxy->ForEachRenderable([&Emit](int32 ArchIndex, const FVector& Loc, float Yaw) { Emit(ArchIndex, Loc, Yaw); });
	}

	RenderISMs.SetNum(Archetypes.Num());
	for (int32 K = 0; K < Archetypes.Num(); ++K)
	{
		UEclipseMobArchetype* A = Archetypes[K];
		if (!A || !A->Mesh)
		{
			continue;
		}
		TObjectPtr<UInstancedStaticMeshComponent>& ISM = RenderISMs[K];
		if (!ISM)
		{
			ISM = NewObject<UInstancedStaticMeshComponent>(RenderHost);
			ISM->SetStaticMesh(A->Mesh);
			ISM->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Gameplay uses the grid, not physics.
			ISM->SetCanEverAffectNavigation(false);
			ISM->SetMobility(EComponentMobility::Movable);
			ISM->SetupAttachment(RenderHost->GetRootComponent());
			ISM->RegisterComponent();
		}

		// Grow-only instance pool: unused instances are collapsed to zero scale,
		// avoiding RemoveInstance() which reshuffles indices and reallocates GPU buffers.
		TArray<FTransform>& Transforms = RenderScratch[K];
		const int32 Existing = ISM->GetInstanceCount();
		if (Transforms.Num() > Existing)
		{
			TArray<FTransform> Extra;
			Extra.Init(FTransform(FQuat::Identity, FVector::ZeroVector, FVector::ZeroVector), Transforms.Num() - Existing);
			ISM->AddInstances(Extra, false, true);
		}
		while (Transforms.Num() < ISM->GetInstanceCount())
		{
			Transforms.Emplace(FQuat::Identity, FVector::ZeroVector, FVector::ZeroVector);
		}
		if (Transforms.Num() > 0)
		{
			ISM->BatchUpdateInstancesTransforms(0, Transforms, /*bWorldSpace*/ true, /*bMarkRenderStateDirty*/ true, /*bTeleport*/ true);
		}
	}
}
