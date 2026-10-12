// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Combat/EclipseAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Combat/EclipseStatusComponent.h"
#include "EclipseGameSettings.h"
#include "EclipseGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UEclipseAttributeSet::UEclipseAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitArmor(0.f);
	InitClassResource(0.f);
	InitMaxClassResource(100.f);
	InitIncomingDamage(0.f);
}

void UEclipseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// REPNOTIFY_Always so client-predicted changes that end on the same value still notify.
	DOREPLIFETIME_CONDITION_NOTIFY(UEclipseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEclipseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEclipseAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	// Only the owning player needs their resource bar; saves bandwidth with many players.
	DOREPLIFETIME_CONDITION_NOTIFY(UEclipseAttributeSet, ClassResource, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEclipseAttributeSet, MaxClassResource, COND_OwnerOnly, REPNOTIFY_Always);
}

void UEclipseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetClassResourceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxClassResource());
	}
	else if (Attribute == GetMaxHealthAttribute() || Attribute == GetMaxClassResourceAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
}

void UEclipseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute != GetIncomingDamageAttribute())
	{
		if (Data.EvaluatedData.Attribute == GetClassResourceAttribute())
		{
			SetClassResource(FMath::Clamp(GetClassResource(), 0.f, GetMaxClassResource()));
		}
		return;
	}

	// ---- Damage resolution (server: GEs with executions never run on clients) ----
	const float RawDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (RawDamage <= 0.f || bOutOfHealth)
	{
		return;
	}

	AActor* Victim = GetOwningActor();
	float DamageTakenMultiplier = 1.f;
	if (const UEclipseStatusComponent* Status = Victim ? Victim->FindComponentByClass<UEclipseStatusComponent>() : nullptr)
	{
		// Vulnerable (+20%+) and the stagger window are applied here so that every
		// damage source (abilities, DoTs, mob hits) benefits identically.
		DamageTakenMultiplier = Status->GetDamageTakenMultiplier();
	}

	const float FinalDamage = RawDamage * DamageTakenMultiplier;
	const float NewHealth = FMath::Clamp(GetHealth() - FinalDamage, 0.f, GetMaxHealth());
	SetHealth(NewHealth);

	// Heavy-hit ritual event (runewords: OnHeavyDamageTaken). Sent to the victim's own ASC.
	const float Fraction = GetMaxHealth() > 0.f ? FinalDamage / GetMaxHealth() : 0.f;
	if (Victim && Fraction >= UEclipseGameSettings::Get()->HeavyDamageFraction)
	{
		FGameplayEventData Payload;
		Payload.EventTag = EclipseTags::Event_Ritual_HeavyDamageTaken;
		Payload.EventMagnitude = Fraction;
		Payload.Instigator = Data.EffectSpec.GetContext().GetOriginalInstigator();
		Payload.Target = Victim;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Victim, Payload.EventTag, Payload);
	}

	if (NewHealth <= 0.f)
	{
		bOutOfHealth = true;
		OnOutOfHealth.Broadcast(Victim, Data.EffectSpec.GetContext().GetOriginalInstigator());
	}
}

void UEclipseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEclipseAttributeSet, Health, OldValue);
}

void UEclipseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEclipseAttributeSet, MaxHealth, OldValue);
}

void UEclipseAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEclipseAttributeSet, Armor, OldValue);
}

void UEclipseAttributeSet::OnRep_ClassResource(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEclipseAttributeSet, ClassResource, OldValue);
}

void UEclipseAttributeSet::OnRep_MaxClassResource(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEclipseAttributeSet, MaxClassResource, OldValue);
}
