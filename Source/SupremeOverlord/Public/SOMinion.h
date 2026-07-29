#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SOMinion.generated.h"

class USOHealthComponent;
class USOStatusEffectComponent;
class USOSummonComponent;

UENUM(BlueprintType)
enum class ESOMinMinionType : uint8
{
	Skeleton     UMETA(DisplayName = "Skeleton"),
	Zombie       UMETA(DisplayName = "Zombie"),
	ShadowWraith UMETA(DisplayName = "ShadowWraith"),
};

/** Power rank a minion climbs by landing killing blows. */
UENUM(BlueprintType)
enum class ESOMinionTier : uint8
{
	Base     UMETA(DisplayName = "Base"),
	Elite    UMETA(DisplayName = "Elite"),
	Champion UMETA(DisplayName = "Champion"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnMinionEvolved, class ASOMinion*, Minion, ESOMinionTier, NewTier);

/**
 * AI-driven minion spawned by the player's SummonComponent.
 * Ticks at 10 Hz to find the nearest live ASOEnemyCharacter within AggroRange
 * and chases/attacks it using the nav system.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOMinion : public ACharacter
{
	GENERATED_BODY()

public:
	ASOMinion();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleDeath(USOHealthComponent* OwningComponent, AController* InstigatedBy, AActor* DamageCauser);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minion|Health")
	TObjectPtr<USOHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minion|StatusEffects")
	TObjectPtr<USOStatusEffectComponent> StatusEffectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion")
	ESOMinMinionType MinionType = ESOMinMinionType::Skeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Combat", meta = (ClampMin = "0.0"))
	float AttackDamage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Combat", meta = (ClampMin = "50.0"))
	float AttackRange = 130.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Combat", meta = (ClampMin = "0.1"))
	float AttackCooldown = 1.2f;

	/** Distance (cm) within which the minion will acquire a target. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|AI", meta = (ClampMin = "100.0"))
	float AggroRange = 2000.0f;

	/**
	 * With no target to fight, the minion follows the summoning character once
	 * farther than this away, so it doesn't get left behind while the player
	 * explores. 0 disables follow-when-idle.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|AI", meta = (ClampMin = "0.0"))
	float FollowOwnerDistance = 400.0f;

	/** How long the minion lives before auto-dissipating (0 = permanent until killed). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Lifetime", meta = (ClampMin = "0.0"))
	float LifetimeDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Audio")
	TObjectPtr<class USoundBase> AttackSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Audio")
	TObjectPtr<class USoundBase> DeathSFX;

	UFUNCTION(BlueprintPure, Category = "Minion")
	bool IsAlive() const;

	/** Set by USOSummonComponent on spawn so death events propagate back. */
	UPROPERTY(Transient)
	TWeakObjectPtr<USOSummonComponent> OwnerSummonComponent;

	// -----------------------------------------------------------------------
	// Tiers / Evolution — minions grow stronger by landing killing blows.
	// -----------------------------------------------------------------------

	/** Current power rank. Starts at Base and climbs every KillsToEvolve kills. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Minion|Tier", Transient)
	ESOMinionTier CurrentTier = ESOMinionTier::Base;

	/** Killing blows required to advance one tier. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Tier", meta = (ClampMin = "1", UIMin = "1", UIMax = "50"))
	int32 KillsToEvolve = 5;

	/** Health / damage multiplier applied on every tier-up. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Tier", meta = (ClampMin = "1.0", UIMin = "1.0", UIMax = "3.0"))
	float TierStatMultiplier = 1.35f;

	/** Movement-speed multiplier applied on every tier-up. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Tier", meta = (ClampMin = "1.0", UIMin = "1.0", UIMax = "2.0"))
	float TierSpeedMultiplier = 1.10f;

	/** Actor scale multiplier applied on every tier-up, so ranks read at a glance. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Tier", meta = (ClampMin = "1.0", UIMin = "1.0", UIMax = "2.0"))
	float TierScaleMultiplier = 1.15f;

	/** Kills this minion has landed since it was summoned. */
	UFUNCTION(BlueprintPure, Category = "Minion|Tier")
	int32 GetKillCount() const { return KillCount; }

	/** Kills still needed for the next tier. Returns 0 once Champion is reached. */
	UFUNCTION(BlueprintPure, Category = "Minion|Tier")
	int32 GetKillsUntilEvolve() const;

	/**
	 * Records a killing blow. Evolves the minion once the tier threshold is met.
	 * Called automatically when a melee swing drops its target.
	 */
	UFUNCTION(BlueprintCallable, Category = "Minion|Tier")
	void NotifyKill();

	UPROPERTY(BlueprintAssignable, Category = "Minion|Tier")
	FSOOnMinionEvolved OnMinionEvolved;

	UFUNCTION(BlueprintImplementableEvent, Category = "Minion|Tier")
	void OnMinionEvolvedBP(ESOMinionTier NewTier);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minion|Audio")
	TObjectPtr<class USoundBase> EvolveSFX;

private:
	float AttackCooldownRemaining = 0.0f;
	float LifetimeRemaining       = 0.0f;

	int32 KillCount = 0;

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> CurrentTarget;

	AActor* FindBestTarget() const;

	/** Walks back to the summoning character once idle and too far away. No-op if already close or unsummoned. */
	void FollowOwner();

	/** Advances CurrentTier one step and applies the stat/scale bumps. */
	void Evolve();
};
