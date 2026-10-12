// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "EclipseCore/Runewords.h"
#include "GameplayAbilitySpecHandle.h"
#include "Items/EclipseItemDatabase.h"

#include "EclipseRunewordComponent.generated.h"

class UAbilitySystemComponent;
class UEclipseInventoryComponent;
struct FGameplayEventData;

/** Owner-only UI view of an active runeword. */
USTRUCT(BlueprintType)
struct FEclipseRunewordView
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Runeword") int64 ItemGuid = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Runeword") int32 PairIndex = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Runeword") int32 RitualRuneId = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Runeword") int32 EffectRuneId = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Runeword") EEclipseRitualCondition Condition = EEclipseRitualCondition::OnUltimateUsed;
	UPROPERTY(BlueprintReadOnly, Category = "Runeword") EEclipseRuneEffectAction Action = EEclipseRuneEffectAction::TriggerTimeStop;
};

/**
 * RunewordEvaluator bridge for GAS (server only).
 *
 *   Inventory.OnEquipmentChanged ──► Rebuild()
 *        │  core RunewordEvaluator::Rebuild (validates Ritual+Effect pairs)
 *        │  SyncEventBindings(): for every ritual CONDITION with >= 1 binding,
 *        │     ASC->GenericGameplayEventCallbacks[Event.Ritual.X].AddUObject(...)
 *        │  and remove the delegate when the last binding disappears.
 *        └  SyncEffectAbilities(): GiveAbility() for each needed effect action,
 *              SourceObject = this (=> cost/cooldown bypass in UEclipseGameplayAbility),
 *              ClearAbility() for actions no longer socketed.
 *
 *   Gameplay event Event.Ritual.X ──► HandleRitualEvent(X)
 *        └ core EvaluateRuneword() ──► for each invocation:
 *             ASC->TriggerAbilityFromGameplayEvent(effect spec, Param as EventMagnitude)
 *
 * Re-entrancy: an effect may itself cause a ritual (Time Stop applies CC ->
 * CrowdControlApplied). Events raised while dispatching are ignored, so a
 * runeword can never trigger itself in the same call stack; offering costs and
 * cooldowns bound cross-frame chains.
 */
UCLASS(ClassGroup = (Eclipse), meta = (BlueprintSpawnableComponent))
class ECLIPSEGAME_API UEclipseRunewordComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEclipseRunewordComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	/** Server: called by the hero on possession (inventory lives on the PlayerState). */
	void BindToInventory(UEclipseInventoryComponent* InInventory);

	/** Server: re-evaluates socketed runewords. Event-driven; never per frame. */
	void Rebuild();

	UFUNCTION(BlueprintPure, Category = "Runeword")
	TArray<FEclipseRunewordView> GetActiveRunewords() const { return ActiveViews; }

	static FGameplayTag GetConditionTag(Eclipse::ERitualCondition Condition);

private:
	UAbilitySystemComponent* GetASC() const;
	void SyncEventBindings();
	void SyncEffectAbilities(const UEclipseItemDatabase& Db);
	void HandleRitualEvent(const FGameplayEventData* Payload, Eclipse::ERitualCondition Condition);
	void TriggerEffect(const Eclipse::RuneEffectInvocation& Invocation);
	void UnbindAll();

	Eclipse::RunewordEvaluator Evaluator;
	std::vector<Eclipse::RuneEffectInvocation> InvocationScratch;

	TWeakObjectPtr<UEclipseInventoryComponent> Inventory;
	FDelegateHandle EquipmentChangedHandle;

	/** One live delegate per bound ritual condition. */
	TMap<uint8, FDelegateHandle> EventBindings;
	/** One granted ability spec per socketed effect action. */
	TMap<uint8, FGameplayAbilitySpecHandle> EffectSpecs;

	bool bDispatching = false;

	UPROPERTY(Replicated)
	TArray<FEclipseRunewordView> ActiveViews;
};
