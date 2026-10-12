// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CoreMinimal.h"

#include "EclipseAttributeSet.generated.h"

#define ECLIPSE_ATTRIBUTE_ACCESSORS(ClassName, PropertyName)           \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)         \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                       \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                       \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_MULTICAST_DELEGATE_TwoParams(FEclipseOnOutOfHealth, AActor* /*Victim*/, AActor* /*Killer*/);

/**
 * Attributes for GAS-driven actors (heroes, bosses). Swarm mobs do NOT use
 * GAS; their health lives in the swarm's SoA arrays (100+ ASCs would cost far
 * more memory and replication bandwidth than the gameplay needs).
 *
 * Damage flow (server only):
 *   GE_Damage (UEclipseDamageExecution: armor & penetration)
 *     -> IncomingDamage (meta, never replicated)
 *     -> PostGameplayEffectExecute: x status DamageTakenMultiplier (Vulnerable)
 *     -> Health
 *     -> HeavyDamageTaken ritual event / OnOutOfHealth
 */
UCLASS()
class ECLIPSEGAME_API UEclipseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEclipseAttributeSet();

	ECLIPSE_ATTRIBUTE_ACCESSORS(UEclipseAttributeSet, Health);
	ECLIPSE_ATTRIBUTE_ACCESSORS(UEclipseAttributeSet, MaxHealth);
	ECLIPSE_ATTRIBUTE_ACCESSORS(UEclipseAttributeSet, Armor);
	ECLIPSE_ATTRIBUTE_ACCESSORS(UEclipseAttributeSet, ClassResource);
	ECLIPSE_ATTRIBUTE_ACCESSORS(UEclipseAttributeSet, MaxClassResource);
	ECLIPSE_ATTRIBUTE_ACCESSORS(UEclipseAttributeSet, IncomingDamage);

	/** Fired once on the server when Health reaches 0. */
	FEclipseOnOutOfHealth OnOutOfHealth;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:
	UFUNCTION() void OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_Armor(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_ClassResource(const FGameplayAttributeData& OldValue);
	UFUNCTION() void OnRep_MaxClassResource(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vitals", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vitals", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Defense", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Armor;

	/** Biomass / Temporal Strain / Seismic Pressure / Lunar Essence depending on class. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ClassResource, Category = "Resource", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ClassResource;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxClassResource, Category = "Resource", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxClassResource;

	/** Meta attribute: written by the damage execution, consumed immediately. */
	UPROPERTY(BlueprintReadOnly, Category = "Meta", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData IncomingDamage;

	bool bOutOfHealth = false;
};
