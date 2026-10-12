// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"

#include "EclipseBossCharacter.generated.h"

class UAbilitySystemComponent;
class UEclipseAttributeSet;
class UEclipseStaggerComponent;
class UEclipseStatusComponent;

/**
 * Boss actor. Unlike swarm mobs, bosses are full actors with GAS: they need
 * scripted phases (abilities), precise hit reactions and a Stagger Meter.
 * Crowd control NEVER disables a boss directly - it fills the meter
 * (UEclipseStatusComponent Tier = Boss routes CC to UEclipseStaggerComponent).
 */
UCLASS()
class ECLIPSEGAME_API AEclipseBossCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	static constexpr uint8 EnemyTeam = 2;

	AEclipseBossCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(EnemyTeam); }

protected:
	virtual void BeginPlay() override;

	/** Server: called once when Health reaches 0. Default destroys after a delay; BP adds loot/cinematic. */
	UFUNCTION(BlueprintNativeEvent, Category = "Boss")
	void OnBossDefeated(AActor* Killer);

	UPROPERTY(EditDefaultsOnly, Category = "Boss")
	float MaxHealth = 250000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Boss")
	float Armor = 1500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;

	UPROPERTY()
	TObjectPtr<UEclipseAttributeSet> Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEclipseStatusComponent> Status;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEclipseStaggerComponent> Stagger;
};
