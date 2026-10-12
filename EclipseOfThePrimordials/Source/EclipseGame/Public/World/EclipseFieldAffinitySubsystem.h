// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Combat/EclipseCombatTypes.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "EclipseFieldAffinitySubsystem.generated.h"

class AEclipseCreationDomain;

/**
 * Environment "field affinity" layer.
 *
 * Each zone has a base affinity (set by the level / encounter director). Damage
 * whose affinity RESONATES with the local field is amplified; OPPOSED affinities
 * are dampened. Eclipse Puppeteer's ultimate "Creation" spawns a domain that
 * OVERRIDES the field inside its radius.
 *
 * Runs identically on server and clients (domains are replicated actors that
 * register themselves), so UI can preview modifiers. Only the server's answer
 * affects damage.
 */
UCLASS()
class ECLIPSEGAME_API UEclipseFieldAffinitySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Affinity")
	void SetBaseAffinity(EEclipseAffinity InAffinity) { BaseAffinity = InAffinity; }

	void RegisterDomain(AEclipseCreationDomain* Domain);
	void UnregisterDomain(AEclipseCreationDomain* Domain);

	/** Strongest (most recent) domain covering Location wins; else the base field. */
	UFUNCTION(BlueprintPure, Category = "Affinity")
	EEclipseAffinity ResolveAffinityAt(const FVector& Location) const;

	/** Multiplier for damage of DamageAffinity landing at Location. */
	UFUNCTION(BlueprintPure, Category = "Affinity")
	float GetDamageScalar(const FVector& Location, EEclipseAffinity DamageAffinity) const;

	static bool AreOpposed(EEclipseAffinity A, EEclipseAffinity B);

private:
	const AEclipseCreationDomain* FindDomainAt(const FVector& Location) const;

	EEclipseAffinity BaseAffinity = EEclipseAffinity::Neutral;
	TArray<TWeakObjectPtr<AEclipseCreationDomain>> Domains; // Few (0-4); linear scan is fastest.
};
