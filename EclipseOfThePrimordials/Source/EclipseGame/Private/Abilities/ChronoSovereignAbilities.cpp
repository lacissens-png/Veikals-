// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Abilities/ChronoSovereignAbilities.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Combat/EclipseAttributeSet.h"
#include "Combat/EclipseCombatStatics.h"
#include "EclipseGameplayTags.h"
#include "Swarm/EclipseSwarmSubsystem.h"

// =============================================================================
// Time Stop
// =============================================================================

UGA_TimeStop::UGA_TimeStop()
{
	Affinity = EEclipseAffinity::Temporal;
}

void UGA_TimeStop::OnAbilityCommitted()
{
	if (IsServer())
	{
		// Freeze routes through the core pipeline: trash 100% duration, captains 75%,
		// brutes 50%, Unstoppable immune, bosses -> stagger meter (22 pts/s of freeze).
		FEclipseStatusSpec Freeze;
		Freeze.Effect = EEclipseStatusEffect::Freeze;
		Freeze.Duration = FMath::Clamp(GetParamOr(Duration), MinDuration, MaxDuration);

		EclipseCombat::HitSphere(this, GetAvatarLocation(), Radius, MakeHitParams(0.f, 0.f, &Freeze));

		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			FGameplayCueParameters Cue;
			Cue.Location = GetAvatarLocation();
			Cue.RawMagnitude = Radius;
			ASC->ExecuteGameplayCue(EclipseTags::GameplayCue_TimeStop, Cue);
		}
	}
	FinishAbility();
}

// =============================================================================
// Reality Slash
// =============================================================================

UGA_RealitySlash::UGA_RealitySlash()
{
	Affinity = EEclipseAffinity::Temporal;
}

void UGA_RealitySlash::OnAbilityCommitted()
{
	if (IsServer())
	{
		const FVector Start = GetAvatarLocation();
		const FVector End = Start + GetAvatarForward() * Length;
		// Runeword param is a damage scalar (1.0 = full Reality Slash).
		const float Scaled = Damage * GetParamOr(1.f);
		EclipseCombat::HitLine(this, Start, End, HalfWidth, MakeHitParams(Scaled, ArmorPenetration));
	}
	FinishAbility();
}

// =============================================================================
// The Goal of All Life is Death (Super-Tier Ultimate)
// =============================================================================

UGA_GoalOfAllLifeIsDeath::UGA_GoalOfAllLifeIsDeath()
{
	bIsUltimate = true;
	Affinity = EEclipseAffinity::Temporal;
	AbilityTags.AddTag(EclipseTags::Ability_Type_Ultimate);
}

void UGA_GoalOfAllLifeIsDeath::OnAbilityCommitted()
{
	if (IsServer())
	{
		// Mark: everything in range becomes Vulnerable for the countdown, telegraphing
		// the doom and rewarding the party for piling damage in before detonation.
		FEclipseStatusSpec Mark;
		Mark.Effect = EEclipseStatusEffect::Vulnerable;
		Mark.Duration = Delay;
		Mark.Magnitude = MarkVulnerableBonus;
		EclipseCombat::HitSphere(this, GetAvatarLocation(), Radius, MakeHitParams(0.f, 0.f, &Mark));
	}

	// The ability stays active for the countdown (InstancedPerActor holds the state).
	// If the caster dies or the ability is cancelled, the task is destroyed with it
	// and the wave never fires - the counterplay to this ultimate.
	UAbilityTask_WaitDelay* Wait = UAbilityTask_WaitDelay::WaitDelay(this, Delay);
	Wait->OnFinish.AddDynamic(this, &UGA_GoalOfAllLifeIsDeath::Detonate);
	Wait->ReadyForActivation();
}

void UGA_GoalOfAllLifeIsDeath::Detonate()
{
	if (IsServer())
	{
		const FVector Center = GetAvatarLocation();

		// 1) Fatal to every non-boss mob: execute with threshold 1.0 kills trash,
		//    captains and brutes regardless of health (Brutes use Threshold*0.5 in the
		//    core rule, so pass 2.0 to make them executable at any health).
		FEclipseAreaHitParams Wave = MakeHitParams(0.f);
		Wave.Damage.bExecute = true;
		Wave.Damage.ExecuteThreshold = 2.f;
		Wave.bIncludeActors = false;
		EclipseCombat::HitSphere(this, Center, Radius, Wave);

		// 2) Bosses: a fixed fraction of max health, ignoring armor.
		TArray<AActor*> Actors;
		EclipseCombat::GatherHostileActorsInRadius(this, Center, Radius, GetAvatarActorFromActorInfo(), Actors);
		for (AActor* A : Actors)
		{
			const UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(A);
			const float MaxHP = TargetASC ? TargetASC->GetNumericAttribute(UEclipseAttributeSet::GetMaxHealthAttribute()) : 0.f;
			EclipseCombat::ApplyDamageToActor(MakeHitParams(MaxHP * BossMaxHealthFraction, 1.f), A);
		}

		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			FGameplayCueParameters Cue;
			Cue.Location = Center;
			Cue.RawMagnitude = Radius;
			ASC->ExecuteGameplayCue(EclipseTags::GameplayCue_DeathWave, Cue);
		}
	}
	FinishAbility();
}
