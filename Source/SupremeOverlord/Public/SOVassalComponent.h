#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOVassalComponent.generated.h"

class ASOCharacter;
class ASOVassalActor;
class USOVassalData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnVassalRecruited, USOVassalData*, Vassal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnVassalSummoned,  USOVassalData*, Vassal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSOOnVassalDismissed);

/**
 * Manages the player's roster of recruited named vassals (Overlord's "Floor
 * Guardians") — distinct from USOSummonComponent's disposable minion army.
 * Only one vassal is summoned ("active") at a time; summoning grants the
 * caster the vassal's passive buff (USOVassalData::BuffType), queried live
 * via GetIncomingDamageMultiplier()/GetAttackCooldownMultiplier() rather than
 * mutating stored stats — so the buff turns off automatically the instant
 * the vassal dies or is dismissed, with nothing to revert.
 *
 * Recruitment isn't autonomous — call RecruitVassal(Data) from wherever the
 * game grants it (a dialogue choice, a quest reward, a boss defeat).
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOVassalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOVassalComponent();

	// ---------- Recruitment ----------

	/** Adds Vassal to the recruited roster (deduped). Returns false if already recruited or null. */
	UFUNCTION(BlueprintCallable, Category = "Vassal")
	bool RecruitVassal(USOVassalData* Vassal);

	UFUNCTION(BlueprintPure, Category = "Vassal")
	bool IsVassalRecruited(USOVassalData* Vassal) const;

	UFUNCTION(BlueprintPure, Category = "Vassal")
	const TArray<TObjectPtr<USOVassalData>>& GetRecruitedVassals() const { return RecruitedVassals; }

	/** Load support: replaces the recruited roster wholesale (no OnVassalRecruited replay per entry). */
	UFUNCTION(BlueprintCallable, Category = "Vassal")
	void RestoreRecruitedVassals(const TArray<USOVassalData*>& Vassals);

	/** Moves the selection to the next recruited vassal, wrapping around. No-op with 0-1 recruited. */
	UFUNCTION(BlueprintCallable, Category = "Vassal")
	void CycleVassalSelection();

	/** The vassal SummonSelectedVassal would summon next. Null if none recruited. */
	UFUNCTION(BlueprintPure, Category = "Vassal")
	USOVassalData* GetSelectedVassal() const;

	// ---------- Summon / Dismiss ----------

	/** Summons the currently selected recruited vassal, dismissing any previously active one first. */
	UFUNCTION(BlueprintCallable, Category = "Vassal")
	bool SummonSelectedVassal();

	/** Destroys the active vassal. No-op if none active. */
	UFUNCTION(BlueprintCallable, Category = "Vassal")
	void DismissVassal();

	UFUNCTION(BlueprintPure, Category = "Vassal")
	bool HasActiveVassal() const;

	UFUNCTION(BlueprintPure, Category = "Vassal")
	ASOVassalActor* GetActiveVassalActor() const { return ActiveVassalActor.Get(); }

	UFUNCTION(BlueprintPure, Category = "Vassal")
	USOVassalData* GetActiveVassalData() const { return HasActiveVassal() ? ActiveVassalData : nullptr; }

	/** Called by ASOVassalActor::HandleDeath. Clears the active reference so the buff getters fall back to neutral. */
	void HandleVassalDied(ASOVassalActor* Vassal);

	// ---------- Buffs — queried live by combat code, never stored/mutated ----------

	/** Incoming-damage multiplier from an active DamageReduction vassal (1.0 = no effect). */
	UFUNCTION(BlueprintPure, Category = "Vassal")
	float GetIncomingDamageMultiplier() const;

	/** Attack-cooldown multiplier from an active AttackSpeed vassal (1.0 = no effect). */
	UFUNCTION(BlueprintPure, Category = "Vassal")
	float GetAttackCooldownMultiplier() const;

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "Vassal")
	FSOOnVassalRecruited OnVassalRecruited;

	UPROPERTY(BlueprintAssignable, Category = "Vassal")
	FSOOnVassalSummoned OnVassalSummoned;

	UPROPERTY(BlueprintAssignable, Category = "Vassal")
	FSOOnVassalDismissed OnVassalDismissed;

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<USOVassalData>> RecruitedVassals;

	int32 CurrentSelectionIndex = 0;

	UPROPERTY(Transient)
	TObjectPtr<USOVassalData> ActiveVassalData;

	UPROPERTY(Transient)
	TWeakObjectPtr<ASOVassalActor> ActiveVassalActor;
};
