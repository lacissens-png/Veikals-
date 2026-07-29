#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SODialogueNPC.generated.h"

class USODialogueComponent;
class USphereComponent;
class UBillboardComponent;

/**
 * Minimal NPC actor that owns a SODialogueComponent.
 * Place in the level, set a DisplayName and wire up the EntryNode on
 * the component — the player can then talk to it with the F key.
 *
 * For richer NPCs (vendor + dialogue), add SODialogueComponent to
 * ASOVendorNPC in a Blueprint subclass.
 */
UCLASS(Blueprintable)
class SUPREMEOVERLORD_API ASODialogueNPC : public AActor
{
	GENERATED_BODY()

public:
	ASODialogueNPC();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<USODialogueComponent> DialogueComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<USphereComponent> InteractionSphere;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "Editor")
	TObjectPtr<UBillboardComponent> EditorSprite;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText DisplayName = FText::FromString(TEXT("NPC"));
};
