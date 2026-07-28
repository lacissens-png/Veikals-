#include "SODialogueNPC.h"

#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"
#include "SODialogueComponent.h"

ASODialogueNPC::ASODialogueNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetSphereRadius(300.0f);
	InteractionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = InteractionSphere;

	DialogueComponent = CreateDefaultSubobject<USODialogueComponent>(TEXT("DialogueComponent"));

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
