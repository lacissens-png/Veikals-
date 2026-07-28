#include "SODialogueComponent.h"

#include "SOCharacter.h"
#include "SODialogueNode.h"

USODialogueComponent::USODialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USODialogueComponent::StartDialogue(ASOCharacter* Participant)
{
	if (!EntryNode)
	{
		return;
	}

	bActive = true;
	SetCurrentNode(EntryNode);

	OnDialogueStarted.Broadcast(CurrentNode);
	OnDialogueStartedBP(Participant, CurrentNode);
}

void USODialogueComponent::SelectChoice(int32 ChoiceIndex)
{
	if (!bActive || !CurrentNode)
	{
		return;
	}

	const TArray<FSODialogueChoice>& Choices = CurrentNode->Choices;

	if (Choices.Num() == 0)
	{
		// No choices — this was an auto-advance / single-prompt node.
		EndDialogue();
		return;
	}

	if (!Choices.IsValidIndex(ChoiceIndex))
	{
		return;
	}

	const FSODialogueChoice& Chosen = Choices[ChoiceIndex];
	USODialogueNode* Next = Chosen.NextNode.LoadSynchronous();

	if (!Next)
	{
		EndDialogue();
	}
	else
	{
		SetCurrentNode(Next);
	}
}

void USODialogueComponent::EndDialogue()
{
	bActive     = false;
	CurrentNode = nullptr;
	OnDialogueEnded.Broadcast();
	OnDialogueEndedBP();
}

bool USODialogueComponent::IsPlayerInRange(const ASOCharacter* Player) const
{
	if (!Player)
	{
		return false;
	}
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return false;
	}
	return FVector::DistSquared(Player->GetActorLocation(), Owner->GetActorLocation())
	       <= FMath::Square(InteractionRadius);
}

void USODialogueComponent::SetCurrentNode(USODialogueNode* Node)
{
	CurrentNode = Node;
	if (Node)
	{
		OnDialogueNodeChanged.Broadcast(Node);
		OnDialogueNodeChangedBP(Node);
	}
}
