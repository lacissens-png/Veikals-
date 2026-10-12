// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "EclipseCore/StatusEffects.h"

#include "EclipseStaggerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEclipseOnStaggerChanged, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEclipseOnStaggerEvent);

/** Designer-facing subset of Eclipse::StaggerConfig. */
USTRUCT(BlueprintType)
struct FEclipseStaggerTuning
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Stagger", meta = (ClampMin = "1"))
	float BaseMaxStagger = 100.f;

	UPROPERTY(EditAnywhere, Category = "Stagger", meta = (ClampMin = "0"))
	float MaxGrowthPerBreak = 0.25f;

	/** N seconds the boss is stunned & vulnerable after a break. */
	UPROPERTY(EditAnywhere, Category = "Stagger", meta = (ClampMin = "0.5"))
	float StaggeredDuration = 5.f;

	UPROPERTY(EditAnywhere, Category = "Stagger", meta = (ClampMin = "0.2"))
	float StaggeredVulnerableBonus = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Stagger", meta = (ClampMin = "0"))
	float DecayDelay = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Stagger", meta = (ClampMin = "0"))
	float DecayPerSecond = 8.f;

	UPROPERTY(EditAnywhere, Category = "Stagger", meta = (ClampMin = "0", ClampMax = "1"))
	float UnstoppableFillScale = 0.5f;

	Eclipse::StaggerConfig ToCore() const;
};

/**
 * Boss Stagger Meter.
 *
 * Authority: the meter only changes on the server (via UEclipseStatusComponent,
 * which routes all CC through Eclipse::ApplyStatusEffect). Clients receive the
 * three replicated values below for the HUD bar and break VFX; they never run
 * stagger math, so there is nothing to desync or cheat.
 */
UCLASS(ClassGroup = (Eclipse), meta = (BlueprintSpawnableComponent))
class ECLIPSEGAME_API UEclipseStaggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEclipseStaggerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// --- Server API used by the status pipeline --------------------------------
	Eclipse::StaggerState& GetCoreState() { return State; }
	const Eclipse::StaggerConfig& GetCoreConfig() const { return CoreConfig; }

	/** Call after ApplyStatusEffect touched the meter: syncs replicated values and fires events. */
	void NotifyMeterChanged(const Eclipse::ApplyOutcome& Outcome);

	// --- Read API (server + clients) -------------------------------------------
	UFUNCTION(BlueprintPure, Category = "Stagger")
	float GetStaggerPercent() const { return ReplicatedMax > 0.f ? ReplicatedCurrent / ReplicatedMax : 0.f; }

	UFUNCTION(BlueprintPure, Category = "Stagger")
	bool IsStaggered() const { return bReplicatedStaggered; }

	UPROPERTY(BlueprintAssignable) FEclipseOnStaggerChanged OnStaggerChanged;
	UPROPERTY(BlueprintAssignable) FEclipseOnStaggerEvent OnStaggerBroken;
	UPROPERTY(BlueprintAssignable) FEclipseOnStaggerEvent OnStaggerRecovered;

protected:
	UPROPERTY(EditAnywhere, Category = "Stagger")
	FEclipseStaggerTuning Tuning;

	UFUNCTION() void OnRep_Stagger();
	UFUNCTION() void OnRep_Staggered();

private:
	void SyncReplicatedValues();

	UPROPERTY(ReplicatedUsing = OnRep_Stagger) float ReplicatedCurrent = 0.f;
	UPROPERTY(ReplicatedUsing = OnRep_Stagger) float ReplicatedMax = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_Staggered) bool bReplicatedStaggered = false;

	Eclipse::StaggerState State;
	Eclipse::StaggerConfig CoreConfig;
};
