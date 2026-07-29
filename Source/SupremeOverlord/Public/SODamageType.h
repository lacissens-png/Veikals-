#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "SODamageType.generated.h"

/** High-level damage categories used for resistances, VFX, and combat log tinting. */
UENUM(BlueprintType)
enum class ESODamageCategory : uint8
{
	Physical UMETA(DisplayName = "Physical"),
	Shadow   UMETA(DisplayName = "Shadow"),
	Fire     UMETA(DisplayName = "Fire"),
	Frost    UMETA(DisplayName = "Frost"),
	Necrotic UMETA(DisplayName = "Necrotic"),
	Holy     UMETA(DisplayName = "Holy"),
	TrueDamage UMETA(DisplayName = "True")
};

/**
 * Base damage type for Supreme Overlord.
 * Extend this to make per-school damage types (BP_DamageType_Shadow, etc.)
 * that resistances and VFX pipelines can key off.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USODamageType : public UDamageType
{
	GENERATED_BODY()

public:
	USODamageType();

	/** Which school this damage belongs to. Resistances / vulnerabilities match on this. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SupremeOverlord|Damage")
	ESODamageCategory Category = ESODamageCategory::Physical;

	/** Rough hit reaction intensity, 0..1. UI / hit-react animation can multiply into this. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SupremeOverlord|Damage", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HitReactStrength = 0.5f;

	/** True = damage ignores IncomingDamageMultiplier on the health component. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SupremeOverlord|Damage")
	bool bIgnoresResistances = false;
};
