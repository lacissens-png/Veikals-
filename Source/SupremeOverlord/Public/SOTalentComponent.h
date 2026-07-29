#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOTalentComponent.generated.h"

class USOTalentNode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnTalentPointsChanged,
	int32, NewTotal,
	int32, Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnTalentUnlocked,
	USOTalentNode*, Node);

/**
 * Passive talent-tree pool. Nodes are USOTalentNode assets referenced by
 * hard pointer; unlock costs live on the node so the tree can be
 * rebalanced without recompiling. Level-ups bank TalentPointsPerLevel
 * points; the character (or a UMG widget) spends them via UnlockNode.
 */
UCLASS(ClassGroup = (SupremeOverlord), meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOTalentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOTalentComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Talents", meta = (ClampMin = "0", UIMin = "0", UIMax = "10"))
	int32 TalentPointsPerLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Talents", meta = (ClampMin = "0", UIMin = "0", UIMax = "20"))
	int32 StartingTalentPoints = 0;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Talents")
	FSOOnTalentPointsChanged OnTalentPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Talents")
	FSOOnTalentUnlocked OnTalentUnlocked;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Talents")
	int32 GetTalentPoints() const { return AvailableTalentPoints; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Talents")
	bool IsUnlocked(USOTalentNode* Node) const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Talents")
	bool CanUnlock(USOTalentNode* Node, FString& OutReason) const;

	const TArray<TObjectPtr<USOTalentNode>>& GetUnlockedNodes() const { return UnlockedNodes; }

	/** Called by ASOCharacter on level-up (and by save/load) to add points. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Talents")
	void GrantPoints(int32 Amount);

	/** Attempts to unlock a node. Spends points, applies effects, broadcasts events. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Talents")
	bool UnlockNode(USOTalentNode* Node);

	/**
	 * Reverts every unlocked node's stat effects and refunds their PointCost.
	 * No-op (returns false) if nothing is unlocked. Gold cost, if any, is the
	 * caller's responsibility — see ASOCharacter::RespecTalents.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Talents")
	bool RespecAll();

	/**
	 * Load support: clears any current unlocks, then unlocks exactly Nodes (in
	 * order, applying their stat effects) and sets AvailableTalentPoints to
	 * Points directly. Skips CanUnlock's prerequisite/cost checks since a
	 * save file is assumed to represent a previously-valid state.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Talents")
	void RestoreFromSave(const TArray<USOTalentNode*>& Nodes, int32 Points);

private:
	void ApplyNodeEffects(USOTalentNode* Node);
	void RevertNodeEffects(USOTalentNode* Node);

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Talents", Transient)
	TArray<TObjectPtr<USOTalentNode>> UnlockedNodes;

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Talents", Transient)
	int32 AvailableTalentPoints = 0;
};
