// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Abilities/EclipsePuppeteerAbilities.h"

#include "Combat/EclipseCombatStatics.h"
#include "EclipseGameplayTags.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Swarm/EclipseSwarmSubsystem.h"
#include "World/EclipseCreationDomain.h"

// =============================================================================
// Puppet Master's Thread
// =============================================================================

UGA_PuppetMastersThread::UGA_PuppetMastersThread()
{
	Affinity = EEclipseAffinity::Lunar;
}

void UGA_PuppetMastersThread::OnAbilityCommitted()
{
	UEclipseSwarmSubsystem* Swarm = GetSwarm();
	if (IsServer() && Swarm)
	{
		const FEclipseMobHandle Primary = Swarm->FindNearestMobInCone(GetAvatarLocation(), GetAvatarForward(), CastRange, 35.f);
		FEclipseMobInfo PrimaryInfo;
		if (Swarm->GetMobInfo(Primary, PrimaryInfo))
		{
			// Gather neighbours around the primary, nearest first, capped at MaxLinks total.
			TArray<FEclipseMobHandle> Nearby;
			Swarm->QueryMobsInRadius(PrimaryInfo.Location, LinkRadius, Nearby);
			Nearby.RemoveSwap(Primary);

			TArray<TPair<float, FEclipseMobHandle>> Sorted;
			Sorted.Reserve(Nearby.Num());
			for (const FEclipseMobHandle& H : Nearby)
			{
				FEclipseMobInfo Info;
				if (Swarm->GetMobInfo(H, Info))
				{
					Sorted.Emplace(FVector::DistSquared2D(Info.Location, PrimaryInfo.Location), H);
				}
			}
			Sorted.Sort([](const TPair<float, FEclipseMobHandle>& A, const TPair<float, FEclipseMobHandle>& B) { return A.Key < B.Key; });

			TArray<FEclipseMobHandle> Links;
			Links.Add(Primary);
			for (int32 K = 0; K < Sorted.Num() && Links.Num() < MaxLinks; ++K)
			{
				Links.Add(Sorted[K].Value);
			}

			Swarm->CreateDamageLinkGroup(Links, ShareFraction, Duration);

			// Opening hit on the primary immediately propagates through the new threads.
			EclipseCombat::HitMobs(this, { Primary }, MakeHitParams(InitialDamage));
		}
	}
	FinishAbility();
}

// =============================================================================
// Creation (Super-Tier Ultimate)
// =============================================================================

UGA_Creation::UGA_Creation()
{
	bIsUltimate = true;
	Affinity = EEclipseAffinity::Lunar;
	AbilityTags.AddTag(EclipseTags::Ability_Type_Ultimate);
}

void UGA_Creation::OnAbilityCommitted()
{
	UWorld* World = GetWorld();
	if (IsServer() && World)
	{
		const TSubclassOf<AEclipseCreationDomain> Class = DomainClass ? DomainClass : TSubclassOf<AEclipseCreationDomain>(AEclipseCreationDomain::StaticClass());
		const FTransform Xform(FRotator::ZeroRotator, GetAvatarLocation());
		AActor* Avatar = GetAvatarActorFromActorInfo();
		if (AEclipseCreationDomain* Domain = World->SpawnActorDeferred<AEclipseCreationDomain>(Class, Xform, Avatar, Cast<APawn>(Avatar),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			Domain->InitDomain(OverrideAffinity, Radius, Duration, ResonanceScalar);
			Domain->FinishSpawning(Xform); // Replicates; registers with the field subsystem on every machine.
		}
	}
	FinishAbility();
}

// =============================================================================
// Gain Unstoppable (rune effect)
// =============================================================================

void UGA_GainUnstoppable::OnAbilityCommitted()
{
	if (IsServer())
	{
		FEclipseStatusSpec Unstoppable;
		Unstoppable.Effect = EEclipseStatusEffect::Unstoppable;
		Unstoppable.Duration = GetParamOr(DefaultDuration);
		EclipseCombat::ApplyStatusToActor(GetAvatarActorFromActorInfo(), Unstoppable, GetAvatarActorFromActorInfo());
	}
	FinishAbility();
}
