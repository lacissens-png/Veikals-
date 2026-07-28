#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SODialogueComponent.generated.h"

class USODialogueNode;
class ASOCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnDialogueStarted,  USODialogueNode*, FirstNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnDialogueNodeChanged, USODialogueNode*, NewNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSOOnDialogueEnded);

/**
 * Add to any NPC actor to give it a conversation tree.
 * The PlayerController detects this component during Interact (F) and
 * routes input into SelectChoice / AdvanceOrEnd instead of vendor logic.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USODialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USODialogueComponent();

	// ---------- Config ----------

	/** Root node of this NPC's conversation tree. Assign in the editor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<USODialogueNode> EntryNode;

	/** Interaction range (cm) at which the player can start the conversation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (ClampMin = "50.0"))
	float InteractionRadius = 300.0f;

	// ---------- Runtime API ----------

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(ASOCharacter* Participant);

	/** Selects a choice by index. Index -1 (or no choices) advances/ends. */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SelectChoice(int32 ChoiceIndex);

	/** Closes the dialogue immediately (e.g. ESC pressed or player walks away). */
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EndDialogue();

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	bool IsInDialogue() const { return bActive; }

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	USODialogueNode* GetCurrentNode() const { return CurrentNode; }

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	bool IsPlayerInRange(const ASOCharacter* Player) const;

	// ---------- Delegates ----------
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FSOOnDialogueStarted OnDialogueStarted;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FSOOnDialogueNodeChanged OnDialogueNodeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FSOOnDialogueEnded OnDialogueEnded;

	// ---------- BP hooks ----------
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void OnDialogueStartedBP(ASOCharacter* Participant, USODialogueNode* FirstNode);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void OnDialogueNodeChangedBP(USODialogueNode* NewNode);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	void OnDialogueEndedBP();

private:
	UPROPERTY(Transient)
	TObjectPtr<USODialogueNode> CurrentNode;

	bool bActive = false;

	void SetCurrentNode(USODialogueNode* Node);
};
