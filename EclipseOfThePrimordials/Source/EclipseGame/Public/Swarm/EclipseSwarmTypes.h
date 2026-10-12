// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Combat/EclipseCombatTypes.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "EclipseSwarmTypes.generated.h"

class AActor;
class UStaticMesh;

/**
 * Generational handle into the swarm's SoA arrays. Slots are recycled; the
 * Serial makes a stale handle (mob died, slot reused) resolve to "invalid"
 * instead of silently hitting a different mob.
 */
USTRUCT(BlueprintType)
struct FEclipseMobHandle
{
	GENERATED_BODY()

	UPROPERTY() int32 Index = INDEX_NONE;
	UPROPERTY() int32 Serial = 0;

	bool IsSet() const { return Index != INDEX_NONE; }
	bool operator==(const FEclipseMobHandle& O) const { return Index == O.Index && Serial == O.Serial; }
	bool operator!=(const FEclipseMobHandle& O) const { return !(*this == O); }

	/** Compact id used on the wire (index in low 16 bits, serial in high 16). */
	int32 ToNetId() const { return (Serial & 0xFFFF) << 16 | (Index & 0xFFFF); }

	friend uint32 GetTypeHash(const FEclipseMobHandle& H) { return HashCombine(::GetTypeHash(H.Index), ::GetTypeHash(H.Serial)); }
};

/** Captain/Elite aura: buffs nearby non-captain swarmers. Strongest aura wins (no stacking). */
USTRUCT(BlueprintType)
struct FEclipseEliteAura
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura", meta = (ClampMin = "0"))
	float Radius = 900.f;

	/** +X outgoing damage for buffed mobs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura", meta = (ClampMin = "0"))
	float DamageBonus = 0.25f;

	/** X fraction of incoming damage removed for buffed mobs. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura", meta = (ClampMin = "0", ClampMax = "0.9"))
	float DamageReduction = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura", meta = (ClampMin = "0"))
	float MoveSpeedBonus = 0.15f;
};

/**
 * Static definition of a mob type. Shared by every instance (flyweight): the
 * per-mob SoA rows only store an archetype index, so 500 Ghouls cost one asset.
 */
UCLASS(BlueprintType)
class ECLIPSEGAME_API UEclipseMobArchetype : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tier")
	EEclipseMobTier Tier = EEclipseMobTier::TrashSwarmer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "1"))
	float MaxHealth = 60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "0"))
	float Armor = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "0"))
	float MoveSpeed = 420.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (ClampMin = "10"))
	float CollisionRadius = 45.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0"))
	float AttackRange = 140.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0"))
	float AttackDamage = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (ClampMin = "0.1"))
	float AttackInterval = 1.2f;

	/** Optional status applied to the player on hit (e.g. Bleed for flesh-horrors). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	FEclipseStatusSpec OnHitStatus;

	/** Captains/Elites only. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura")
	bool bProjectsAura = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura", meta = (EditCondition = "bProjectsAura"))
	FEclipseEliteAura Aura;

	/** Rendered via one ISM per archetype. Use a Vertex-Animation-Texture material for animation. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	float MeshScale = 1.f;

	/** Loot table row name consumed by the (server) loot director on death. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
	FName LootTableRow;
};

USTRUCT(BlueprintType)
struct FEclipseMobDamageResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Damage") float DamageDealt = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Damage") bool bKilled = false;
	UPROPERTY(BlueprintReadOnly, Category = "Damage") bool bExecuted = false;
};

USTRUCT(BlueprintType)
struct FEclipseMobInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Mob") FEclipseMobHandle Handle;
	UPROPERTY(BlueprintReadOnly, Category = "Mob") EEclipseMobTier Tier = EEclipseMobTier::TrashSwarmer;
	UPROPERTY(BlueprintReadOnly, Category = "Mob") FVector Location = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Mob") float Health = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Mob") float MaxHealth = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Mob") TObjectPtr<UEclipseMobArchetype> Archetype = nullptr;
};

USTRUCT(BlueprintType)
struct FEclipseMobDeathInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Mob") FEclipseMobInfo Mob;
	/** Weak: the killer may have left the game before listeners run. */
	UPROPERTY() TWeakObjectPtr<AActor> Killer;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FEclipseOnMobKilled, const FEclipseMobDeathInfo&);
