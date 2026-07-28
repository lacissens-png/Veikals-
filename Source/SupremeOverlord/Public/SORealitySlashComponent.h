#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SORealitySlashComponent.generated.h"

class ASOCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnRealitySlash, AActor*, Target, bool, bWasInstantKill);

/**
 * Overlord-flavored signature spell: an instant-kill strike on the nearest
 * enemy to the cursor — Ainz's "Grasp Heart"/"Reality Slash", spells that
 * work on ordinary monsters but explicitly fail against "Players" (bosses)
 * in the source material. Mirrored here: any ASOEnemyCharacter that isn't a
 * ASOBossCharacter and doesn't carry an active USOEliteComponent dies
 * outright via USOHealthComponent::Kill (which still drives the normal
 * OnDeath flow — loot/XP/corruption/bestiary/quest credit all fire exactly
 * as they would for a regular kill). Anything that resists instead takes a
 * heavy burst of USORealitySlashDamageType damage (Category = True,
 * bIgnoresResistances = true) as a consolation.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USORealitySlashComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USORealitySlashComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// ---------- Config ----------

	/** Max distance from the cursor to find a target. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealitySlash", meta = (ClampMin = "50.0", UIMin = "100.0", UIMax = "2000.0"))
	float Range = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealitySlash", meta = (ClampMin = "0.0"))
	float ManaCost = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealitySlash", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "60.0"))
	float Cooldown = 8.0f;

	/** Fallback damage dealt to bosses/elites that resist the instant kill. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealitySlash", meta = (ClampMin = "0.0"))
	float BossFallbackDamage = 400.0f;

	/** Defaults to USORealitySlashDamageType if unset (True category, ignores resistances). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RealitySlash")
	TSubclassOf<class USODamageType> FallbackDamageType;

	// ---------- Runtime API ----------

	UFUNCTION(BlueprintPure, Category = "RealitySlash")
	bool CanCast() const;

	/** Finds the nearest live enemy to CursorLocation within Range and slashes it. Returns true if a cast happened. */
	UFUNCTION(BlueprintCallable, Category = "RealitySlash")
	bool Cast(FVector CursorLocation, ASOCharacter* Caster);

	UFUNCTION(BlueprintPure, Category = "RealitySlash")
	float GetCooldownRemaining() const { return CooldownRemaining; }

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "RealitySlash")
	FSOOnRealitySlash OnRealitySlash;

	UFUNCTION(BlueprintImplementableEvent, Category = "RealitySlash")
	void OnRealitySlashBP(AActor* Target, bool bWasInstantKill);

private:
	float CooldownRemaining = 0.0f;
};
