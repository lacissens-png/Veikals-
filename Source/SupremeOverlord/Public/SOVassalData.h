#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SOVassalData.generated.h"

class UTexture2D;
class ASOVassalActor;

/** Passive buff a vassal grants the summoning character while active, summoned, and alive. */
UENUM(BlueprintType)
enum class ESOVassalBuffType : uint8
{
	None            UMETA(DisplayName = "None"),
	DamageReduction UMETA(DisplayName = "Damage Reduction"),
	AttackSpeed     UMETA(DisplayName = "Attack Speed"),
};

/**
 * Identity + stats for one recruitable named vassal — Overlord's "Floor
 * Guardians" fantasy: a small roster of unique, named companions the player
 * recruits through the world (dialogue, quests, boss defeats) and calls on
 * one at a time via USOVassalComponent. Distinct from USOSummonComponent's
 * anonymous, disposable minion army — a vassal is a single, more powerful
 * unit with its own identity and a passive buff for its summoner.
 *
 * Create one asset per vassal (e.g. DA_Vassal_Bloodfang) in the Content
 * Browser; RequiredLevel gates when it can be recruited.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOVassalData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal|Presentation")
	FText VassalName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal|Presentation")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal|Presentation", meta = (MultiLine = "true"))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal|Presentation")
	TObjectPtr<UTexture2D> Icon;

	/** Actor class spawned when this vassal is summoned. Assign a BP_Vassal subclass (or ASOVassalActor directly). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal")
	TSubclassOf<ASOVassalActor> ActorClass;

	/** Player level required before this vassal can be recruited. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal", meta = (ClampMin = "1", UIMin = "1", UIMax = "50"))
	int32 RequiredLevel = 1;

	// ---------- Combat stats — applied to the spawned ASOVassalActor via InitializeFromData ----------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal|Stats", meta = (ClampMin = "1.0"))
	float MaxHealth = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal|Stats", meta = (ClampMin = "0.0"))
	float AttackDamage = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal|Stats", meta = (ClampMin = "50.0"))
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal|Stats", meta = (ClampMin = "0.1"))
	float AttackCooldown = 1.0f;

	// ---------- Passive buff granted to the summoner while this vassal is active and alive ----------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal|Buff")
	ESOVassalBuffType BuffType = ESOVassalBuffType::None;

	/** Fractional magnitude of BuffType (0.15 = 15% less damage taken, or 15% faster attacks). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vassal|Buff", meta = (ClampMin = "0.0", ClampMax = "0.9"))
	float BuffMagnitude = 0.15f;
};
