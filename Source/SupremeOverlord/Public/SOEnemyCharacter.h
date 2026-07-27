#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SOEnemyCharacter.generated.h"

class USOHealthComponent;
class USODamageType;
class USOItemData;
class ASOPickupOrb;
class ASOItemPickup;

/** One row of an enemy's loot table. */
USTRUCT(BlueprintType)
struct FSOLootDrop
{
	GENERATED_BODY()

	/** Orb class to spawn on a successful roll. Typically a BP subclass of ASOPickupOrb. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot")
	TSubclassOf<ASOPickupOrb> OrbClass;

	/** Probability [0..1] that this row drops when the enemy dies. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float DropChance = 1.0f;

	/** Minimum number of orbs spawned if the roll succeeds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "1", UIMin = "1"))
	int32 MinCount = 1;

	/** Maximum number of orbs spawned if the roll succeeds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "1", UIMin = "1"))
	int32 MaxCount = 1;
};

/** One row of the enemy's item-drop pool. Weights drive the weighted-random pick. */
USTRUCT(BlueprintType)
struct FSOItemDrop
{
	GENERATED_BODY()

	/** Item to drop. Usually a USOWeaponData asset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDrop")
	TObjectPtr<USOItemData> Item;

	/** Relative weight in the pool. 1 = baseline, 3 = 3x more likely than a weight-1 sibling. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDrop", meta = (ClampMin = "0.01", UIMin = "0.01", UIMax = "10.0"))
	float Weight = 1.0f;
};

UCLASS()
class SUPREMEOVERLORD_API ASOEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASOEnemyCharacter();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleDeath(USOHealthComponent* OwningComponent, AController* InstigatedBy, AActor* DamageCauser);

public:
	/** Health/damage/death — same component the player uses. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Health")
	TObjectPtr<USOHealthComponent> HealthComponent;

	/** Max walk speed the AI moves at. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Movement", meta = (ClampMin = "50.0", UIMin = "50.0", UIMax = "1500.0"))
	float MovementSpeed = 400.0f;

	/** Radius (cm) within which this enemy notices the player and begins chasing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|AI", meta = (ClampMin = "100.0", UIMin = "100.0", UIMax = "5000.0"))
	float SightRadius = 1500.0f;

	/** Once aggro'd, the enemy will keep chasing until the player leaves this larger radius. Prevents jitter. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|AI", meta = (ClampMin = "100.0", UIMin = "100.0", UIMax = "8000.0"))
	float LoseSightRadius = 2200.0f;

	/** Distance (cm) at which this enemy stops moving and starts hitting. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Combat", meta = (ClampMin = "50.0", UIMin = "50.0", UIMax = "1000.0"))
	float AttackRange = 180.0f;

	/** Damage dealt per melee swing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Combat", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float AttackDamage = 12.0f;

	/** Seconds between attacks. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Combat", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "10.0"))
	float AttackCooldown = 1.4f;

	/** Damage type class used when hitting the player. Assign a BP_Damage_* subclass in the editor to key resistances/VFX. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Combat")
	TSubclassOf<USODamageType> AttackDamageType;

	/** After death the actor stays this long before being destroyed. Set to 0 to persist forever (until GC). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Death", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "60.0"))
	float CorpseLifetime = 5.0f;

	/** XP granted to the killer's ASOCharacter on death. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Death", meta = (ClampMin = "0", UIMin = "0", UIMax = "10000"))
	int32 XPReward = 25;

	/** Orb loot rolled and spawned on death. Each row is an independent chance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Loot")
	TArray<FSOLootDrop> LootTable;

	/** Actor class used when this enemy drops an item. Typically a BP subclass of ASOItemPickup. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Loot|Items")
	TSubclassOf<ASOItemPickup> ItemPickupClass;

	/** Overall chance any item drops when this enemy dies (0..1). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Loot|Items", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float ItemDropChance = 0.15f;

	/** Pool of items this enemy can roll for. Picked weighted-random when ItemDropChance triggers. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Loot|Items")
	TArray<FSOItemDrop> ItemDropPool;

	/** Horizontal spread (cm) each spawned orb is offset by from the corpse. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Loot", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "500.0"))
	float LootSpreadRadius = 80.0f;

	/** Vertical offset (cm) applied to spawned orbs so they float above the ground. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Loot", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "300.0"))
	float LootSpawnHeight = 50.0f;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Enemy")
	bool IsAlive() const;

	/**
	 * Called by the AI controller when the enemy should swing.
	 * Default: applies AttackDamage to Target using AttackDamageType.
	 * Returns true if the attack "landed" (damage applied or projectile spawned).
	 * Subclasses (e.g. ranged casters) override to spawn a projectile instead.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Enemy|Combat")
	virtual bool PerformAttack(AActor* Target);

	/** Rolls LootTable and spawns any resulting orbs around the corpse. Public so BP subclasses can chain into it. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Enemy|Loot")
	void DropLoot();

	/** Rolls the item pool once and spawns an ItemPickupClass carrying the picked item if the roll succeeds. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Enemy|Loot|Items")
	void RollItemDrop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Audio")
	TObjectPtr<class USoundBase> AttackSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Enemy|Audio")
	TObjectPtr<class USoundBase> DeathSFX;
};
