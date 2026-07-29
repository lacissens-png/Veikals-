#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SOItemAffix.h"
#include "SOItemData.h"
#include "SODifficultySubsystem.h"
#include "SOSaveGame.generated.h"

class USOItemData;
class USOGemData;
class USOMaterialData;
class USOTalentNode;
class USOQuestData;
class ASOEnemyCharacter;
class USOVassalData;

/**
 * One equipped/inventoried item instance, serialized compactly enough to
 * round-trip through USaveGame: the source template asset (soft path) plus
 * whatever USOLootRoller rolled onto it, replayed via
 * USOLootRoller::ReconstructItemInstance on load rather than re-randomized.
 */
USTRUCT(BlueprintType)
struct FSOSavedItemInstance
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	TSoftObjectPtr<USOItemData> SourceTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	ESOItemRarity Rarity = ESOItemRarity::Common;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	int32 ItemLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	TArray<FSOItemAffix> RolledAffixes;

	/** Gems socketed into this item, in socket order — re-applied via SocketGem after the base item is re-equipped. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	TArray<TSoftObjectPtr<USOGemData>> SocketedGemPaths;
};

/** One active quest's saved objective progress. Resolved back to a live USOQuestData* on load. */
USTRUCT(BlueprintType)
struct FSOSavedQuestProgress
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	TSoftObjectPtr<USOQuestData> Quest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	TArray<int32> Progress;
};

/** One material stack. Kept as an array entry rather than a TMap keyed by soft pointer, which UHT doesn't support. */
USTRUCT(BlueprintType)
struct FSOSavedMaterialCount
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	TSoftObjectPtr<USOMaterialData> Material;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save")
	int32 Count = 0;
};

/**
 * Serialized snapshot of the player's persistent state. Kept intentionally
 * flat and value-only so it round-trips cleanly through SaveGameToSlot /
 * LoadGameFromSlot even after class layouts change.
 *
 * Assets (items, gems, talent nodes, quests) round-trip by soft path,
 * resolved on load via LoadSynchronous(). Level state (spawner progress,
 * dropped orbs, corpses, other actors' positions) is deliberately *not*
 * persisted — that's a Diablo "walk-out-of-town" style save, not a full
 * world snapshot. Waypoints are the one placed-actor exception: they're
 * matched back up by ASOWaypoint::GetStableID() rather than by position.
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

	/**
	 * Final, bonus-inclusive walk speed (base + attributes + talents +
	 * equipment). Restored directly rather than replayed from talent/equipment
	 * effects, which would double-count whatever's already baked into this.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Pools")
	float MovementSpeed = 600.0f;

	/** Final, bonus-inclusive Life Drain heal fraction (base + FlatLifeDrainHealFrac talent nodes). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Pools")
	float LifeDrainHealFraction = 0.5f;

	/** Final, bonus-inclusive base crit chance (base + FlatCritChance talent nodes; excludes the equipped weapon's own bonus). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Pools")
	float CritChance = 0.05f;

	/** Final, bonus-inclusive mana regen per second (base + FlatManaRegen talent nodes). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Pools")
	float ManaRegenPerSecond = 10.0f;

	// -- Equipment ------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Equipment")
	TMap<ESOEquipSlot, FSOSavedItemInstance> EquippedItems;

	// -- Inventory / Crafting ---------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Inventory")
	TArray<FSOSavedMaterialCount> MaterialCounts;

	// -- Talents ----------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Talents")
	TArray<TSoftObjectPtr<USOTalentNode>> UnlockedTalentNodes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Talents")
	int32 AvailableTalentPoints = 0;

	// -- Quests -------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Quests")
	TArray<FSOSavedQuestProgress> ActiveQuestProgress;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Quests")
	TArray<TSoftObjectPtr<USOQuestData>> CompletedQuests;

	// -- Bestiary -------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Bestiary")
	TMap<TSubclassOf<ASOEnemyCharacter>, int32> BestiaryKillCounts;

	// -- Achievements ----------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Achievements")
	TArray<FName> UnlockedAchievements;

	// -- Consumables --------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Consumable")
	int32 PotionCharges = 0;

	// -- Waypoints -----------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Waypoint")
	TArray<FName> DiscoveredWaypointIDs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Waypoint")
	FName LastWaypointID;

	// -- Difficulty / Corruption -----------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Difficulty")
	ESODifficultyTier DifficultyTier = ESODifficultyTier::Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Corruption")
	float CorruptionAmount = 0.0f;

	// -- Vassals ----------------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Save|Vassal")
	TArray<TSoftObjectPtr<USOVassalData>> RecruitedVassals;
};
