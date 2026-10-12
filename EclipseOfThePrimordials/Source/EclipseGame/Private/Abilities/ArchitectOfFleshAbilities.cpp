// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Abilities/ArchitectOfFleshAbilities.h"

#include "Combat/EclipseCombatStatics.h"
#include "EclipseGameplayTags.h"
#include "Swarm/EclipseSwarmSubsystem.h"

// =============================================================================
// Grasping Heart
// =============================================================================

UGA_GraspingHeart::UGA_GraspingHeart()
{
	Affinity = EEclipseAffinity::Blood;
}

void UGA_GraspingHeart::OnAbilityCommitted()
{
	if (IsServer())
	{
		FEclipseStatusSpec Stun;
		Stun.Effect = EEclipseStatusEffect::Stun;
		Stun.Duration = StunDuration;

		FEclipseAreaHitParams Params = MakeHitParams(Damage, 0.f, &Stun);
		Params.Damage.bExecute = true;
		Params.Damage.ExecuteThreshold = ExecuteThreshold;

		// Target selection is server-side (cone from the facing the client replicated).
		// A production build would send FGameplayAbilityTargetData (cursor hit) from the
		// client and validate it here against Range.
		bool bHitSomething = false;
		if (UEclipseSwarmSubsystem* Swarm = GetSwarm())
		{
			const FEclipseMobHandle Target = Swarm->FindNearestMobInCone(GetAvatarLocation(), GetAvatarForward(), Range, TargetConeHalfAngle);
			if (Target.IsSet())
			{
				// Execute-or-stun: the stun only matters if the target survives.
				const FEclipseAreaHitResult R = EclipseCombat::HitMobs(this, { Target }, Params);
				EclipseCombat::NotifyCrowdControlLanded(GetAvatarActorFromActorInfo(), R.CrowdControlLanded);
				if (R.MobsKilled > 0)
				{
					ModifyResource(BiomassOnKill);
				}
				bHitSomething = true;
			}
		}

		if (!bHitSomething)
		{
			// No swarm target: grab the nearest boss/actor in front (stun -> stagger on bosses).
			TArray<AActor*> Actors;
			EclipseCombat::GatherHostileActorsInRadius(this, GetAvatarLocation() + GetAvatarForward() * Range * 0.5f, Range * 0.5f,
				GetAvatarActorFromActorInfo(), Actors);
			if (Actors.Num() > 0)
			{
				Params.Damage.bExecute = false; // Bosses are never executable.
				EclipseCombat::ApplyDamageToActor(Params, Actors[0]);
				const bool bLanded = EclipseCombat::ApplyStatusToActor(Actors[0], Stun, GetAvatarActorFromActorInfo()).Landed();
				EclipseCombat::NotifyCrowdControlLanded(GetAvatarActorFromActorInfo(), bLanded ? 1 : 0);
			}
		}
	}
	FinishAbility();
}

// =============================================================================
// Ia Mshub-Niggurath (Super-Tier Ultimate)
// =============================================================================

UGA_IaMshubNiggurath::UGA_IaMshubNiggurath()
{
	bIsUltimate = true;
	Affinity = EEclipseAffinity::Blood;
	AbilityTags.AddTag(EclipseTags::Ability_Type_Ultimate);
}

void UGA_IaMshubNiggurath::OnAbilityCommitted()
{
	UEclipseSwarmSubsystem* Swarm = GetSwarm();
	if (IsServer() && Swarm)
	{
		TArray<FEclipseMobHandle> InRadius;
		Swarm->QueryMobsInRadius(GetAvatarLocation(), Radius, InRadius);

		// Partition: trash dies outright (execute), everything else takes heavy damage.
		TArray<FEclipseMobHandle> Trash, Others;
		for (const FEclipseMobHandle& H : InRadius)
		{
			FEclipseMobInfo Info;
			if (Swarm->GetMobInfo(H, Info))
			{
				(Info.Tier == EEclipseMobTier::TrashSwarmer ? Trash : Others).Add(H);
			}
		}

		FEclipseAreaHitParams Cull = MakeHitParams(0.f);
		Cull.Damage.bExecute = true; // Core rule: trash is always executable.
		const FEclipseAreaHitResult Culled = EclipseCombat::HitMobs(this, Trash, Cull);

		const FEclipseAreaHitParams Heavy = MakeHitParams(Damage);
		const FEclipseAreaHitResult Hit = EclipseCombat::HitMobs(this, Others, Heavy);

		// Bosses / actors in range.
		TArray<AActor*> Actors;
		EclipseCombat::GatherHostileActorsInRadius(this, GetAvatarLocation(), Radius, GetAvatarActorFromActorInfo(), Actors);
		for (AActor* A : Actors)
		{
			EclipseCombat::ApplyDamageToActor(Heavy, A);
		}

		ModifyResource(BiomassPerKill * static_cast<float>(Culled.MobsKilled + Hit.MobsKilled));
		SpawnSummons(DarkYoungClass, DarkYoungCount, 250.f, DarkYoungLifetime);
	}
	FinishAbility();
}

// =============================================================================
// Flesh Colossus (rune effect)
// =============================================================================

void UGA_SpawnFleshColossus::OnAbilityCommitted()
{
	SpawnSummons(ColossusClass, 1, 200.f, GetParamOr(DefaultLifetime));
	FinishAbility();
}
