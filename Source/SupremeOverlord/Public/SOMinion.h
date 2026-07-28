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

private:
	float AttackCooldownRemaining = 0.0f;
	float LifetimeRemaining       = 0.0f;

	UPROPERTY(Transient)
	TObjectPtr<AActor> CurrentTarget;

	AActor* FindBestTarget() const;
};
