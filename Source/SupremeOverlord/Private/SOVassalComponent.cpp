#include "SOVassalComponent.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "SOCharacter.h"
#include "SOExperienceComponent.h"
#include "SOManaComponent.h"
#include "SOVassalActor.h"
#include "SOVassalData.h"

USOVassalComponent::USOVassalComponent()
{
	PrimaryComponentTick.bCanEverTick          = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval          = 0.1f;
}

void USOVassalComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SummonCooldownRemaining > 0.0f)
	{
		SummonCooldownRemaining = FMath::Max(0.0f, SummonCooldownRemaining - DeltaTime);
	}
}

bool USOVassalComponent::RecruitVassal(USOVassalData* Vassal)
{
	if (!Vassal || RecruitedVassals.Contains(Vassal))
	{
		return false;
	}

	RecruitedVassals.Add(Vassal);
	OnVassalRecruited.Broadcast(Vassal);
	return true;
}

bool USOVassalComponent::IsVassalRecruited(USOVassalData* Vassal) const
{
	return Vassal && RecruitedVassals.Contains(Vassal);
}

void USOVassalComponent::RestoreRecruitedVassals(const TArray<USOVassalData*>& Vassals)
{
	RecruitedVassals.Reset();
	for (USOVassalData* Vassal : Vassals)
	{
		if (Vassal)
		{
			RecruitedVassals.Add(Vassal);
		}
	}
	CurrentSelectionIndex = 0;
}

void USOVassalComponent::CycleVassalSelection()
{
	if (RecruitedVassals.Num() <= 1)
	{
		return;
	}
	CurrentSelectionIndex = (CurrentSelectionIndex + 1) % RecruitedVassals.Num();
}

USOVassalData* USOVassalComponent::GetSelectedVassal() const
{
	return RecruitedVassals.IsValidIndex(CurrentSelectionIndex) ? RecruitedVassals[CurrentSelectionIndex] : nullptr;
}

bool USOVassalComponent::SummonSelectedVassal()
{
	if (SummonCooldownRemaining > 0.0f)
	{
		return false;
	}

	USOVassalData* Data = GetSelectedVassal();
	if (!Data || !Data->ActorClass)
	{
		return false;
	}

	ASOCharacter* Owner = Cast<ASOCharacter>(GetOwner());
	if (!Owner || !Owner->IsAlive())
	{
		return false;
	}

	if (Data->RequiredLevel > 1 && Owner->ExperienceComponent
	    && Owner->ExperienceComponent->GetCurrentLevel() < Data->RequiredLevel)
	{
		return false;
	}

	if (ManaCostPerSummon > 0.0f && (!Owner->ManaComponent || !Owner->ManaComponent->HasEnough(ManaCostPerSummon)))
	{
		return false;
	}

	DismissVassal();

	if (ManaCostPerSummon > 0.0f)
	{
		Owner->ManaComponent->Consume(ManaCostPerSummon);
	}

	FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 150.0f;
	if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation NavLoc;
		if (NavSys->GetRandomPointInNavigableRadius(SpawnLocation, 150.0f, NavLoc))
		{
			SpawnLocation = NavLoc.Location;
		}
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ASOVassalActor* Spawned = GetWorld()->SpawnActor<ASOVassalActor>(Data->ActorClass, SpawnLocation, FRotator::ZeroRotator, Params);
	if (!Spawned)
	{
		return false;
	}

	Spawned->OwnerVassalComponent = this;
	Spawned->InitializeFromData(Data);

	ActiveVassalData        = Data;
	ActiveVassalActor       = Spawned;
	SummonCooldownRemaining = SummonCooldown;

	if (SummonSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SummonSFX, SpawnLocation);
	}

	OnVassalSummoned.Broadcast(Data);
	return true;
}

void USOVassalComponent::DismissVassal()
{
	if (ASOVassalActor* Vassal = ActiveVassalActor.Get())
	{
		Vassal->Destroy();
	}

	ActiveVassalActor.Reset();
	ActiveVassalData = nullptr;
	OnVassalDismissed.Broadcast();
}

bool USOVassalComponent::HasActiveVassal() const
{
	return ActiveVassalActor.IsValid() && ActiveVassalActor->IsAlive();
}

void USOVassalComponent::HandleVassalDied(ASOVassalActor* Vassal)
{
	if (ActiveVassalActor.Get() == Vassal)
	{
		ActiveVassalActor.Reset();
		ActiveVassalData = nullptr;
		OnVassalDismissed.Broadcast();
	}
}

float USOVassalComponent::GetIncomingDamageMultiplier() const
{
	if (HasActiveVassal() && ActiveVassalData && ActiveVassalData->BuffType == ESOVassalBuffType::DamageReduction)
	{
		return FMath::Clamp(1.0f - ActiveVassalData->BuffMagnitude, 0.1f, 1.0f);
	}
	return 1.0f;
}

float USOVassalComponent::GetAttackCooldownMultiplier() const
{
	if (HasActiveVassal() && ActiveVassalData && ActiveVassalData->BuffType == ESOVassalBuffType::AttackSpeed)
	{
		return FMath::Clamp(1.0f - ActiveVassalData->BuffMagnitude, 0.1f, 1.0f);
	}
	return 1.0f;
}
