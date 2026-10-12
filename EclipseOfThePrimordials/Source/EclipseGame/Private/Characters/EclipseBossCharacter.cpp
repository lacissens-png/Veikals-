// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Characters/EclipseBossCharacter.h"

#include "AbilitySystemComponent.h"
#include "Combat/EclipseAttributeSet.h"
#include "Combat/EclipseStaggerComponent.h"
#include "Combat/EclipseStatusComponent.h"

AEclipseBossCharacter::AEclipseBossCharacter()
{
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	// AI-controlled: nobody needs full GE replication.
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UEclipseAttributeSet>(TEXT("Attributes"));
	Stagger = CreateDefaultSubobject<UEclipseStaggerComponent>(TEXT("Stagger"));
	Status = CreateDefaultSubobject<UEclipseStatusComponent>(TEXT("Status"));
	Status->SetTier(EEclipseMobTier::Boss); // CC -> stagger routing.
}

UAbilitySystemComponent* AEclipseBossCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void AEclipseBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystem->InitAbilityActorInfo(this, this);
	ensureMsgf(Status->GetTier() == EEclipseMobTier::Boss, TEXT("%s: StatusComponent Tier must be Boss"), *GetName());

	if (HasAuthority())
	{
		AbilitySystem->SetNumericAttributeBase(UEclipseAttributeSet::GetMaxHealthAttribute(), MaxHealth);
		AbilitySystem->SetNumericAttributeBase(UEclipseAttributeSet::GetHealthAttribute(), MaxHealth);
		AbilitySystem->SetNumericAttributeBase(UEclipseAttributeSet::GetArmorAttribute(), Armor);

		Attributes->OnOutOfHealth.AddWeakLambda(this, [this](AActor*, AActor* Killer) { OnBossDefeated(Killer); });
	}
}

void AEclipseBossCharacter::OnBossDefeated_Implementation(AActor* Killer)
{
	SetLifeSpan(5.f); // Leave time for the death cinematic / loot fountain.
}
