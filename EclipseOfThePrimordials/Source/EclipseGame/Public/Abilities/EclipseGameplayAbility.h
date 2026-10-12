// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "Combat/EclipseCombatStatics.h"
#include "Combat/EclipseCombatTypes.h"
#include "CoreMinimal.h"

#include "EclipseGameplayAbility.generated.h"

class UEclipseSwarmSubsystem;
class AEclipseGroundZone;

/**
 * Base for every class ability.
 *
 * ACTIVATION
 *   ActivateAbility -> CommitAbility (resource + cooldown) -> raise UltimateUsed
 *   ritual (if ultimate) -> OnAbilityCommitted() [subclass logic].
 *
 * NETWORKING
 *   LocalPredicted: the owning client activates immediately for responsiveness
 *   (animation montages, cues). ALL gameplay outcomes - damage, statuses,
 *   summons, swarm queries - go through helpers that early-out unless
 *   HasAuthority, so prediction can never desync or fabricate damage.
 *
 * RESOURCE MODELS (see EEclipseResourceModel)
 *   Builder/Spender: cost is subtracted; Strain: cost is ADDED and casting is
 *   refused if it would exceed MaxClassResource.
 *
 * RUNEWORD-TRIGGERED
 *   When the runeword component triggers a spec it granted (SourceObject is the
 *   component), cost and cooldown are skipped and EventMagnitude carries the
 *   rune's parameter (e.g. Time Stop duration).
 */
UCLASS(Abstract)
class ECLIPSEGAME_API UEclipseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEclipseGameplayAbility();

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	/** Subclass entry point; called on every machine that activated. Must eventually call FinishAbility(). */
	virtual void OnAbilityCommitted() { FinishAbility(); }

	// ---- Tunables -------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category = "Eclipse|Cost", meta = (ClampMin = "0"))
	float ResourceCost = 0.f;

	/** Builder classes: resource gained on cast (e.g. Seismic Pressure per slam). */
	UPROPERTY(EditDefaultsOnly, Category = "Eclipse|Cost", meta = (ClampMin = "0"))
	float ResourceGeneratedOnCast = 0.f;

	/** Instant GE: ClassResource += SetByCaller(Data.ResourceDelta). */
	UPROPERTY(EditDefaultsOnly, Category = "Eclipse|Cost")
	TSubclassOf<UGameplayEffect> ResourceDeltaEffect;

	/** Instant GE with UEclipseDamageExecution. */
	UPROPERTY(EditDefaultsOnly, Category = "Eclipse|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Eclipse|Damage")
	EEclipseAffinity Affinity = EEclipseAffinity::Neutral;

	/** Super-Tier Ultimate: raises the OnUltimateUsed ritual. */
	UPROPERTY(EditDefaultsOnly, Category = "Eclipse")
	bool bIsUltimate = false;

	// ---- Helpers (server-gated) ----------------------------------------------
	bool IsServer() const;
	bool IsRunewordTriggered() const { return bRunewordTriggered; }
	/** Runeword parameter if present (>0), else Default. */
	float GetParamOr(float Default) const { return (bRunewordTriggered && EventParam > 0.f) ? EventParam : Default; }

	FVector GetAvatarLocation() const;
	FVector GetAvatarForward() const;
	UEclipseSwarmSubsystem* GetSwarm() const;

	FEclipseAreaHitParams MakeHitParams(float Damage, float ArmorPen = 0.f, const FEclipseStatusSpec* Status = nullptr) const;

	/** Applies ClassResource delta through GAS (predicted on the owner). */
	void ModifyResource(float Delta) const;

	/** Server: spawns a ground zone (obelisk / scorch) configured for this caster. */
	AEclipseGroundZone* SpawnGroundZone(TSubclassOf<AEclipseGroundZone> ZoneClass, const FVector& Location, float DamagePerPulse,
		const FEclipseStatusSpec& Status, float Lifetime) const;

	/** Server: spawns summons around the caster (Dark Young, Flesh Colossus). */
	int32 SpawnSummons(TSubclassOf<AActor> SummonClass, int32 Count, float Radius, float Lifetime) const;

	void SendRitualEvent(const FGameplayTag& Tag, float Magnitude) const;

	void FinishAbility(bool bCancelled = false);

private:
	static bool IsRunewordSpec(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo);

	bool bRunewordTriggered = false;
	float EventParam = 0.f;
};
