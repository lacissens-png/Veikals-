#include "SOAchievementComponent.h"

#include "Kismet/GameplayStatics.h"

USOAchievementComponent::USOAchievementComponent()
{
	PrimaryComponentTick.bCanEverTick          = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval          = 0.1f;
}

void USOAchievementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ToastTimeRemaining > 0.0f)
	{
		ToastTimeRemaining = FMath::Max(0.0f, ToastTimeRemaining - DeltaTime);
	}
}

bool USOAchievementComponent::UnlockAchievement(FName ID, const FText& DisplayName)
{
	if (ID.IsNone() || UnlockedAchievements.Contains(ID))
	{
		return false;
	}

	UnlockedAchievements.Add(ID);

	ToastText          = DisplayName;
	ToastTimeRemaining = ToastDuration;

	if (UnlockSFX)
	{
		if (AActor* Owner = GetOwner())
		{
			UGameplayStatics::PlaySoundAtLocation(this, UnlockSFX, Owner->GetActorLocation());
		}
	}

	OnAchievementUnlocked.Broadcast(ID, DisplayName);
	OnAchievementUnlockedBP(ID, DisplayName);

	return true;
}
