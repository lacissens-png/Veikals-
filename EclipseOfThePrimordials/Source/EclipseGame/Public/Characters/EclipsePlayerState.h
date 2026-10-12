// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "EclipsePlayerState.generated.h"

class UEclipseClassDefinition;
class UEclipseForgeComponent;
class UEclipseInventoryComponent;

/**
 * Persistent per-player state. Lives across pawn death/respawn, so the
 * inventory, materials, Codex of Power and chosen class belong here rather
 * than on the hero pawn.
 */
UCLASS()
class ECLIPSEGAME_API AEclipsePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AEclipsePlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UEclipseInventoryComponent* GetInventory() const { return Inventory; }
	UEclipseForgeComponent* GetForge() const { return Forge; }

	UFUNCTION(BlueprintPure, Category = "Class")
	UEclipseClassDefinition* GetClassDefinition() const { return ClassDefinition; }

	/** Server: set at character select / load. */
	void SetClassDefinition(UEclipseClassDefinition* InClass) { ClassDefinition = InClass; }

	/** Eclipse::EClassId as a byte, or Eclipse::AnyClass if unset. */
	uint8 GetClassId() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEclipseInventoryComponent> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEclipseForgeComponent> Forge;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Class")
	TObjectPtr<UEclipseClassDefinition> ClassDefinition;
};
