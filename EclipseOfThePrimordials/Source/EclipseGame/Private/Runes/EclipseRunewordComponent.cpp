// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Runes/EclipseRunewordComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EclipseGameplayTags.h"
#include "Items/EclipseInventoryComponent.h"
#include "Net/UnrealNetwork.h"

UEclipseRunewordComponent::UEclipseRunewordComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f; // Only cooldowns tick; 10 Hz is ample.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
}

void UEclipseRunewordComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UEclipseRunewordComponent, ActiveViews, COND_OwnerOnly);
}

FGameplayTag UEclipseRunewordComponent::GetConditionTag(Eclipse::ERitualCondition Condition)
{
	switch (Condition)
	{
	case Eclipse::ERitualCondition::OnUltimateUsed:        return EclipseTags::Event_Ritual_UltimateUsed;
	case Eclipse::ERitualCondition::OnCrowdControlApplied: return EclipseTags::Event_Ritual_CrowdControlApplied;
	case Eclipse::ERitualCondition::OnHeavyDamageTaken:    return EclipseTags::Event_Ritual_HeavyDamageTaken;
	case Eclipse::ERitualCondition::OnEnemyKilled:         return EclipseTags::Event_Ritual_EnemyKilled;
	case Eclipse::ERitualCondition::OnResourceSpent:       return EclipseTags::Event_Ritual_ResourceSpent;
	default:                                               return FGameplayTag();
	}
}

UAbilitySystemComponent* UEclipseRunewordComponent::GetASC() const
{
	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
}

void UEclipseRunewordComponent::BindToInventory(UEclipseInventoryComponent* InInventory)
{
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	if (UEclipseInventoryComponent* Old = Inventory.Get())
	{
		Old->OnEquipmentChanged.Remove(EquipmentChangedHandle);
	}
	Inventory = InInventory;
	if (InInventory)
	{
		EquipmentChangedHandle = InInventory->OnEquipmentChanged.AddUObject(this, &UEclipseRunewordComponent::Rebuild);
	}
	SetComponentTickEnabled(true);
	Rebuild();
}

void UEclipseRunewordComponent::Rebuild()
{
	const UEclipseItemDatabase* Db = UEclipseItemDatabase::Get();
	UEclipseInventoryComponent* Inv = Inventory.Get();
	if (!GetOwner()->HasAuthority() || !Db || !Inv)
	{
		return;
	}

	// Convert equipped items to core form. The vector owns the copies for the
	// duration of Rebuild; the evaluator copies what it keeps (rune definitions).
	TArray<const FEclipseItemData*> Equipped;
	Inv->GetEquippedItems(Equipped);
	std::vector<Eclipse::ItemInstance> CoreItems;
	CoreItems.reserve(Equipped.Num());
	for (const FEclipseItemData* Item : Equipped)
	{
		CoreItems.push_back(EclipseItems::ToCore(*Item));
	}
	std::vector<const Eclipse::ItemInstance*> Ptrs;
	Ptrs.reserve(CoreItems.size());
	for (const Eclipse::ItemInstance& I : CoreItems)
	{
		Ptrs.push_back(&I);
	}

	Evaluator.Rebuild(Ptrs, Db->GetRuneDatabase());

	SyncEventBindings();
	SyncEffectAbilities(*Db);

	ActiveViews.Reset();
	for (const Eclipse::RunewordBinding& B : Evaluator.GetBindings())
	{
		FEclipseRunewordView& V = ActiveViews.AddDefaulted_GetRef();
		V.ItemGuid = static_cast<int64>(B.ItemGuid);
		V.PairIndex = B.PairIndex;
		V.RitualRuneId = static_cast<int32>(B.Ritual.Id);
		V.EffectRuneId = static_cast<int32>(B.Effect.Id);
		V.Condition = static_cast<EEclipseRitualCondition>(B.Ritual.Condition);
		V.Action = static_cast<EEclipseRuneEffectAction>(B.Effect.Action);
	}
}

void UEclipseRunewordComponent::SyncEventBindings()
{
	UAbilitySystemComponent* ASC = GetASC();
	if (!ASC)
	{
		return;
	}

	for (uint8 C = 0; C < Eclipse::RitualConditionCount; ++C)
	{
		const Eclipse::ERitualCondition Condition = static_cast<Eclipse::ERitualCondition>(C);
		const bool bNeeded = Evaluator.GetBindingCountFor(Condition) > 0;
		const bool bBound = EventBindings.Contains(C);
		const FGameplayTag Tag = GetConditionTag(Condition);

		if (bNeeded && !bBound)
		{
			// DYNAMIC BINDING: subscribe this condition's gameplay event to the evaluator.
			// The condition is captured as a delegate payload, so one handler serves all.
			const FDelegateHandle Handle = ASC->GenericGameplayEventCallbacks.FindOrAdd(Tag)
				.AddUObject(this, &UEclipseRunewordComponent::HandleRitualEvent, Condition);
			EventBindings.Add(C, Handle);
		}
		else if (!bNeeded && bBound)
		{
			if (FGameplayEventMulticastDelegate* Delegate = ASC->GenericGameplayEventCallbacks.Find(Tag))
			{
				Delegate->Remove(EventBindings[C]);
			}
			EventBindings.Remove(C);
		}
	}
}

void UEclipseRunewordComponent::SyncEffectAbilities(const UEclipseItemDatabase& Db)
{
	UAbilitySystemComponent* ASC = GetASC();
	if (!ASC)
	{
		return;
	}

	TSet<uint8> Needed;
	for (const Eclipse::RunewordBinding& B : Evaluator.GetBindings())
	{
		Needed.Add(static_cast<uint8>(B.Effect.Action));
	}

	// Revoke abilities whose effect rune is no longer socketed.
	for (auto It = EffectSpecs.CreateIterator(); It; ++It)
	{
		if (!Needed.Contains(It->Key))
		{
			ASC->ClearAbility(It->Value);
			It.RemoveCurrent();
		}
	}

	// Grant newly needed ones. A separate spec from the class-granted one (if any),
	// tagged by SourceObject so it skips resource cost and class cooldown.
	for (const uint8 Action : Needed)
	{
		if (EffectSpecs.Contains(Action))
		{
			continue;
		}
		const TSubclassOf<UGameplayAbility>* AbilityClass = Db.RuneEffectAbilities.Find(static_cast<EEclipseRuneEffectAction>(Action));
		if (AbilityClass && *AbilityClass)
		{
			FGameplayAbilitySpec Spec(*AbilityClass, 1, INDEX_NONE, this);
			Spec.DynamicAbilityTags.AddTag(EclipseTags::Ability_Source_Runeword);
			EffectSpecs.Add(Action, ASC->GiveAbility(Spec));
		}
	}
}

void UEclipseRunewordComponent::HandleRitualEvent(const FGameplayEventData* Payload, Eclipse::ERitualCondition Condition)
{
	if (bDispatching || !GetOwner()->HasAuthority())
	{
		return; // Re-entrant event from an effect we are currently firing.
	}
	TGuardValue<bool> Guard(bDispatching, true);

	Eclipse::RitualEvent Event;
	Event.Condition = Condition;
	Event.Magnitude = Payload ? Payload->EventMagnitude : 1.f;

	InvocationScratch.clear(); // Reused buffer: no allocation in steady state.
	Evaluator.EvaluateRuneword(Event, InvocationScratch);
	for (const Eclipse::RuneEffectInvocation& Invocation : InvocationScratch)
	{
		TriggerEffect(Invocation);
	}
}

void UEclipseRunewordComponent::TriggerEffect(const Eclipse::RuneEffectInvocation& Invocation)
{
	UAbilitySystemComponent* ASC = GetASC();
	const FGameplayAbilitySpecHandle* Handle = EffectSpecs.Find(static_cast<uint8>(Invocation.Action));
	if (!ASC || !Handle)
	{
		return;
	}

	FGameplayEventData EffectPayload;
	EffectPayload.EventTag = EclipseTags::Ability_Source_Runeword;
	EffectPayload.EventMagnitude = Invocation.Param; // e.g. TriggerTimeStop(Duration)
	EffectPayload.Instigator = GetOwner();
	EffectPayload.OptionalObject = this;

	ASC->TriggerAbilityFromGameplayEvent(*Handle, ASC->AbilityActorInfo.Get(), EffectPayload.EventTag, &EffectPayload, *ASC);
}

void UEclipseRunewordComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Evaluator.Tick(DeltaTime);
}

void UEclipseRunewordComponent::UnbindAll()
{
	if (UEclipseInventoryComponent* Inv = Inventory.Get())
	{
		Inv->OnEquipmentChanged.Remove(EquipmentChangedHandle);
	}
	if (UAbilitySystemComponent* ASC = GetASC())
	{
		for (const TPair<uint8, FDelegateHandle>& Pair : EventBindings)
		{
			if (FGameplayEventMulticastDelegate* Delegate = ASC->GenericGameplayEventCallbacks.Find(GetConditionTag(static_cast<Eclipse::ERitualCondition>(Pair.Key))))
			{
				Delegate->Remove(Pair.Value);
			}
		}
		if (GetOwner()->HasAuthority())
		{
			for (const TPair<uint8, FGameplayAbilitySpecHandle>& Pair : EffectSpecs)
			{
				ASC->ClearAbility(Pair.Value);
			}
		}
	}
	EventBindings.Reset();
	EffectSpecs.Reset();
}

void UEclipseRunewordComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	UnbindAll();
	Super::EndPlay(Reason);
}
