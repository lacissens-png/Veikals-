#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SODialogueNode.generated.h"

class USOVassalData;

/**
 * One player-choice option inside a dialogue node.
 * NextNode null means the conversation ends after this choice is picked.
 */
USTRUCT(BlueprintType)
struct FSODialogueChoice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText ChoiceText;

	/** Node to advance to when this choice is selected. Null = end dialogue. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TSoftObjectPtr<USODialogueNode> NextNode;

	/** Optional vassal to recruit when this choice is selected — see USOVassalComponent::RecruitVassal. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TSoftObjectPtr<USOVassalData> VassalReward;
};

/**
 * One screen of dialogue: speaker name, body text, and a list of choices.
 * If Choices is empty the conversation advances / ends automatically when the
 * player presses the Interact key (F) a second time.
 *
 * Build conversation trees by chaining nodes via the NextNode soft references.
 * All assets live in the Content Browser; no hard package dependencies.
 */
UCLASS(BlueprintType)
class SUPREMEOVERLORD_API USODialogueNode : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Shown as the title of the dialogue box. Usually the NPC name. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText SpeakerName;

	/** Body text of this dialogue page. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (MultiLine = "true"))
	FText BodyText;

	/** Player choices. Leave empty for a single-advance prompt ([F] Continue). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TArray<FSODialogueChoice> Choices;
};
