#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOAttributesComponent.generated.h"

UENUM(BlueprintType)
enum class ESOAttribute : uint8
{
	Strength  UMETA(DisplayName = "Strength"),
	Intellect UMETA(DisplayName = "Intellect"),
	Vitality  UMETA(DisplayName = "Vitality")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSOOnAttributeChanged,
	USOAttributesComponent*, OwningComponent,
	ESOAttribute,            Attribute,
	int32,                   NewValue,
	int32,                   Delta);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnUnspentPointsChanged,
	USOAttributesComponent*, OwningComponent,
	int32,                   NewUnspentPoints);

/**
 * Passive-attribute pool. Level-ups grant PointsPerLevel unspent points,
 * which the player can allocate into Strength / Intellect / Vitality.
 * Every allocation calls back into the owning ASOCharacter so effective
 * stats (damage, mana, HP) update immediately.
 */
UCLASS(ClassGroup = (SupremeOverlord), meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOAttributesComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Attributes", meta = (ClampMin = "0", UIMin = "0", UIMax = "50"))
	int32 StartingStrength = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Attributes", meta = (ClampMin = "0", UIMin = "0", UIMax = "50"))
	int32 StartingIntellect = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Attributes", meta = (ClampMin = "0", UIMin = "0", UIMax = "50"))
	int32 StartingVitality = 5;

	/** Points granted per level-up. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Attributes", meta = (ClampMin = "0", UIMin = "0", UIMax = "10"))
	int32 PointsPerLevel = 3;

	// -------- Per-point effects (BP-facing, so multipliers can be tuned live). --------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Attributes|Effects", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float PrimaryDamagePerStrength = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Attributes|Effects", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "10.0"))
	float ShadowBoltDamagePerIntellect = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Attributes|Effects", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "20.0"))
	float MaxManaPerIntellect = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Attributes|Effects", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "20.0"))
	float MaxHealthPerVitality = 5.0f;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Attributes")
	FSOOnAttributeChanged OnAttributeChanged;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Attributes")
	FSOOnUnspentPointsChanged OnUnspentPointsChanged;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Attributes")
	int32 GetAttribute(ESOAttribute Attribute) const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Attributes")
	int32 GetUnspentPoints() const { return UnspentPoints; }

	/** Called by the owning character on level-up to bank more points. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Attributes")
	void GrantPoints(int32 Amount);

	/** Attempts to spend one point into Attribute. Returns true if it succeeded. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Attributes")
	bool AllocatePoint(ESOAttribute Attribute);

private:
	void ApplyAttributeChange(ESOAttribute Attribute, int32 Delta);

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Attributes", Transient)
	int32 Strength = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Attributes", Transient)
	int32 Intellect = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Attributes", Transient)
	int32 Vitality = 0;

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Attributes", Transient)
	int32 UnspentPoints = 0;
};
