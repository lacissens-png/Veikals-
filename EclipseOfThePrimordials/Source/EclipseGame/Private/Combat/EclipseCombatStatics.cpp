// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Combat/EclipseCombatStatics.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Combat/EclipseStatusComponent.h"
#include "EclipseGameplayTags.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GenericTeamAgentInterface.h"
#include "Swarm/EclipseSwarmSubsystem.h"
#include "World/EclipseFieldAffinitySubsystem.h"

namespace
{
	bool IsServer(const UObject* WorldContext)
	{
		const UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
		return World && World->GetNetMode() != NM_Client;
	}

	bool IsCrowdControl(const FEclipseStatusSpec* Spec)
	{
		return Spec && Eclipse::IsCrowdControl(static_cast<Eclipse::EStatusEffect>(Spec->Effect));
	}

	void HitActors(const UObject* WorldContext, const TArray<AActor*>& Actors, const FEclipseAreaHitParams& Params,
		FEclipseAreaHitResult& Result)
	{
		AActor* Instigator = Params.Damage.Instigator.Get();
		for (AActor* Target : Actors)
		{
			if (Params.Damage.Amount > 0.f && EclipseCombat::ApplyDamageToActor(Params, Target))
			{
				++Result.ActorsHit;
			}
			if (Params.Status && EclipseCombat::ApplyStatusToActor(Target, *Params.Status, Instigator).Landed()
				&& IsCrowdControl(Params.Status))
			{
				++Result.CrowdControlLanded;
			}
		}
	}
}

namespace EclipseCombat
{
	bool IsHostile(const AActor* Source, const AActor* Target)
	{
		if (!Source || !Target || Source == Target || Target->IsPendingKillPending())
		{
			return false;
		}
		// Teams come from IGenericTeamAgentInterface (heroes = 1, enemies = 2, pets inherit owner).
		const FGenericTeamId A = FGenericTeamId::GetTeamIdentifier(Source);
		const FGenericTeamId B = FGenericTeamId::GetTeamIdentifier(Target);
		return A != B && B != FGenericTeamId::NoTeam;
	}

	bool ApplyDamageToActor(const FEclipseAreaHitParams& Params, AActor* Target)
	{
		UAbilitySystemComponent* SourceASC = Params.SourceASC;
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		if (!SourceASC || !TargetASC || !Params.DamageEffect || !IsServer(Target))
		{
			return false;
		}

		float Amount = Params.Damage.Amount;
		if (const UEclipseFieldAffinitySubsystem* Field = Target->GetWorld()->GetSubsystem<UEclipseFieldAffinitySubsystem>())
		{
			Amount *= Field->GetDamageScalar(Target->GetActorLocation(), Params.Damage.Affinity);
		}

		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		Context.AddInstigator(Params.Damage.Instigator.Get(), Params.Damage.Instigator.Get());
		const FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(Params.DamageEffect, 1.f, Context);
		if (!Spec.IsValid())
		{
			return false;
		}
		Spec.Data->SetSetByCallerMagnitude(EclipseTags::Data_Damage, Amount);
		Spec.Data->SetSetByCallerMagnitude(EclipseTags::Data_ArmorPenetration, Params.Damage.ArmorPenetration);
		SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
		return true;
	}

	Eclipse::ApplyOutcome ApplyStatusToActor(AActor* Target, const FEclipseStatusSpec& Spec, AActor* Instigator)
	{
		if (UEclipseStatusComponent* Status = Target ? Target->FindComponentByClass<UEclipseStatusComponent>() : nullptr)
		{
			return Status->ApplyStatus(Spec, Instigator);
		}
		return {};
	}

	void GatherHostileActorsInRadius(const UObject* WorldContext, const FVector& Center, float Radius, const AActor* Source,
		TArray<AActor*>& Out)
	{
		UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
		if (!World)
		{
			return;
		}
		TArray<FOverlapResult> Overlaps;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(EclipseAreaHit), false, Source);
		World->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity, FCollisionObjectQueryParams(ECC_Pawn),
			FCollisionShape::MakeSphere(Radius), QueryParams);

		for (const FOverlapResult& O : Overlaps)
		{
			AActor* A = O.GetActor();
			if (A && IsHostile(Source, A) && UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(A))
			{
				Out.AddUnique(A); // Multi-component actors can overlap more than once.
			}
		}
	}

	FEclipseAreaHitResult HitMobs(const UObject* WorldContext, const TArray<FEclipseMobHandle>& Mobs, const FEclipseAreaHitParams& Params)
	{
		FEclipseAreaHitResult Result;
		UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
		UEclipseSwarmSubsystem* Swarm = World ? World->GetSubsystem<UEclipseSwarmSubsystem>() : nullptr;
		if (!Swarm || !IsServer(WorldContext))
		{
			return Result;
		}

		AActor* Instigator = Params.Damage.Instigator.Get();
		for (const FEclipseMobHandle& H : Mobs)
		{
			// Status first so a Vulnerable/Freeze in the same hit affects this hit's damage.
			if (Params.Status && Swarm->ApplyStatusToMob(H, *Params.Status, Instigator).Landed() && IsCrowdControl(Params.Status))
			{
				++Result.CrowdControlLanded;
			}
			if (Params.Damage.Amount > 0.f || Params.Damage.bExecute)
			{
				const FEclipseMobDamageResult R = Swarm->ApplyDamageToMob(H, Params.Damage);
				Result.TotalDamage += R.DamageDealt;
				Result.MobsKilled += R.bKilled ? 1 : 0;
			}
			++Result.MobsHit;
		}
		return Result;
	}

	FEclipseAreaHitResult HitSphere(const UObject* WorldContext, const FVector& Center, float Radius, const FEclipseAreaHitParams& Params)
	{
		FEclipseAreaHitResult Result;
		UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
		if (!World || !IsServer(WorldContext))
		{
			return Result;
		}

		if (UEclipseSwarmSubsystem* Swarm = World->GetSubsystem<UEclipseSwarmSubsystem>())
		{
			TArray<FEclipseMobHandle> Mobs;
			Swarm->QueryMobsInRadius(Center, Radius, Mobs);
			Result = HitMobs(WorldContext, Mobs, Params);
		}

		if (Params.bIncludeActors)
		{
			TArray<AActor*> Actors;
			GatherHostileActorsInRadius(WorldContext, Center, Radius, Params.Damage.Instigator.Get(), Actors);
			HitActors(WorldContext, Actors, Params, Result);
		}

		NotifyCrowdControlLanded(Params.Damage.Instigator.Get(), Result.CrowdControlLanded);
		return Result;
	}

	FEclipseAreaHitResult HitLine(const UObject* WorldContext, const FVector& Start, const FVector& End, float HalfWidth,
		const FEclipseAreaHitParams& Params)
	{
		FEclipseAreaHitResult Result;
		UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
		if (!World || !IsServer(WorldContext))
		{
			return Result;
		}

		if (UEclipseSwarmSubsystem* Swarm = World->GetSubsystem<UEclipseSwarmSubsystem>())
		{
			TArray<FEclipseMobHandle> Mobs;
			Swarm->QueryMobsInCapsule(Start, End, HalfWidth, Mobs);
			Result = HitMobs(WorldContext, Mobs, Params);
		}

		if (Params.bIncludeActors)
		{
			TArray<FHitResult> Hits;
			FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(EclipseLineHit), false, Params.Damage.Instigator.Get());
			World->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, FCollisionObjectQueryParams(ECC_Pawn),
				FCollisionShape::MakeSphere(HalfWidth), QueryParams);

			TArray<AActor*> Actors;
			for (const FHitResult& H : Hits)
			{
				AActor* A = H.GetActor();
				if (A && IsHostile(Params.Damage.Instigator.Get(), A) && UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(A))
				{
					Actors.AddUnique(A);
				}
			}
			HitActors(WorldContext, Actors, Params, Result);
		}

		NotifyCrowdControlLanded(Params.Damage.Instigator.Get(), Result.CrowdControlLanded);
		return Result;
	}

	void NotifyCrowdControlLanded(AActor* Instigator, int32 Count)
	{
		if (!Instigator || Count <= 0)
		{
			return;
		}
		FGameplayEventData Payload;
		Payload.EventTag = EclipseTags::Event_Ritual_CrowdControlApplied;
		Payload.Instigator = Instigator;
		Payload.EventMagnitude = static_cast<float>(Count);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, Payload.EventTag, Payload);
	}
}
