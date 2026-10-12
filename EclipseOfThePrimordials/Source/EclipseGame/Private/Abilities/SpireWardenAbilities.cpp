// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Abilities/SpireWardenAbilities.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystemComponent.h"
#include "Combat/EclipseCombatStatics.h"
#include "EclipseGameplayTags.h"
#include "World/EclipseGroundZone.h"

// =============================================================================
// Grand Catastrophe
// =============================================================================

UGA_GrandCatastrophe::UGA_GrandCatastrophe()
{
	Affinity = EEclipseAffinity::Seismic;
	ResourceGeneratedOnCast = 15.f; // Builder: slams build Seismic Pressure.
	ObeliskBurn.Effect = EEclipseStatusEffect::Burn;
	ObeliskBurn.Duration = 2.f;
	ObeliskBurn.Magnitude = 25.f;
}

void UGA_GrandCatastrophe::OnAbilityCommitted()
{
	if (IsServer())
	{
		const FVector Center = GetAvatarLocation();

		// Knockback goes through the status pipeline so tier rules apply uniformly
		// (Brute immune, Unstoppable immune, Boss -> flat stagger fill).
		FEclipseStatusSpec Knockback;
		Knockback.Effect = EEclipseStatusEffect::Knockback;
		Knockback.Magnitude = KnockbackImpulse;
		EclipseCombat::HitSphere(this, Center, Radius, MakeHitParams(Damage, 0.f, &Knockback));

		for (int32 K = 0; K < ObeliskCount; ++K)
		{
			const float Angle = (UE_TWO_PI * K) / FMath::Max(1, ObeliskCount);
			const FVector Loc = Center + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f) * ObeliskRingRadius;
			SpawnGroundZone(ObeliskClass, Loc, ObeliskPulseDamage, ObeliskBurn, ObeliskLifetime);
		}
	}
	FinishAbility();
}

// =============================================================================
// Fallen Down (Super-Tier Ultimate)
// =============================================================================

UGA_FallenDown::UGA_FallenDown()
{
	bIsUltimate = true;
	Affinity = EEclipseAffinity::Seismic;
	AbilityTags.AddTag(EclipseTags::Ability_Type_Ultimate);
	BeamBurn.Effect = EEclipseStatusEffect::Burn;
	BeamBurn.Duration = 4.f;
	BeamBurn.Magnitude = 150.f;
}

void UGA_FallenDown::OnAbilityCommitted()
{
	BeamLocation = GetAvatarLocation() + GetAvatarForward() * TargetDistance;

	if (IsServer())
	{
		// Telegraph cue for every client: the beam's targeting reticle.
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			FGameplayCueParameters Cue;
			Cue.Location = BeamLocation;
			Cue.RawMagnitude = Radius;
			ASC->AddGameplayCue(EclipseTags::GameplayCue_OrbitalBeam, Cue);
		}
	}

	UAbilityTask_WaitDelay* Wait = UAbilityTask_WaitDelay::WaitDelay(this, TelegraphDelay);
	Wait->OnFinish.AddDynamic(this, &UGA_FallenDown::StrikeBeam);
	Wait->ReadyForActivation();
}

void UGA_FallenDown::StrikeBeam()
{
	if (IsServer())
	{
		EclipseCombat::HitSphere(this, BeamLocation, Radius, MakeHitParams(Damage, 0.f, &BeamBurn));
		SpawnGroundZone(ScorchZoneClass, BeamLocation, ScorchPulseDamage, BeamBurn, ScorchLifetime);

		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			ASC->RemoveGameplayCue(EclipseTags::GameplayCue_OrbitalBeam);
		}
	}
	FinishAbility();
}
