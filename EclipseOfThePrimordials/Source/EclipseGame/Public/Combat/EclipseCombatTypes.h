// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// UE-facing mirrors of the core combat enums/structs (Blueprint & replication
// friendly) plus zero-cost converters. static_asserts keep the enum values in
// lock-step with EclipseCore so a reorder on either side fails the build.

#pragma once

#include "CoreMinimal.h"
#include "EclipseCore/StatusEffects.h"

#include "EclipseCombatTypes.generated.h"

class AActor;

UENUM(BlueprintType)
enum class EEclipseStatusEffect : uint8
{
	Vulnerable,
	Unstoppable,
	Bleed,
	Burn,
	Freeze,
	Stun,
	Slow,
	Knockback
};

UENUM(BlueprintType)
enum class EEclipseMobTier : uint8
{
	TrashSwarmer,
	Captain,
	PrimordialBrute,
	Boss,
	Player
};

/** Field affinities for the environment layer (Eclipse Puppeteer "Creation"). */
UENUM(BlueprintType)
enum class EEclipseAffinity : uint8
{
	Neutral,
	Blood,
	Temporal,
	Seismic,
	Lunar,
	Void
};

static_assert(static_cast<uint8>(EEclipseStatusEffect::Knockback) + 1 == Eclipse::StatusEffectCount, "Status enum mirror out of sync");
static_assert(static_cast<uint8>(EEclipseStatusEffect::Stun) == static_cast<uint8>(Eclipse::EStatusEffect::Stun), "Status enum mirror out of sync");
static_assert(static_cast<uint8>(EEclipseMobTier::Player) + 1 == static_cast<uint8>(Eclipse::EMobTier::Count), "Tier enum mirror out of sync");

/** Designer-facing status request (ability defaults, rune effects, mob attacks). */
USTRUCT(BlueprintType)
struct FEclipseStatusSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	EEclipseStatusEffect Effect = EEclipseStatusEffect::Vulnerable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (ClampMin = "0"))
	float Duration = 0.f;

	/** Vulnerable: bonus fraction (>= 0.2). DoTs: DPS. Slow: fraction. Knockback: impulse (cm/s). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float Magnitude = 0.f;

	bool IsValid() const { return Duration > 0.f || Effect == EEclipseStatusEffect::Knockback; }

	Eclipse::StatusApplication ToCore(uint32 SourceId = 0) const
	{
		Eclipse::StatusApplication App;
		App.Effect = static_cast<Eclipse::EStatusEffect>(Effect);
		App.Duration = Duration;
		App.Magnitude = Magnitude;
		App.SourceId = SourceId;
		return App;
	}
};

/** A single hit description shared by the swarm path and the GAS path. */
USTRUCT(BlueprintType)
struct FEclipseDamageSpec
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Amount = 0.f;

	/** 0..1 fraction of target armor ignored. Reality Slash = 0.5. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (ClampMin = "0", ClampMax = "1"))
	float ArmorPenetration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EEclipseAffinity Affinity = EEclipseAffinity::Neutral;

	/** Kill outright if the core execute rule allows it for the target's tier. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bExecute = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (EditCondition = "bExecute"))
	float ExecuteThreshold = 0.2f;

	/** Internal: false for Puppet-link mirrored damage so it is never re-shared. */
	bool bCanBeShared = true;

	/** Who gets kill credit / ritual events. Not replicated; server-only data. */
	TWeakObjectPtr<AActor> Instigator;
};

inline Eclipse::EMobTier ToCore(EEclipseMobTier Tier) { return static_cast<Eclipse::EMobTier>(Tier); }
