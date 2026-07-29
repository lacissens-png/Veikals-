#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SOVassalActor.generated.h"

class USOHealthComponent;
class USOVassalComponent;
class USOVassalData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnVassalDied, class ASOVassalActor*, Vassal);

/**
 * Spawned actor for a summoned named vassal (see USOVassalData / USOVassalComponent).
 * Chases/attacks the nearest ASOEnemyCharacter within AggroRange like ASOMinion,
 * and walks back to the summoner when idle and too far away — but represents one
 * unique, more powerful companion rather than a disposable army member.
 */
UCLASS(Blueprintable)
class SUPREMEOVERLORD_API ASOVassalActor : public ACharacter
{
	GENERATED_BODY()

public:
	ASOVassalActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleDeath(USOHealthComponent* OwningComponent, AController* InstigatedBy, AActor* DamageCauser);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vassal")
	TObjectPtr<USOHealthComponent> HealthComponent;

	/** Source data this instance was summoned from. Set by InitializeFromData. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Vassal", Transient)
	TObjectPtr<USOVassalData> SourceData;

	/** Set by USOVassalComponent on spawn so death events propagate back. */
	UPROPERTY(Transient)
	TWeakObjectPtr<USOVassalComponent> OwnerVassalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vassal|Combat", meta = (ClampMin = "0.0"))
	float AttackDamage = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vassal|Combat", meta = (ClampMin = "50.0"))
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vassal|Combat", meta = (ClampMin = "0.1"))
	float AttackCooldown = 1.0f;

	/** Distance (cm) within which the vassal will acquire a target. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vassal|AI", meta = (ClampMin = "100.0"))
	float AggroRange = 2200.0f;

	/** With no target to fight, walks back to the summoner once farther than this away. 0 disables follow-when-idle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vassal|AI", meta = (ClampMin = "0.0"))
	float FollowOwnerDistance = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vassal|Audio")
	TObjectPtr<class USoundBase> AttackSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vassal|Audio")
	TObjectPtr<class USoundBase> DeathSFX;

	UFUNCTION(BlueprintPure, Category = "Vassal")
	bool IsAlive() const;

	/** Applies Data's base stats (health/damage/range/cooldown) to this instance. Called by USOVassalComponent right after spawn. */
	UFUNCTION(BlueprintCallable, Category = "Vassal")
	void InitializeFromData(USOVassalData* Data);

	UPROPERTY(BlueprintAssignable, Category = "Vassal")
	FSOOnVassalDied OnVassalDied;

private:
	float AttackCooldownRemaining = 0.0f;

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> CurrentTarget;

	AActor* FindBestTarget() const;

	/** Walks back to the summoning character once idle and too far away. No-op if already close or unsummoned. */
	void FollowOwner();
};
