// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "EclipseGameSettings.generated.h"

class UGameplayEffect;
class UEclipseItemDatabase;

/**
 * Project-wide tunables (Project Settings > Game > Eclipse).
 * Soft references: nothing is loaded until first use, keeping startup memory flat.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Eclipse"))
class ECLIPSEGAME_API UEclipseGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Instant GE using UEclipseDamageExecution. Used whenever damage hits an ASC actor. */
	UPROPERTY(Config, EditAnywhere, Category = "Combat")
	TSoftClassPtr<UGameplayEffect> MobDamageEffect;

	UPROPERTY(Config, EditAnywhere, Category = "Items")
	TSoftObjectPtr<UEclipseItemDatabase> ItemDatabase;

	/** Rate at which swarm snapshots are pushed to clients. */
	UPROPERTY(Config, EditAnywhere, Category = "Swarm", meta = (ClampMin = "2", ClampMax = "30"))
	float SwarmNetUpdateHz = 10.f;

	/** Hard ceiling on simultaneously simulated swarm mobs (spawn requests beyond are refused). */
	UPROPERTY(Config, EditAnywhere, Category = "Swarm", meta = (ClampMin = "1"))
	int32 SwarmSimHardCap = 600;

	/** Hit >= this fraction of max health raises Event.Ritual.HeavyDamageTaken. */
	UPROPERTY(Config, EditAnywhere, Category = "Combat", meta = (ClampMin = "0", ClampMax = "1"))
	float HeavyDamageFraction = 0.2f;

	static const UEclipseGameSettings* Get() { return GetDefault<UEclipseGameSettings>(); }

	virtual FName GetCategoryName() const override { return TEXT("Game"); }
};
