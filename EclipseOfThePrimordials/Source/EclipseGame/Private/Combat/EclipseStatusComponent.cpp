// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Combat/EclipseStatusComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Combat/EclipseStaggerComponent.h"
#include "EclipseGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

UEclipseStatusComponent::UEclipseStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.f; // Status timers need frame accuracy for short CC.
	SetIsReplicatedByDefault(true);
}

void UEclipseStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEclipseStatusComponent, ActiveMask);
	DOREPLIFETIME(UEclipseStatusComponent, MoveSpeedScale);
}

void UEclipseStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Stagger = GetOwner()->FindComponentByClass<UEclipseStaggerComponent>();
	ensureMsgf(Tier != EEclipseMobTier::Boss || Stagger, TEXT("%s is a Boss but has no UEclipseStaggerComponent"), *GetNameSafe(GetOwner()));

	if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		BaseMaxWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	}

	// Clients only react to replication; no ticking needed there.
	SetComponentTickEnabled(GetOwner()->HasAuthority());
}

Eclipse::ApplyOutcome UEclipseStatusComponent::ApplyStatus(const FEclipseStatusSpec& Spec, AActor* Instigator)
{
	Eclipse::ApplyOutcome Outcome;
	if (!GetOwner() || !GetOwner()->HasAuthority() || !Spec.IsValid())
	{
		return Outcome; // Server authority: clients can never apply statuses.
	}

	Eclipse::StatusTarget Target{ Statuses, ToCore(Tier) };
	if (Stagger)
	{
		Target.Stagger = &Stagger->GetCoreState();
		Target.StaggerCfg = &Stagger->GetCoreConfig();
	}

	Outcome = Eclipse::ApplyStatusEffect(Target, Spec.ToCore());

	if (Stagger && (Outcome.StaggerAdded > 0.f || Outcome.Result == Eclipse::EApplyResult::StaggerBroken))
	{
		Stagger->NotifyMeterChanged(Outcome);
	}

	if (Outcome.Result == Eclipse::EApplyResult::Applied || Outcome.Result == Eclipse::EApplyResult::Refreshed)
	{
		if (Spec.Effect == EEclipseStatusEffect::Knockback)
		{
			ApplyKnockback(Spec, Instigator);
		}
	}
	if (Spec.Effect == EEclipseStatusEffect::Bleed || Spec.Effect == EEclipseStatusEffect::Burn)
	{
		LastDotInstigator = Instigator;
	}

	PostChangeSync();
	return Outcome;
}

void UEclipseStatusComponent::RemoveStatus(EEclipseStatusEffect Effect)
{
	if (GetOwner()->HasAuthority())
	{
		Statuses.Remove(static_cast<Eclipse::EStatusEffect>(Effect));
		PostChangeSync();
	}
}

void UEclipseStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PendingDotDamage += Statuses.Tick(DeltaTime);
	DotAccumulator += DeltaTime;
	if (DotAccumulator >= DotApplyInterval)
	{
		DotAccumulator = 0.f;
		FlushDotDamage();
	}

	// Expirations change the mask; only push when something actually changed.
	if (Statuses.GetActiveMask() != ActiveMask)
	{
		PostChangeSync();
	}
}

void UEclipseStatusComponent::FlushDotDamage()
{
	if (PendingDotDamage <= 0.f || !DotDamageEffect)
	{
		PendingDotDamage = 0.f;
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC)
	{
		PendingDotDamage = 0.f;
		return;
	}

	// DoT is self-applied with the original instigator recorded in the context for
	// kill credit. Full armor penetration: DoTs are tuned as true damage; Vulnerable
	// still applies in the AttributeSet.
	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddInstigator(LastDotInstigator.Get(), LastDotInstigator.Get());
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DotDamageEffect, 1.f, Context);
	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(EclipseTags::Data_Damage, PendingDotDamage);
		SpecHandle.Data->SetSetByCallerMagnitude(EclipseTags::Data_ArmorPenetration, 1.f);
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	PendingDotDamage = 0.f;
}

void UEclipseStatusComponent::PostChangeSync()
{
	ActiveMask = Statuses.GetActiveMask();
	MoveSpeedScale = Statuses.GetMoveSpeedScale();

	// Server mirrors exactly what OnRep does on clients.
	OnRep_ActiveMask();
	OnRep_MoveSpeedScale();
}

void UEclipseStatusComponent::OnRep_ActiveMask()
{
	if (AppliedMask != ActiveMask)
	{
		ApplyLooseTags(AppliedMask, ActiveMask);
		AppliedMask = ActiveMask;
		OnStatusMaskChanged.Broadcast(ActiveMask);
	}
}

void UEclipseStatusComponent::ApplyLooseTags(uint16 OldMask, uint16 NewMask)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC)
	{
		return;
	}
	const uint16 Changed = OldMask ^ NewMask;
	for (uint8 Bit = 0; Bit < Eclipse::StatusEffectCount; ++Bit)
	{
		if (Changed & (1u << Bit))
		{
			// Loose tags are local-only by design: each machine derives them from the
			// replicated mask, so no extra tag replication traffic is needed.
			ASC->SetLooseGameplayTagCount(EclipseTags::GetStatusTag(Bit), (NewMask & (1u << Bit)) ? 1 : 0);
		}
	}
}

void UEclipseStatusComponent::OnRep_MoveSpeedScale()
{
	ApplyMovementScale();
}

void UEclipseStatusComponent::ApplyMovementScale()
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (BaseMaxWalkSpeed > 0.f)
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = BaseMaxWalkSpeed * MoveSpeedScale;
		}
	}
}

void UEclipseStatusComponent::ApplyKnockback(const FEclipseStatusSpec& Spec, AActor* Instigator)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !Instigator)
	{
		return;
	}
	FVector Dir = Character->GetActorLocation() - Instigator->GetActorLocation();
	Dir.Z = 0.f;
	Dir = Dir.GetSafeNormal();
	// LaunchCharacter replicates through CharacterMovement's server correction.
	Character->LaunchCharacter(Dir * Spec.Magnitude + FVector(0.f, 0.f, Spec.Magnitude * 0.25f), true, true);
}
