#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SOItemAffix.h"
#include "SOGemData.generated.h"

class UTexture2D;

/** Classic ARPG gem quality ladder — purely presentational, magnitude lives on Value. */
UENUM(BlueprintType)
enum class ESOGemTier : uint8
{
	Chipped   UMETA(DisplayName = "Chipped"),
	Flawed    UMETA(DisplayName = "Flawed"),
	Regular   UMETA(DisplayName = "Regular"),
	Flawless  UMETA(DisplayName = "Flawless"),
	Perfect   UMETA(DisplayName = "Perfect"),
};

/**
 * Socketable gem. USOEquipmentComponent::SocketGem applies Stat/Value to the
 * target item via USOLootRoller::ApplyAffix — the exact same routine used for
 * randomly-rolled affixes — so a socketed gem is indistinguishable from a
 * rolled bonus once applied.
 */
UCLASS(Blueprintable, BlueprintType)
class SUPREMEOVERLORD_API USOGemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem")
	ESOGemTier Tier = ESOGemTier::Regular;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem")
	ESOAffixStat Stat = ESOAffixStat::PrimaryDamage;

	/** Base magnitude before the Tier multiplier — same units as FSOItemAffix::Value (flat for damage/health/mana, fractional for speed/reduction/attack speed). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gem")
	float Value = 5.0f;

	/** Value scaled by Tier (Chipped = 0.5x ... Perfect = 2x) — what USOEquipmentComponent::SocketGem actually applies. */
	UFUNCTION(BlueprintPure, Category = "Gem")
	float GetEffectiveValue() const;
};
