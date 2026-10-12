// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Abilities/EclipseGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/EclipseHeroCharacter.h"
#include "Combat/EclipseAttributeSet.h"
#include "EclipseGameplayTags.h"
#include "Engine/World.h"
#include "Runes/EclipseRunewordComponent.h"
#include "Swarm/EclipseSwarmSubsystem.h"
#include "World/EclipseGroundZone.h"

UEclipseGameplayAbility::UEclipseGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor; // Holds per-cast state (delays, params).
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ActivationBlockedTags.AddTag(EclipseTags::Status_Stun);
	ActivationBlockedTags.AddTag(EclipseTags::Status_Freeze);
}

// =============================================================================
// Runeword detection
// =============================================================================

bool UEclipseGameplayAbility::IsRunewordSpec(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo)
{
	const UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	const FGameplayAbilitySpec* Spec = ASC ? ASC->FindAbilitySpecFromHandle(Handle) : nullptr;
	return Spec && Cast<UEclipseRunewordComponent>(Spec->SourceObject.Get()) != nullptr;
}

// =============================================================================
// Cost / cooldown
// =============================================================================

bool UEclipseGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (IsRunewordSpec(Handle, ActorInfo) || ResourceCost <= 0.f)
	{
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	}

	const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	const float Current = ASC->GetNumericAttribute(UEclipseAttributeSet::GetClassResourceAttribute());
	const float MaxValue = ASC->GetNumericAttribute(UEclipseAttributeSet::GetMaxClassResourceAttribute());

	const AEclipseHeroCharacter* Hero = Cast<AEclipseHeroCharacter>(ActorInfo->AvatarActor.Get());
	const EEclipseResourceModel Model = Hero ? Hero->GetResourceModel() : EEclipseResourceModel::Spender;

	const bool bAffordable = (Model == EEclipseResourceModel::Strain) ? (Current + ResourceCost <= MaxValue) : (Current >= ResourceCost);
	return bAffordable && Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
}

void UEclipseGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (IsRunewordSpec(Handle, ActorInfo))
	{
		return; // Runeword procs are free.
	}
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	const AEclipseHeroCharacter* Hero = Cast<AEclipseHeroCharacter>(ActorInfo->AvatarActor.Get());
	const EEclipseResourceModel Model = Hero ? Hero->GetResourceModel() : EEclipseResourceModel::Spender;

	float Delta = ResourceGeneratedOnCast;
	if (ResourceCost > 0.f)
	{
		Delta += (Model == EEclipseResourceModel::Strain) ? ResourceCost : -ResourceCost;
	}
	if (!FMath::IsNearlyZero(Delta) && ResourceDeltaEffect)
	{
		// Applied through GAS with the activation's prediction key: the owner sees the
		// bar move instantly; the server's application is authoritative.
		const FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, ResourceDeltaEffect);
		if (Spec.IsValid())
		{
			Spec.Data->SetSetByCallerMagnitude(EclipseTags::Data_ResourceDelta, Delta);
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
		}
	}

	if (ResourceCost > 0.f && ActorInfo->IsNetAuthority())
	{
		SendRitualEvent(EclipseTags::Event_Ritual_ResourceSpent, ResourceCost);
	}
}

bool UEclipseGameplayAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	// Runeword specs are rate-limited by their rune's own cooldown/offering instead.
	return IsRunewordSpec(Handle, ActorInfo) || Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}

void UEclipseGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (!IsRunewordSpec(Handle, ActorInfo))
	{
		Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	}
}

// =============================================================================
// Activation
// =============================================================================

void UEclipseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	bRunewordTriggered = IsRunewordSpec(Handle, ActorInfo);
	EventParam = TriggerEventData ? TriggerEventData->EventMagnitude : 0.f;

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Ultimates feed OnUltimateUsed runewords. Runeword-cast ultimates don't, which
	// prevents an "ultimate -> rune casts ultimate -> ..." chain.
	if (bIsUltimate && !bRunewordTriggered && ActorInfo->IsNetAuthority())
	{
		SendRitualEvent(EclipseTags::Event_Ritual_UltimateUsed, 1.f);
	}

	OnAbilityCommitted();
}

void UEclipseGameplayAbility::FinishAbility(bool bCancelled)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEndAbility*/ true, bCancelled);
}

// =============================================================================
// Helpers
// =============================================================================

bool UEclipseGameplayAbility::IsServer() const
{
	return CurrentActorInfo && CurrentActorInfo->IsNetAuthority();
}

FVector UEclipseGameplayAbility::GetAvatarLocation() const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	return Avatar ? Avatar->GetActorLocation() : FVector::ZeroVector;
}

FVector UEclipseGameplayAbility::GetAvatarForward() const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	return Avatar ? Avatar->GetActorForwardVector().GetSafeNormal2D() : FVector::ForwardVector;
}

UEclipseSwarmSubsystem* UEclipseGameplayAbility::GetSwarm() const
{
	const UWorld* World = GetWorld();
	return World ? World->GetSubsystem<UEclipseSwarmSubsystem>() : nullptr;
}

FEclipseAreaHitParams UEclipseGameplayAbility::MakeHitParams(float Damage, float ArmorPen, const FEclipseStatusSpec* Status) const
{
	FEclipseAreaHitParams P;
	P.SourceASC = GetAbilitySystemComponentFromActorInfo();
	P.DamageEffect = DamageEffect;
	P.Damage.Amount = Damage;
	P.Damage.ArmorPenetration = ArmorPen;
	P.Damage.Affinity = Affinity;
	P.Damage.Instigator = GetAvatarActorFromActorInfo();
	P.Status = Status;
	return P;
}

void UEclipseGameplayAbility::ModifyResource(float Delta) const
{
	if (!IsServer() || !ResourceDeltaEffect || FMath::IsNearlyZero(Delta))
	{
		return;
	}
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	const FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(ResourceDeltaEffect, 1.f, ASC->MakeEffectContext());
	if (Spec.IsValid())
	{
		Spec.Data->SetSetByCallerMagnitude(EclipseTags::Data_ResourceDelta, Delta);
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

AEclipseGroundZone* UEclipseGameplayAbility::SpawnGroundZone(TSubclassOf<AEclipseGroundZone> ZoneClass, const FVector& Location,
	float DamagePerPulse, const FEclipseStatusSpec& Status, float Lifetime) const
{
	UWorld* World = GetWorld();
	if (!IsServer() || !ZoneClass || !World)
	{
		return nullptr;
	}
	AActor* Avatar = GetAvatarActorFromActorInfo();
	const FTransform Xform(FRotator::ZeroRotator, Location);
	AEclipseGroundZone* Zone = World->SpawnActorDeferred<AEclipseGroundZone>(ZoneClass, Xform, Avatar, Cast<APawn>(Avatar),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Zone)
	{
		Zone->InitZone(GetAbilitySystemComponentFromActorInfo(), DamageEffect, DamagePerPulse, Affinity, Status, Lifetime);
		Zone->FinishSpawning(Xform);
	}
	return Zone;
}

int32 UEclipseGameplayAbility::SpawnSummons(TSubclassOf<AActor> SummonClass, int32 Count, float Radius, float Lifetime) const
{
	UWorld* World = GetWorld();
	if (!IsServer() || !SummonClass || !World)
	{
		return 0;
	}
	AActor* Avatar = GetAvatarActorFromActorInfo();
	FActorSpawnParameters Params;
	Params.Owner = Avatar;
	Params.Instigator = Cast<APawn>(Avatar);
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	int32 Spawned = 0;
	for (int32 K = 0; K < Count; ++K)
	{
		const float Angle = (UE_TWO_PI * K) / FMath::Max(1, Count);
		const FVector Loc = GetAvatarLocation() + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f) * Radius;
		if (AActor* Summon = World->SpawnActor<AActor>(SummonClass, Loc, GetAvatarForward().Rotation(), Params))
		{
			Summon->SetLifeSpan(Lifetime);
			++Spawned;
		}
	}
	return Spawned;
}

void UEclipseGameplayAbility::SendRitualEvent(const FGameplayTag& Tag, float Magnitude) const
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
	{
		return;
	}
	FGameplayEventData Payload;
	Payload.EventTag = Tag;
	Payload.Instigator = Avatar;
	Payload.EventMagnitude = Magnitude;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Avatar, Tag, Payload);
}
