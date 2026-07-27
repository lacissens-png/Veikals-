#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOEliteComponent.generated.h"

class ASOEnemyCharacter;

/** Bitflag affix set — an elite can carry any combination. Rendered as a checkbox row in the editor. */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESOEliteAffix : uint8
{
	None        = 0        UMETA(Hidden),
	Fortified   = 1 << 0   UMETA(DisplayName = "Fortified (2.5x HP)"),
	Vicious     = 1 << 1   UMETA(DisplayName = "Vicious (1.5x Damage)"),
	Swift       = 1 << 2   UMETA(DisplayName = "Swift (1.4x Speed)"),
	Enriched    = 1 << 3   UMETA(DisplayName = "Enriched (2x Loot)"),
	FarSighted  = 1 << 4   UMETA(DisplayName = "Far-Sighted (1.5x Sight)")
};
ENUM_CLASS_FLAGS(ESOEliteAffix);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnEliteAffixesApplied, int32, AffixMask);

/**
 * Attach to any ASOEnemyCharacter (in a Blueprint subclass or at spawn time)
 * to mutate its stats before BeginPlay resolves. Applies in Owner's
 * OnConstruction so the changes seed the health/mana components too.
 *
 * Affix bits can be authored fixed in the editor, or randomised at spawn
 * time via RollRandomAffixes for wave-spawner elites.
 */
UCLASS(ClassGroup = (SupremeOverlord), meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOEliteComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOEliteComponent();

protected:
	virtual void OnRegister() override;

public:
	/**
	 * Bitflags mask of active affixes.
	 * Editor renders it as a checkbox list because the enum is marked Bitflags.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Elite", meta = (Bitmask, BitmaskEnum = "/Script/SupremeOverlord.ESOEliteAffix"))
	int32 AffixMask = 0;

	/** If true, ignore AffixMask and roll RollCount random unique affixes on BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Elite|Random")
	bool bRandomizeOnBeginPlay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Elite|Random", meta = (ClampMin = "1", UIMin = "1", UIMax = "5"))
	int32 RollCount = 2;

	// Per-affix multipliers so designers can rebalance without recompiling.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Elite|Multipliers", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "10.0"))
	float FortifiedHealthMult = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Elite|Multipliers", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "10.0"))
	float ViciousDamageMult = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Elite|Multipliers", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "5.0"))
	float SwiftSpeedMult = 1.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Elite|Multipliers", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "10.0"))
	float EnrichedLootMult = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Elite|Multipliers", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "5.0"))
	float FarSightedMult = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Elite|Visual")
	FLinearColor EliteAuraColor = FLinearColor(1.0f, 0.35f, 0.15f, 1.0f);

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Elite")
	FSOOnEliteAffixesApplied OnAffixesApplied;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Elite")
	bool HasAffix(ESOEliteAffix Affix) const { return (AffixMask & static_cast<int32>(Affix)) != 0; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Elite")
	int32 GetAffixMask() const { return AffixMask; }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Elite")
	bool IsElite() const { return AffixMask != 0; }

	/** Applies the current AffixMask to the owning enemy. Called automatically on register + BeginPlay. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Elite")
	void ApplyAffixesToOwner();

	/** Chooses RollCount unique random affixes, overwriting AffixMask. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Elite")
	void RollRandomAffixes();
};
