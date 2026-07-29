#include "SODialogueNPC.h"

#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"
#include "SODialogueComponent.h"

ASODialogueNPC::ASODialogueNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = InteractionSphere;

	DialogueComponent = CreateDefaultSubobject<USODialogueComponent>(TEXT("DialogueComponent"));

	// Seed the sphere from the component's own radius rather than a hardcoded
	// literal, so the two can't drift apart (see BeginPlay for the re-sync).
	if (DialogueComponent)
	{
		InteractionSphere->InitSphereRadius(DialogueComponent->InteractionRadius);
	}

#if WITH_EDITORONLY_DATA
	EditorSprite = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("EditorSprite"));
	if (EditorSprite)
	{
		EditorSprite->SetupAttachment(RootComponent);
		EditorSprite->bIsScreenSizeScaled = true;
		EditorSprite->ScreenSize          = 0.0025f;
	}
#endif
}

void ASODialogueNPC::BeginPlay()
{
	Super::BeginPlay();

	// The sphere is purely a visualization - USODialogueComponent::IsPlayerInRange
	// does its own distance check against InteractionRadius. Re-sync here (the
	// same way ASOVendorNPC does) so a designer raising/lowering that radius
	// doesn't leave the drawn sphere showing a range the game no longer uses.
	if (InteractionSphere && DialogueComponent)
	{
		InteractionSphere->SetSphereRadius(DialogueComponent->InteractionRadius);
	}
}
