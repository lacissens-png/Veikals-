#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SOSaveGame.generated.h"

/**
 * Serialized snapshot of the player's persistent state. Kept intentionally
 * flat and value-only so it round-trips cleanly through SaveGameToSlot /
 * LoadGameFromSlot even after class layouts change.
 *
 * Weapons round-trip by soft path — the actual data asset is resolved on
 * load via LoadObject<USOWeaponData>. Level state (spawner progress,
 * dropped orbs, corpses) is deliberately *not* persisted — that's a Diablo
 * "walk-out-of-town" style save, not a full world snapshot.
 */
UCLASS()
class SUPREMEOVERLORD_API USOSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	FString SlotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	int32 UserIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	FDateTime SavedAt;

	// -- Character stats ----------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Stats")
	int32 CharacterLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Stats")
	int32 XPInCurrentLevel = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Stats")
	int32 Gold = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Stats")
	int32 Strength  = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Stats")
	int32 Intellect = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Stats")
	int32 Vitality  = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Stats")
	int32 UnspentAttributePoints = 0;

	// -- Pool sizes ----------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Pools")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Pools")
	float CurrentHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Pools")
	float MaxMana = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Pools")
	float CurrentMana = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Pools")
	float PrimaryAttackDamage = 15.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Pools")
	float ShadowBoltBaseDamage = 40.0f;

	// -- Equipment ----------------------------------------------------------

	/** Path of the equipped weapon's USOWeaponData asset, empty if unarmed. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Equipment")
	FString EquippedWeaponPath;
};
