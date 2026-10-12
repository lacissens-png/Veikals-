// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Combat/EclipseStaggerComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EclipseGameplayTags.h"
#include "Net/UnrealNetwork.h"

Eclipse::StaggerConfig FEclipseStaggerTuning::ToCore() const
{
	Eclipse::StaggerConfig C; // Keeps core defaults for the per-CC fill table.
	C.BaseMaxStagger = BaseMaxStagger;
	C.MaxGrowthPerBreak = MaxGrowthPerBreak;
	C.StaggeredDuration = StaggeredDuration;
	C.StaggeredVulnerableBonus = FMath::Max(StaggeredVulnerableBonus, 0.2f);
	C.DecayDelay = DecayDelay;
	C.DecayPerSecond = DecayPerSecond;
	C.UnstoppableFillScale = UnstoppableFillScale;
	return C;
}

UEclipseStaggerComponent::UEclipseStaggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f; // Decay is slow; 10 Hz is plenty.
	SetIsReplicatedByDefault(true);
}

void UEclipseStaggerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEclipseStaggerComponent, ReplicatedCurrent);
	DOREPLIFETIME(UEclipseStaggerComponent, ReplicatedMax);
	DOREPLIFETIME(UEclipseStaggerComponent, bReplicatedStaggered);
}

void UEclipseStaggerComponent::BeginPlay()
{
	Super::BeginPlay();
	CoreConfig = Tuning.ToCore();

	// Only the server simulates the meter.
	SetComponentTickEnabled(GetOwner() && GetOwner()->HasAuthority());
	SyncReplicatedValues();
}

void UEclipseStaggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float Before = State.Current;
	const bool bRecovered = Eclipse::TickStagger(State, CoreConfig, DeltaTime);

	if (bRecovered)
	{
		SyncReplicatedValues();
		OnStaggerRecovered.Broadcast();
	}
	else if (!FMath::IsNearlyEqual(Before, State.Current))
	{
		SyncReplicatedValues();
	}
}

void UEclipseStaggerComponent::NotifyMeterChanged(const Eclipse::ApplyOutcome& Outcome)
{
	SyncReplicatedValues();

	if (Outcome.Result == Eclipse::EApplyResult::StaggerBroken)
	{
		OnStaggerBroken.Broadcast();

		// Cosmetic burst for every client via GameplayCue (unreliable multicast, no state).
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
		{
			ASC->ExecuteGameplayCue(EclipseTags::GameplayCue_Stagger_Break);
		}
	}
}

void UEclipseStaggerComponent::SyncReplicatedValues()
{
	ReplicatedCurrent = State.Current;
	ReplicatedMax = State.GetMax(CoreConfig);

	const bool bWasStaggered = bReplicatedStaggered;
	bReplicatedStaggered = State.IsStaggered();

	OnStaggerChanged.Broadcast(ReplicatedCurrent, ReplicatedMax);
	if (bWasStaggered != bReplicatedStaggered)
	{
		OnRep_Staggered(); // Server applies the same tag bookkeeping clients do.
	}
}

void UEclipseStaggerComponent::OnRep_Stagger()
{
	OnStaggerChanged.Broadcast(ReplicatedCurrent, ReplicatedMax);
}

void UEclipseStaggerComponent::OnRep_Staggered()
{
	// State.Staggered is a loose tag maintained identically on server and clients
	// (derived from replicated state), so AnimBPs / ability blocking work everywhere.
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->SetLooseGameplayTagCount(EclipseTags::State_Staggered, bReplicatedStaggered ? 1 : 0);
	}
}
