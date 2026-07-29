#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SOItemAffix.h"
#include "SOTrap.h"
#include "SOCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USOHealthComponent;
class USOManaComponent;
class USOExperienceComponent;
class USOAttributesComponent;
class USOTalentComponent;
class USODamageType;
class USOWeaponData;
class ASOShadowBoltProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSOOnGoldChanged,     int32, OldGold, int32, NewGold, int32, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams  (FSOOnWeaponChanged,   USOWeaponData*, OldWeapon, USOWeaponData*, NewWeapon);

UCLASS()
class SUPREMEOVERLORD_API ASOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASOCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	/** Bound to HealthComponent->OnDeath — flips the character into a dead state. */
	UFUNCTION()
	void HandleDeath(USOHealthComponent* OwningComponent, AController* InstigatedBy, AActor* DamageCauser);

	/** Bound to ExperienceComponent->OnLevelUp — applies per-level stat bumps. */
	UFUNCTION()
	void HandleLevelUp(USOExperienceComponent* OwningComponent, int32 NewLevel, int32 PreviousLevel);

	/** Bound to HealthComponent->OnHealthChanged — triggers hit-stop/camera shake when Delta < 0 (took damage). */
	UFUNCTION()
	void HandleHealthChanged(USOHealthComponent* OwningComponent, float OldHealth, float NewHealth, float Delta, AController* InstigatedBy, AActor* DamageCauser);

	/** Bound to BestiaryComponent->OnBestiaryEntryUpdated — checks kill-count achievement milestones. */
	UFUNCTION()
	void HandleBestiaryEntryUpdated(TSubclassOf<class ASOEnemyCharacter> EnemyClass, int32 NewCount);

	/** Bound to WaypointComponent->OnWaypointDiscovered — checks exploration achievement milestones. */
	UFUNCTION()
	void HandleWaypointDiscovered(class ASOWaypoint* Waypoint);

public:
	/** Fixed isometric spring arm - not driven by pawn rotation. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera used for the isometric view. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Camera")
	TObjectPtr<UCameraComponent> TopDownCamera;

	/** Health / damage / death tracking. Any actor can read this to query if the character is alive. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Health")
	TObjectPtr<USOHealthComponent> HealthComponent;

	/** Regenerating mana pool consumed by spells (Shadow Bolt, etc.). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Mana")
	TObjectPtr<USOManaComponent> ManaComponent;

	/** XP / level tracking. Enemies grant XP on death. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|XP")
	TObjectPtr<USOExperienceComponent> ExperienceComponent;

	/** Passive attribute pool — Strength/Intellect/Vitality with per-level allocation. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Attributes")
	TObjectPtr<USOAttributesComponent> AttributesComponent;

	/** Talent tree — banked talent points and unlocked nodes. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Talents")
	TObjectPtr<USOTalentComponent> TalentComponent;

	/** Multi-slot equipment — weapons, armor, jewelry. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Equipment")
	TObjectPtr<class USOEquipmentComponent> EquipmentComponent;

	/** Material inventory + crafting entry point. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Inventory")
	TObjectPtr<class USOInventoryComponent> InventoryComponent;

	/** Quest tracking — active quests, objectives, and completion rewards. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Quests")
	TObjectPtr<class USOQuestComponent> QuestComponent;

	/** Active status effects — Burning, Poisoned, Frozen, Slowed, etc. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|StatusEffects")
	TObjectPtr<class USOStatusEffectComponent> StatusEffectComponent;

	/** Minion summoning — spawns and tracks the player's undead army. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Summoning")
	TObjectPtr<class USOSummonComponent> SummonComponent;

	/** Dark Presence Aura — periodically slows and optionally damages nearby enemies. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Aura")
	TObjectPtr<class USOAuraComponent> AuraComponent;

	/** Corruption meter — fills from enemy kills; activates Overlord Mode when full. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Corruption")
	TObjectPtr<class USOCorruptionComponent> CorruptionComponent;

	/** Corpse Explosion — consumes a nearby corpse for a percent-HP AoE detonation. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|CorpseExplosion")
	TObjectPtr<class USOCorpseExplosionComponent> CorpseExplosionComponent;

	/** Shadow Step — short-range teleport toward the cursor with a brief i-frame. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Blink")
	TObjectPtr<class USOBlinkComponent> BlinkComponent;

	/** Dodge Roll — free, cooldown-gated evade with a full-duration i-frame. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|DodgeRoll")
	TObjectPtr<class USODodgeRollComponent> DodgeRollComponent;

	/** Waypoint travel — tracks discovered waypoints and handles fast-travel teleports. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Waypoint")
	TObjectPtr<class USOWaypointComponent> WaypointComponent;

	// -----------------------------------------------------------------------
	// Familiar — a permanent hovering companion, distinct from summoned
	// minions. Auto-spawned once in BeginPlay when FamiliarClass is set.
	// -----------------------------------------------------------------------

	/** Familiar class to spawn on BeginPlay. Leave unset to play without one. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Familiar")
	TSubclassOf<class ASOFamiliarActor> FamiliarClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "SupremeOverlord|Familiar", Transient)
	TObjectPtr<class ASOFamiliarActor> ActiveFamiliar;

	/** Tracks kills per enemy species for the bestiary/codex (L key). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Bestiary")
	TObjectPtr<class USOBestiaryComponent> BestiaryComponent;

	/** Quick-use health/mana potion charges (I key). Refills automatically on waypoint travel. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Consumable")
	TObjectPtr<class USOConsumableComponent> ConsumableComponent;

	/** Consumes one potion charge, healing/restoring mana. See USOConsumableComponent::UsePotion. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Consumable")
	bool UsePotion();

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Consumable")
	float GetPotionCooldownRemaining() const;

	/**
	 * Generic unlock-by-ID milestone tracker with a HUD toast. A handful of
	 * concrete milestones (level/kills/gold/waypoints) are wired up in
	 * BeginPlay as examples — add more by calling
	 * AchievementComponent->UnlockAchievement(...) from anywhere.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Achievements")
	TObjectPtr<class USOAchievementComponent> AchievementComponent;

	/** Opens/closes the bestiary/codex overlay. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Bestiary")
	void ToggleBestiary();

	// -----------------------------------------------------------------------
	// Reality Slash — J key; Overlord-flavored instant-kill on ordinary
	// enemies, a heavy True-damage burst on anything that resists (bosses/elites).
	// -----------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|RealitySlash")
	TObjectPtr<class USORealitySlashComponent> RealitySlashComponent;

	/** Slashes the nearest enemy to TargetLocation. See USORealitySlashComponent::Cast. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|RealitySlash")
	void CastRealitySlash(FVector TargetLocation);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|RealitySlash")
	float GetRealitySlashCooldownRemaining() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> RealitySlashSFX;

	// -----------------------------------------------------------------------
	// Hit-stop / camera shake "juice" — brief time freeze + camera shake on
	// landing or taking a hit, purely cosmetic game feel.
	// -----------------------------------------------------------------------

	/** Camera shake played on impact. Leave unset to disable shake (hit-stop still applies). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Juice")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;

	/** Global time dilation applied for HitStopDuration real seconds. 1.0 disables hit-stop entirely. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Juice", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float HitStopTimeDilation = 0.05f;

	/** Real-world seconds the freeze lasts, regardless of HitStopTimeDilation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Juice", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "0.3"))
	float HitStopDuration = 0.045f;

	/**
	 * Plays the hit camera shake and briefly freezes the world via global time
	 * dilation. ShakeScale multiplies the camera shake's intensity (bigger for
	 * a boss nova, smaller for a jab). Safe to call rapidly — each call resets
	 * the freeze timer rather than stacking.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Juice")
	void TriggerHitImpact(float ShakeScale = 1.0f);

	/** Distance from the character to the camera along the spring arm. Tweak in editor to zoom in/out. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Camera", meta = (ClampMin = "100.0", ClampMax = "5000.0", UIMin = "100.0", UIMax = "3000.0"))
	float CameraDistance = 1200.0f;

	/** Downward pitch of the camera in degrees. 45-60 gives the classic ARPG isometric feel. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Camera", meta = (ClampMin = "20.0", ClampMax = "89.0", UIMin = "30.0", UIMax = "75.0"))
	float CameraPitch = 55.0f;

	/** World yaw the camera locks to. Diablo-style views typically sit at -45 for a slight iso rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Camera")
	float CameraYaw = -45.0f;

	/** Ground movement speed applied to the CharacterMovementComponent. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Movement", meta = (ClampMin = "100.0", UIMin = "100.0", UIMax = "1500.0"))
	float MovementSpeed = 600.0f;

	/** Rotation rate (yaw deg/s) used to orient the character toward its move direction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Movement", meta = (ClampMin = "60.0", UIMin = "60.0", UIMax = "1440.0"))
	float RotationRateYaw = 720.0f;

	/** Applies CameraDistance / CameraPitch / CameraYaw to the spring arm. Also callable from Blueprint after tweaking values. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Camera")
	void ApplyCameraSettings();

	/** Applies MovementSpeed / RotationRateYaw to the movement component. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Movement")
	void ApplyMovementSettings();

	/** Convenience getter — true while HealthComponent reports alive. */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Health")
	bool IsAlive() const;

	/** Cosmetic + gameplay reaction to death: stops movement, disables capsule collision, drops input. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Health")
	void OnCharacterDied(AController* InstigatedBy, AActor* DamageCauser);

	// -----------------------------------------------------------------------
	// Death / Respawn — without this the game had no way to continue after
	// dying; HandleDeath now schedules an automatic respawn at the last
	// discovered waypoint (or in place if none has been found yet).
	// -----------------------------------------------------------------------

	/** Seconds after death before Respawn() fires automatically. 0 with bAutoRespawn disables the auto-timer (call Respawn manually, e.g. from a "Continue" prompt). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Health|Respawn", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "20.0"))
	float RespawnDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Health|Respawn")
	bool bAutoRespawn = true;

	/** Fraction of current gold lost on death (Diablo-style death penalty). 0 disables the penalty. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Health|Respawn", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float RespawnGoldPenaltyFraction = 0.10f;

	/**
	 * Revives to full HP/mana, restores movement/collision, refills potion
	 * charges, applies the gold penalty, and teleports to the last discovered
	 * waypoint (or stays put if none has been found yet). No-op if not dead.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Health|Respawn")
	void Respawn();

	/** Seconds until the scheduled auto-respawn fires. 0 if not dead or no auto-respawn is pending. */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Health|Respawn")
	float GetRespawnTimeRemaining() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Health|Respawn")
	void OnCharacterRespawned();

	// -----------------------------------------------------------------------
	// Primary attack — melee sphere overlap in front of the character.
	// -----------------------------------------------------------------------

	/** Damage dealt per primary swing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|Primary", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float PrimaryAttackDamage = 15.0f;

	/** How far in front of the character the hit sphere is centered (cm). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|Primary", meta = (ClampMin = "50.0", UIMin = "50.0", UIMax = "800.0"))
	float PrimaryAttackRange = 220.0f;

	/** Sphere radius (cm) used for the overlap check. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|Primary", meta = (ClampMin = "20.0", UIMin = "20.0", UIMax = "500.0"))
	float PrimaryAttackRadius = 140.0f;

	/** Seconds between primary attacks. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|Primary", meta = (ClampMin = "0.05", UIMin = "0.05", UIMax = "5.0"))
	float PrimaryAttackCooldown = 0.5f;

	/** DamageType class used by the primary attack. Leave null to fall back to the base USODamageType. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|Primary")
	TSubclassOf<USODamageType> PrimaryAttackDamageType;

	/** Trace channel used to filter the sphere overlap (defaults to Pawn). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|Primary")
	TEnumAsByte<ECollisionChannel> PrimaryAttackChannel = ECC_Pawn;

	/** When true, snaps the character's yaw to face the attack direction on every swing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|Primary")
	bool bFaceAttackDirection = true;

	/** Debug: draws the attack sphere every time a swing is performed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|Debug")
	bool bDrawPrimaryAttackDebug = false;

	/** True while the primary attack is off cooldown and legal to fire. */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Combat|Primary")
	bool CanPrimaryAttack() const;

	/**
	 * Executes a primary swing aimed at TargetLocation.
	 * Applies PrimaryAttackDamage to every ACharacter with a live USOHealthComponent
	 * inside the sphere. Fires OnPrimaryAttackPerformed on success.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Combat|Primary")
	void PerformPrimaryAttack(FVector TargetLocation);

	/**
	 * BP hook for animation, VFX, and SFX. Called every time a swing goes off
	 * (before the overlap is evaluated). ImpactActors lists everything the
	 * overlap touched — plug into it to spawn per-hit VFX/SFX.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Combat|Primary")
	void OnPrimaryAttackPerformed(const FVector& AttackCenter, const TArray<AActor*>& ImpactActors);

	// -----------------------------------------------------------------------
	// Shadow Bolt — ranged spell projectile.
	// -----------------------------------------------------------------------

	/** Projectile class fired by CastShadowBolt. Assign a BP subclass of ASOShadowBoltProjectile in the editor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|ShadowBolt")
	TSubclassOf<ASOShadowBoltProjectile> ShadowBoltClass;

	/** Seconds between casts. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|ShadowBolt", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "10.0"))
	float ShadowBoltCooldown = 1.5f;

	/** Mana cost per cast. Set to 0 to make the spell free. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|ShadowBolt", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "500.0"))
	float ShadowBoltManaCost = 25.0f;

	/** Muzzle offset forward from the character root (cm). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|ShadowBolt", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "500.0"))
	float ShadowBoltMuzzleForward = 60.0f;

	/** Muzzle offset vertically from the character root (cm). Positive = higher. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|ShadowBolt", meta = (ClampMin = "-200.0", UIMin = "-200.0", UIMax = "200.0"))
	float ShadowBoltMuzzleHeight = 40.0f;

	/** When true, snaps the character's yaw to face the cast direction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|ShadowBolt")
	bool bFaceCastDirection = true;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Combat|ShadowBolt")
	bool CanCastShadowBolt() const;

	/**
	 * Fires a ShadowBoltClass projectile from the muzzle offset toward TargetLocation.
	 * No-op if the ability is on cooldown, the character is dead, or ShadowBoltClass is unset.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Combat|ShadowBolt")
	void CastShadowBolt(FVector TargetLocation);

	/** BP hook for cast VFX/SFX. Called the moment the bolt is spawned. */
	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Combat|ShadowBolt")
	void OnShadowBoltCast(const FVector& MuzzleLocation, const FVector& AimDirection, ASOShadowBoltProjectile* SpawnedBolt);

	// -----------------------------------------------------------------------
	// Currency.
	// -----------------------------------------------------------------------

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Currency")
	int32 GetGold() const { return Gold; }

	/** Adds gold (or subtracts if negative). Fires OnGoldChanged when the value moves. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Currency")
	void AddGold(int32 Amount);

	/** Broadcast whenever Gold changes. Params: OldGold, NewGold, Delta. */
	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Currency")
	FSOOnGoldChanged OnGoldChanged;

	/** Starting gold. Applied on BeginPlay. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Currency", meta = (ClampMin = "0", UIMin = "0"))
	int32 StartingGold = 0;

	// -----------------------------------------------------------------------
	// Equipment — single main-hand weapon slot for now.
	// -----------------------------------------------------------------------

	/** Weapon equipped at spawn. Assign a USOWeaponData asset (or a BP subclass). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Equipment")
	TObjectPtr<USOWeaponData> StartingWeapon;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "SupremeOverlord|Equipment", Transient)
	TObjectPtr<USOWeaponData> EquippedWeapon;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Equipment")
	FSOOnWeaponChanged OnWeaponChanged;

	/** Equips NewWeapon (may be null to clear the slot). Broadcasts OnWeaponChanged. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Equipment")
	void EquipWeapon(USOWeaponData* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Equipment")
	void UnequipWeapon();

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Equipment")
	USOWeaponData* GetEquippedWeapon() const { return EquippedWeapon; }

	/** Base primary damage + any equipped-weapon bonus. */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Combat|Primary")
	float GetEffectivePrimaryAttackDamage() const;

	/** Base primary cooldown * equipped-weapon multiplier. */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Combat|Primary")
	float GetEffectivePrimaryAttackCooldown() const;

	/** Base ShadowBolt damage + any equipped-weapon bonus, forwarded to the spawned projectile. */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Combat|ShadowBolt")
	float GetEffectiveShadowBoltDamage() const;

	/** Configured on the character; the projectile's Damage picks this up when it spawns. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|ShadowBolt", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float ShadowBoltBaseDamage = 40.0f;

	// -----------------------------------------------------------------------
	// Per-level stat bumps applied inside HandleLevelUp.
	// -----------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Leveling", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "200.0"))
	float MaxHealthPerLevel = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Leveling", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "200.0"))
	float MaxManaPerLevel = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Leveling", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "50.0"))
	float PrimaryDamagePerLevel = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Leveling", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "50.0"))
	float ShadowBoltDamagePerLevel = 3.0f;

	/** BP hook so subclasses / designers can react to level-ups with VFX/SFX. */
	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Leveling")
	void OnLevelUpReached(int32 NewLevel);

	// -----------------------------------------------------------------------
	// SFX slots. Assigning a USoundBase in a BP subclass makes the event audible.
	// -----------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> PrimaryAttackSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> ShadowBoltCastSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> LifeDrainCastSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> LevelUpSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> DeathSFX;

	// -----------------------------------------------------------------------
	// Life Drain — AoE damage that heals the caster for a portion of damage dealt.
	// -----------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|LifeDrain", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float LifeDrainDamage = 20.0f;

	/** Sphere radius (cm) centered on the character. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|LifeDrain", meta = (ClampMin = "50.0", UIMin = "50.0", UIMax = "1500.0"))
	float LifeDrainRadius = 420.0f;

	/** Cooldown (s) between casts. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|LifeDrain", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "60.0"))
	float LifeDrainCooldown = 6.0f;

	/** Mana cost per cast. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|LifeDrain", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "500.0"))
	float LifeDrainManaCost = 40.0f;

	/** Fraction of TOTAL damage dealt that heals the caster. 0.5 = half of the sum. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|LifeDrain", meta = (ClampMin = "0.0", ClampMax = "5.0", UIMin = "0.0", UIMax = "2.0"))
	float LifeDrainHealFraction = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|LifeDrain")
	TSubclassOf<USODamageType> LifeDrainDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|LifeDrain")
	TEnumAsByte<ECollisionChannel> LifeDrainChannel = ECC_Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Combat|Debug")
	bool bDrawLifeDrainDebug = false;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Combat|LifeDrain")
	bool CanCastLifeDrain() const;

	// -----------------------------------------------------------------------
	// Save / Load.
	// -----------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Save")
	FString DefaultSaveSlot = TEXT("SupremeOverlordSlot");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Save")
	int32 DefaultSaveUserIndex = 0;

	/** Saves the current player state to Slot (falls back to DefaultSaveSlot). Returns true on success. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Save")
	bool SaveGameToSlotName(const FString& Slot);

	/** Loads Slot and applies it to the character. Returns true on success. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Save")
	bool LoadGameFromSlotName(const FString& Slot);

	/** Convenience wrapper — saves to DefaultSaveSlot. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Save")
	bool QuickSave();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Save")
	bool QuickLoad();

	/** BP hook for save/load VFX/SFX/UI toast. bSaved = true on save, false on load. */
	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Save")
	void OnSaveGameCompleted(bool bSaved, bool bSuccess, const FString& Slot);

	// -----------------------------------------------------------------------
	// Minion Summoning.
	// -----------------------------------------------------------------------

	/**
	 * Attempts to summon a minion near TargetLocation.
	 * Delegates to SummonComponent — see USOSummonComponent::SummonMinion for guards.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Summoning")
	void CastSummonMinion(FVector TargetLocation);

	/** Destroys all active minions. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Summoning")
	void DismissAllMinions();

	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Summoning")
	void OnMinionSummoned(class ASOMinion* Minion);

	// -----------------------------------------------------------------------
	// Trap System — place hazard traps at the cursor (C key), cycle type (V).
	// -----------------------------------------------------------------------

	/** Trap actor class to spawn. Assign a BP_Trap subclass in the editor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Traps")
	TSubclassOf<ASOTrap> TrapClass;

	/** Maximum number of traps allowed active simultaneously. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Traps", meta = (ClampMin = "1", UIMin = "1", UIMax = "20"))
	int32 MaxActiveTrapCount = 10;

	/** Currently selected trap variant placed by PlaceTrap. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Traps")
	ESOTrapType SelectedTrapType = ESOTrapType::ShadowSnare;

	/** Spawns the selected trap type at TargetLocation. Respects MaxActiveTrapCount and cooldown. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Traps")
	void PlaceTrap(FVector TargetLocation);

	/** Advances SelectedTrapType to the next variant in the cycle. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Traps")
	void CycleTrap();

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Traps")
	float GetTrapPlaceCooldownRemaining() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Traps")
	float GetTrapPlaceCooldown() const { return TrapPlaceCooldown; }

	// -----------------------------------------------------------------------
	// Corruption / Overlord Mode — Z key, requires full corruption meter.
	// -----------------------------------------------------------------------

	/** Activates Overlord Mode via CorruptionComponent if corruption is full. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Corruption")
	void ActivateOverlordMode();

	// -----------------------------------------------------------------------
	// Necromantic Resurrection — U key; raises a nearby corpse as a temp minion.
	// -----------------------------------------------------------------------

	/** Finds the nearest dead enemy within range of TargetLocation and spawns it as a limited-life minion. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Summoning")
	void CastNecroticResurrect(FVector TargetLocation);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Summoning")
	float GetNecromancyCooldownRemaining() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Summoning")
	float GetNecromancyCooldown() const;

	// Cooldown-remaining accessors used by the HUD skill bar.
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Combat|Cooldowns")
	float GetPrimaryAttackCooldownRemaining() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Combat|Cooldowns")
	float GetShadowBoltCooldownRemaining() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Combat|Cooldowns")
	float GetLifeDrainCooldownRemaining() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Combat|Cooldowns")
	float GetSummonCooldownRemaining() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Summoning")
	float GetSummonCooldown() const;

	/** Hint to the SFX slot for the summon cast. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> SummonCastSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> TrapPlaceSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> OverlordModeActivateSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> NecroResurrectSFX;

	/** AoE around the caster: damages all live enemies, then heals the caster for a fraction of total damage dealt. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Combat|LifeDrain")
	void CastLifeDrain();

	/** BP hook: LifeDrain cast, VFX/SFX, HealedAmount is what actually landed on the caster. */
	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Combat|LifeDrain")
	void OnLifeDrainCast(const TArray<AActor*>& ImpactActors, float TotalDamageDealt, float HealedAmount);

	// -----------------------------------------------------------------------
	// Corpse Explosion — X key; consumes the nearest corpse for a percent-HP AoE.
	// -----------------------------------------------------------------------

	/** Detonates the nearest corpse to TargetLocation via CorpseExplosionComponent. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|CorpseExplosion")
	void CastCorpseExplosion(FVector TargetLocation);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|CorpseExplosion")
	float GetCorpseExplosionCooldownRemaining() const;

	// -----------------------------------------------------------------------
	// Shadow Step / Blink — B key; short teleport with a brief i-frame.
	// -----------------------------------------------------------------------

	/** Teleports the character toward TargetLocation via BlinkComponent. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Blink")
	void CastBlink(FVector TargetLocation);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Blink")
	float GetBlinkCooldownRemaining() const;

	// -----------------------------------------------------------------------
	// Cursed Ground — H key; persistent AoE hazard zone at the cursor.
	// -----------------------------------------------------------------------

	/** Hazard zone actor class to spawn. Assign a BP_CursedGround subclass in the editor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|CursedGround")
	TSubclassOf<class ASOCursedGround> CursedGroundClass;

	/** Mana cost per placement. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|CursedGround", meta = (ClampMin = "0.0"))
	float CursedGroundManaCost = 30.0f;

	/** Maximum number of Cursed Ground zones allowed active at once. Oldest is removed when exceeded. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|CursedGround", meta = (ClampMin = "1", UIMin = "1", UIMax = "10"))
	int32 MaxCursedGrounds = 5;

	/** Spawns a CursedGroundClass zone at TargetLocation. Respects MaxCursedGrounds and cooldown. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|CursedGround")
	void PlaceCursedGround(FVector TargetLocation);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|CursedGround")
	float GetCursedGroundCooldownRemaining() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|CursedGround")
	float GetCursedGroundCooldown() const { return CursedGroundCooldown; }

	// -----------------------------------------------------------------------
	// Dodge Roll — Space key; free, cooldown-gated evade with a full i-frame.
	// -----------------------------------------------------------------------

	/** Rolls away using current movement direction, or toward TargetLocation when standing still. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|DodgeRoll")
	void CastDodgeRoll(FVector TargetLocation);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|DodgeRoll")
	float GetDodgeRollCooldownRemaining() const;

	// -----------------------------------------------------------------------
	// Legendary Uniques — fixed special effects carried by hand-authored items.
	// -----------------------------------------------------------------------

	/** True while any currently-equipped item carries the given LegendaryEffect. */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Legendary")
	bool HasLegendaryEffect(ESOLegendaryEffect Effect) const;

	/** Fraction of total Primary Attack damage healed back when VampiricStrikes is equipped. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Legendary", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float VampiricStrikesHealFraction = 0.25f;

	// -----------------------------------------------------------------------
	// Waypoint Fast Travel — M toggles the map, number keys travel.
	// -----------------------------------------------------------------------

	/** Opens/closes the waypoint map overlay. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Waypoint")
	void ToggleWaypointMap();

	/** Teleports to the Index'th discovered waypoint (0-based). Returns false if the index is out of range. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Waypoint")
	bool TravelToWaypoint(int32 Index);

	// -----------------------------------------------------------------------
	// Talent Respec — P key; refunds every unlocked talent node for a gold fee.
	// -----------------------------------------------------------------------

	/** Gold cost to respec. Set to 0 to make it free. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Talents", meta = (ClampMin = "0"))
	int32 RespecGoldCost = 50;

	/**
	 * Reverts every unlocked talent node and refunds their points, charging
	 * RespecGoldCost. Returns false if nothing is unlocked or gold is short.
	 */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Talents")
	bool RespecTalents();

	// -----------------------------------------------------------------------
	// Vendor transaction toast — set by ASOVendorNPC on purchase/sellback so
	// the HUD can flash a brief "Bought/Sold X for Yg" confirmation.
	// -----------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Vendor")
	void ShowTransactionToast(const FString& Message, float Duration = 2.5f);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Vendor")
	bool IsTransactionToastActive() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Vendor")
	const FString& GetTransactionToastText() const { return TransactionToastText; }

	// -----------------------------------------------------------------------
	// Legendary/Set drop toast — set by ASOItemPickup when a rare drop is
	// picked up, for the Diablo-style "LEGENDARY DROPPED: X" HUD banner.
	// -----------------------------------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Loot")
	void ShowLegendaryDropToast(const FString& Message, float Duration = 4.0f);

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Loot")
	bool IsLegendaryDropToastActive() const;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Loot")
	const FString& GetLegendaryDropToastText() const { return LegendaryDropToastText; }

	// -----------------------------------------------------------------------
	// SFX slots for the new abilities.
	// -----------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> BlinkSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> CorpseExplosionSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> CursedGroundSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> DodgeRollSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Audio")
	TObjectPtr<class USoundBase> RespecSFX;

private:
	FString TransactionToastText;
	float   TransactionToastExpireTime = -1.0f;

	FString LegendaryDropToastText;
	float   LegendaryDropToastExpireTime = -1.0f;

	FTimerHandle PrimaryAttackCooldownHandle;
	bool bPrimaryAttackOnCooldown = false;

	FTimerHandle ShadowBoltCooldownHandle;
	bool bShadowBoltOnCooldown = false;

	FTimerHandle LifeDrainCooldownHandle;
	bool bLifeDrainOnCooldown = false;

	FTimerHandle TrapPlaceCooldownHandle;
	bool  bTrapPlaceOnCooldown = false;
	float TrapPlaceCooldown    = 0.5f;

	TArray<TWeakObjectPtr<ASOTrap>> ActiveTraps;

	FTimerHandle CursedGroundCooldownHandle;
	bool  bCursedGroundOnCooldown = false;
	float CursedGroundCooldown    = 1.0f;

	TArray<TWeakObjectPtr<class ASOCursedGround>> ActiveCursedGrounds;

	FTimerHandle HitStopTimerHandle;

	FTimerHandle RespawnTimerHandle;

	UPROPERTY(VisibleInstanceOnly, Category = "SupremeOverlord|Currency", Transient)
	int32 Gold = 0;
};
