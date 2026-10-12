// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Combat/EclipseCombatTypes.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "EclipseCore/StatusEffects.h"

#include "EclipseStatusComponent.generated.h"

class UEclipseStaggerComponent;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEclipseOnStatusMaskChanged, int32, ActiveMask);

/**
 * Status effects for full Actors (heroes, bosses). Swarm mobs keep their
 * Eclipse::StatusContainer inline in the swarm SoA instead.
 *
 * AUTHORITY
 *   ApplyStatus() is server-only. Clients receive:
 *     - ActiveMask   (16 bits)  -> loose GAS tags (Status.*) for anims / UI / ability blocking
 *     - MoveSpeedScale (float)  -> so client movement prediction matches the server
 *   Remaining durations are not replicated (UI shows icons, not timers) which keeps
 *   this component to ~6 bytes per change on the wire.
 *
 * BOSSES
 *   If the owner's Tier is Boss, a UEclipseStaggerComponent must exist on the
 *   actor; CC is routed into it by the core pipeline and never disables the boss
 *   until the meter breaks.
 */
UCLASS(ClassGroup = (Eclipse), meta = (BlueprintSpawnableComponent))
class ECLIPSEGAME_API UEclipseStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEclipseStatusComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Server only. Runs the unified pipeline (immunity, tier scaling, stagger).
	 * @return the core outcome; Outcome.Landed() tells callers whether to raise
	 *         the CrowdControlApplied ritual for the instigator.
	 */
	Eclipse::ApplyOutcome ApplyStatus(const FEclipseStatusSpec& Spec, AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Status", meta = (DisplayName = "Apply Status"))
	bool K2_ApplyStatus(const FEclipseStatusSpec& Spec, AActor* Instigator) { return ApplyStatus(Spec, Instigator).Landed(); }

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Status")
	void RemoveStatus(EEclipseStatusEffect Effect);

	UFUNCTION(BlueprintPure, Category = "Status")
	bool HasStatus(EEclipseStatusEffect Effect) const { return (ActiveMask & (1 << static_cast<int32>(Effect))) != 0; }

	/** 1.0 + Vulnerable bonus. Server only (damage is server-only); clients always read 1.0. */
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetDamageTakenMultiplier() const { return Statuses.GetDamageTakenMultiplier(); }

	UFUNCTION(BlueprintPure, Category = "Status")
	EEclipseMobTier GetTier() const { return Tier; }

	/** Constructor-time configuration (e.g. bosses). */
	void SetTier(EEclipseMobTier InTier) { Tier = InTier; }

	UPROPERTY(BlueprintAssignable) FEclipseOnStatusMaskChanged OnStatusMaskChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status")
	EEclipseMobTier Tier = EEclipseMobTier::Player;

	/** Instant damage GE used for DoT ticks (Data.Damage, full armor penetration). */
	UPROPERTY(EditAnywhere, Category = "Status")
	TSubclassOf<UGameplayEffect> DotDamageEffect;

	/** DoTs are applied in batches to avoid a GE spec per frame. */
	UPROPERTY(EditAnywhere, Category = "Status", meta = (ClampMin = "0.05"))
	float DotApplyInterval = 0.5f;

	UFUNCTION() void OnRep_ActiveMask();
	UFUNCTION() void OnRep_MoveSpeedScale();

private:
	void PostChangeSync();
	void ApplyLooseTags(uint16 OldMask, uint16 NewMask);

	/** Mask whose side effects (loose tags) have been applied on THIS machine. */
	uint16 AppliedMask = 0;
	void ApplyMovementScale();
	void ApplyKnockback(const FEclipseStatusSpec& Spec, AActor* Instigator);
	void FlushDotDamage();

	Eclipse::StatusContainer Statuses;

	UPROPERTY(Transient)
	TObjectPtr<UEclipseStaggerComponent> Stagger;

	UPROPERTY(ReplicatedUsing = OnRep_ActiveMask)
	uint16 ActiveMask = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MoveSpeedScale)
	float MoveSpeedScale = 1.f;

	float BaseMaxWalkSpeed = 0.f;
	float PendingDotDamage = 0.f;
	float DotAccumulator = 0.f;
	TWeakObjectPtr<AActor> LastDotInstigator;
};
