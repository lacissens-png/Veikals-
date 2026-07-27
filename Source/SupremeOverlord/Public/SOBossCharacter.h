#pragma once

#include "CoreMinimal.h"
#include "SOEnemyCharacter.h"
#include "SOBossCharacter.generated.h"

class USODamageType;

UENUM(BlueprintType)
enum class ESOBossPhase : uint8
{
	Phase1 UMETA(DisplayName = "Phase 1 (100..66% HP)"),
	Phase2 UMETA(DisplayName = "Phase 2 (66..33% HP)"),
	Phase3 UMETA(DisplayName = "Phase 3 (33..0% HP)")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnBossPhaseChanged,
	ESOBossPhase, OldPhase,
	ESOBossPhase, NewPhase);

/**
 * Boss unit. Overrides PerformAttack to alternate between:
 *   - Melee slam (as-is behavior of the base enemy)
 *   - Telegraphed AoE (visualized with a red decal circle for
 *     TelegraphWindup seconds, then damage everyone still inside).
 *
 * Attack pattern intensifies per-phase — phase transitions fire on HP
 * thresholds (66% and 33%) and scale AttackDamage / MovementSpeed /
 * TelegraphedAttackRadius.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOBossCharacter : public ASOEnemyCharacter
{
	GENERATED_BODY()

public:
	ASOBossCharacter();

protected:
	virtual void BeginPlay() override;
	virtual bool PerformAttack(AActor* Target) override;

	UFUNCTION()
	void HandleHealthChanged(USOHealthComponent* OwningComponent, float OldHealth, float NewHealth, float Delta,
	                         AController* InstigatedBy, AActor* DamageCauser);

	void EnterPhase(ESOBossPhase NewPhase);
	void PerformTelegraphedAoE(AActor* Target);
	void TelegraphedAoEResolve(TWeakObjectPtr<AActor> Target, FVector Center);

public:
	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Boss")
	FSOOnBossPhaseChanged OnBossPhaseChanged;

	/** Boss display name shown by the HUD when in range. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Identity")
	FText BossDisplayName = FText::FromString(TEXT("Nameless Overlord"));

	/** How many boss attacks between two telegraphed AoEs (2 = every third swing is AoE). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Attack", meta = (ClampMin = "0", UIMin = "0", UIMax = "10"))
	int32 AoECadence = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Attack", meta = (ClampMin = "50.0", UIMin = "50.0", UIMax = "2000.0"))
	float TelegraphedAoERadius = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Attack", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float TelegraphedAoEDamage = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Attack", meta = (ClampMin = "0.2", UIMin = "0.2", UIMax = "5.0"))
	float TelegraphWindup = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Attack")
	TSubclassOf<USODamageType> AoEDamageType;

	// Phase-based scaling multipliers relative to the base values.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Phase 2", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "5.0"))
	float Phase2DamageMult = 1.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Phase 2", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "3.0"))
	float Phase2SpeedMult = 1.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Phase 3", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "5.0"))
	float Phase3DamageMult = 1.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Phase 3", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "3.0"))
	float Phase3SpeedMult = 1.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Phase 3", meta = (ClampMin = "0", UIMin = "0", UIMax = "10"))
	int32 Phase3AoECadence = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Boss|Debug")
	bool bDrawTelegraphDebug = true;

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Boss")
	ESOBossPhase GetCurrentPhase() const { return CurrentPhase; }

	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Boss")
	void OnPhaseEnteredBP(ESOBossPhase NewPhase);

	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Boss")
	void OnTelegraphAoEStart(const FVector& TelegraphCenter, float Radius, float Windup);

	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|Boss")
	void OnTelegraphAoEResolve(const FVector& ImpactCenter, float Radius, int32 EnemiesHit);

private:
	int32 SwingsSinceLastAoE = 0;
	bool  bAoEPending        = false;

	ESOBossPhase CurrentPhase = ESOBossPhase::Phase1;

	// Cached base values so we can re-scale relative to them on each phase change.
	float BaseAttackDamage = 0.0f;
	float BaseMovementSpeed = 0.0f;
};
