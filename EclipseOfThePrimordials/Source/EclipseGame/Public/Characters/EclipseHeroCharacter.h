// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "Classes/EclipseClassDefinition.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"

#include "EclipseHeroCharacter.generated.h"

class UAbilitySystemComponent;
class UEclipseAttributeSet;
class UEclipseRunewordComponent;
class UEclipseStatusComponent;

/**
 * Player avatar. Owns the ASC (Mixed replication: full GE info to the owner,
 * minimal to others) because heroes are long-lived in an ARPG session; the
 * persistent economy (inventory/forge/codex) lives on AEclipsePlayerState.
 */
UCLASS()
class ECLIPSEGAME_API AEclipseHeroCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	static constexpr uint8 HeroTeam = 1;

	AEclipseHeroCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(HeroTeam); }

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintPure, Category = "Class")
	UEclipseClassDefinition* GetClassDefinition() const;

	EEclipseResourceModel GetResourceModel() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;

	UPROPERTY()
	TObjectPtr<UEclipseAttributeSet> Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEclipseStatusComponent> Status;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEclipseRunewordComponent> Runewords;

private:
	void InitAbilityActorInfo();
	void GrantClass(UEclipseClassDefinition* ClassDef);

	bool bClassGranted = false;
};
