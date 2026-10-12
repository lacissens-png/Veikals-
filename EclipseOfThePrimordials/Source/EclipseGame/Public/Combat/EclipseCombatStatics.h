// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// One choke point for "hit everything in an area": swarm mobs (SoA path) AND
// GAS actors (bosses, via GE_Damage). Abilities, ground zones and rune effects
// all go through here, which guarantees:
//   * identical damage math on both paths,
//   * server-only execution (every function early-outs on clients),
//   * the CrowdControlApplied ritual fires exactly once per application batch.

#pragma once

#include "Combat/EclipseCombatTypes.h"
#include "CoreMinimal.h"
#include "Swarm/EclipseSwarmTypes.h"
#include "Templates/SubclassOf.h"

#include "EclipseCombatStatics.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FEclipseAreaHitResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Hit") int32 MobsHit = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Hit") int32 MobsKilled = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Hit") int32 ActorsHit = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Hit") int32 CrowdControlLanded = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Hit") float TotalDamage = 0.f;
};

/** Everything an area hit needs; built by the caller once per cast. */
struct FEclipseAreaHitParams
{
	UAbilitySystemComponent* SourceASC = nullptr;   // Instigator's ASC (for GE context).
	TSubclassOf<UGameplayEffect> DamageEffect;       // GE using UEclipseDamageExecution.
	FEclipseDamageSpec Damage;                       // Amount 0 => status-only hit.
	const FEclipseStatusSpec* Status = nullptr;      // Optional.
	bool bIncludeActors = true;                      // Bosses / other ASC actors.
};

namespace EclipseCombat
{
	ECLIPSEGAME_API bool IsHostile(const AActor* Source, const AActor* Target);

	/** Damages one ASC actor through GAS. Returns false if not applicable. */
	ECLIPSEGAME_API bool ApplyDamageToActor(const FEclipseAreaHitParams& Params, AActor* Target);

	/** Applies a status to one actor with a UEclipseStatusComponent. */
	ECLIPSEGAME_API Eclipse::ApplyOutcome ApplyStatusToActor(AActor* Target, const FEclipseStatusSpec& Spec, AActor* Instigator);

	ECLIPSEGAME_API void GatherHostileActorsInRadius(const UObject* WorldContext, const FVector& Center, float Radius,
		const AActor* Source, TArray<AActor*>& Out);

	ECLIPSEGAME_API FEclipseAreaHitResult HitMobs(const UObject* WorldContext, const TArray<FEclipseMobHandle>& Mobs,
		const FEclipseAreaHitParams& Params);

	ECLIPSEGAME_API FEclipseAreaHitResult HitSphere(const UObject* WorldContext, const FVector& Center, float Radius,
		const FEclipseAreaHitParams& Params);

	ECLIPSEGAME_API FEclipseAreaHitResult HitLine(const UObject* WorldContext, const FVector& Start, const FVector& End,
		float HalfWidth, const FEclipseAreaHitParams& Params);

	/** Raises Event.Ritual.CrowdControlApplied on the instigator (Magnitude = targets affected). */
	ECLIPSEGAME_API void NotifyCrowdControlLanded(AActor* Instigator, int32 Count);
}
