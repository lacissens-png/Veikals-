// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "EclipseCore/Aspects.h"
#include "EclipseCore/Items.h"
#include "EclipseCore/Runewords.h"
#include "Engine/DataAsset.h"
#include "Items/EclipseItemTypes.h"

#include "EclipseItemDatabase.generated.h"

UENUM(BlueprintType)
enum class EEclipseAffixStat : uint8
{
	Strength, Intelligence, Dexterity, Willpower, MaxLife, Armor, CritChance, CritDamage, AttackSpeed,
	CooldownReduction, ResourceGeneration, DamageToVulnerable, DamageToStaggered, CrowdControlDuration,
	MovementSpeed, AllResistance, UltimateDamage
};
static_assert(static_cast<uint8>(EEclipseAffixStat::UltimateDamage) + 1 == Eclipse::AffixStatCount, "Affix stat mirror out of sync");

UENUM(BlueprintType)
enum class EEclipseRuneType : uint8 { Ritual, Effect };

UENUM(BlueprintType)
enum class EEclipseRitualCondition : uint8 { OnUltimateUsed, OnCrowdControlApplied, OnHeavyDamageTaken, OnEnemyKilled, OnResourceSpent };
static_assert(static_cast<uint8>(EEclipseRitualCondition::OnResourceSpent) + 1 == Eclipse::RitualConditionCount, "Ritual mirror out of sync");

UENUM(BlueprintType)
enum class EEclipseRuneEffectAction : uint8 { TriggerTimeStop, SpawnFleshColossus, CastRealitySlash, GainUnstoppable };
static_assert(static_cast<uint8>(EEclipseRuneEffectAction::GainUnstoppable) + 1 == static_cast<uint8>(Eclipse::ERuneEffectAction::Count), "Rune effect mirror out of sync");

USTRUCT(BlueprintType)
struct FEclipseAffixRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Affix", meta = (ClampMin = "1")) int32 Id = 1;
	UPROPERTY(EditAnywhere, Category = "Affix") EEclipseAffixStat Stat = EEclipseAffixStat::Strength;
	UPROPERTY(EditAnywhere, Category = "Affix") float MinValue = 0.f;
	UPROPERTY(EditAnywhere, Category = "Affix") float MaxValue = 0.f;
	/** Empty = all slots. */
	UPROPERTY(EditAnywhere, Category = "Affix") TArray<EEclipseItemSlot> AllowedSlots;
	UPROPERTY(EditAnywhere, Category = "Affix", meta = (ClampMin = "1")) int32 Weight = 100;
	UPROPERTY(EditAnywhere, Category = "Affix", meta = (ClampMin = "1")) int32 MinItemLevel = 1;
	UPROPERTY(EditAnywhere, Category = "Affix") FText DisplayFormat;
};

USTRUCT(BlueprintType)
struct FEclipseAspectRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Aspect", meta = (ClampMin = "1")) int32 Id = 1;
	UPROPERTY(EditAnywhere, Category = "Aspect") FText DisplayName;
	UPROPERTY(EditAnywhere, Category = "Aspect") TArray<EEclipseItemSlot> AllowedSlots;
	UPROPERTY(EditAnywhere, Category = "Aspect") float MinValue = 0.f;
	UPROPERTY(EditAnywhere, Category = "Aspect") float MaxValue = 0.f;
	/** -1 = any class, else Eclipse::EClassId. */
	UPROPERTY(EditAnywhere, Category = "Aspect") int32 ClassRestriction = -1;
	/** Gameplay effect granted while an item with this aspect is equipped. */
	UPROPERTY(EditAnywhere, Category = "Aspect") TSoftClassPtr<UGameplayEffect> EquippedEffect;
};

USTRUCT(BlueprintType)
struct FEclipseRuneRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Rune", meta = (ClampMin = "1")) int32 Id = 1;
	UPROPERTY(EditAnywhere, Category = "Rune") FText DisplayName;
	UPROPERTY(EditAnywhere, Category = "Rune") EEclipseRuneType Type = EEclipseRuneType::Ritual;

	UPROPERTY(EditAnywhere, Category = "Rune|Ritual", meta = (EditCondition = "Type == EEclipseRuneType::Ritual"))
	EEclipseRitualCondition Condition = EEclipseRitualCondition::OnUltimateUsed;
	UPROPERTY(EditAnywhere, Category = "Rune|Ritual", meta = (EditCondition = "Type == EEclipseRuneType::Ritual"))
	float Threshold = 0.f;
	UPROPERTY(EditAnywhere, Category = "Rune|Ritual", meta = (EditCondition = "Type == EEclipseRuneType::Ritual"))
	int32 OfferingGenerated = 0;

	UPROPERTY(EditAnywhere, Category = "Rune|Effect", meta = (EditCondition = "Type == EEclipseRuneType::Effect"))
	EEclipseRuneEffectAction Action = EEclipseRuneEffectAction::TriggerTimeStop;
	UPROPERTY(EditAnywhere, Category = "Rune|Effect", meta = (EditCondition = "Type == EEclipseRuneType::Effect"))
	float Param = 0.f;
	UPROPERTY(EditAnywhere, Category = "Rune|Effect", meta = (EditCondition = "Type == EEclipseRuneType::Effect"))
	int32 OfferingCost = 0;
	UPROPERTY(EditAnywhere, Category = "Rune|Effect", meta = (EditCondition = "Type == EEclipseRuneType::Effect"))
	float Cooldown = 0.f;
};

/**
 * Single source of truth for affix / aspect / rune data. Designers edit rows;
 * at load the asset builds the immutable core tables used by the server rules.
 * The built tables are plain C++ members (not UPROPERTY): they are derived data
 * and never serialized.
 */
UCLASS(BlueprintType)
class ECLIPSEGAME_API UEclipseItemDatabase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Affixes") TArray<FEclipseAffixRow> Affixes;
	UPROPERTY(EditDefaultsOnly, Category = "Aspects") TArray<FEclipseAspectRow> Aspects;
	UPROPERTY(EditDefaultsOnly, Category = "Runes") TArray<FEclipseRuneRow> Runes;

	/** Rune effect action -> ability executed when the runeword fires. */
	UPROPERTY(EditDefaultsOnly, Category = "Runes")
	TMap<EEclipseRuneEffectAction, TSubclassOf<UGameplayAbility>> RuneEffectAbilities;

	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	const Eclipse::AffixPool& GetAffixPool() const { return CoreAffixPool; }
	const Eclipse::AspectRegistry& GetAspectRegistry() const { return CoreAspects; }
	const Eclipse::RuneDatabase& GetRuneDatabase() const { return CoreRunes; }

	static UEclipseItemDatabase* Get();

private:
	void BuildCoreTables();
	static Eclipse::u32 SlotsToMask(const TArray<EEclipseItemSlot>& Slots);

	Eclipse::AffixPool CoreAffixPool;
	Eclipse::AspectRegistry CoreAspects;
	Eclipse::RuneDatabase CoreRunes;
};
