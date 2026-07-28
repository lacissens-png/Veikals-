#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SOBlinkComponent.generated.h"

class ASOCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSOOnBlinked, FVector, FromLocation, FVector, ToLocation);

/**
 * Shadow Step — short-range teleport toward the cursor with a brief i-frame.
 *
 * The destination is clamped to BlinkRange, snapped to the navmesh when one is
 * available, and validated with TeleportTo so the character never lands inside
 * geometry.  While InvulnerabilityDuration is running the owner's
 * USOHealthComponent::bInvulnerable is set, making the dash a defensive tool.
 */
UCLASS(ClassGroup = "SupremeOverlord", meta = (BlueprintSpawnableComponent))
class SUPREMEOVERLORD_API USOBlinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USOBlinkComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	// ---------- Config ----------

	/** Maximum teleport distance (cm). Cursor targets beyond this are clamped. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink", meta = (ClampMin = "50.0", UIMin = "100.0", UIMax = "2000.0"))
	float BlinkRange = 650.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink", meta = (ClampMin = "0.0"))
	float ManaCost = 22.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "30.0"))
	float Cooldown = 5.0f;

	/** Seconds of invulnerability granted the moment the teleport lands. Set to 0 to disable. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "2.0"))
	float InvulnerabilityDuration = 0.18f;

	/** Vertical lift applied to the destination before the teleport test, so the capsule clears small steps. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "200.0"))
	float DestinationHeightOffset = 20.0f;

	/** Radius (cm) used when snapping the destination onto the navmesh. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink", meta = (ClampMin = "10.0", UIMin = "10.0", UIMax = "1000.0"))
	float NavProjectionExtent = 200.0f;

	// ---------- Runtime API ----------

	UFUNCTION(BlueprintPure, Category = "Blink")
	bool CanBlink() const;

	/** Teleports Caster toward CursorLocation. Returns true when the blink happened. */
	UFUNCTION(BlueprintCallable, Category = "Blink")
	bool Blink(FVector CursorLocation, ASOCharacter* Caster);

	UFUNCTION(BlueprintPure, Category = "Blink")
	float GetCooldownRemaining() const { return CooldownRemaining; }

	// ---------- Delegates ----------

	UPROPERTY(BlueprintAssignable, Category = "Blink")
	FSOOnBlinked OnBlinked;

	UFUNCTION(BlueprintImplementableEvent, Category = "Blink")
	void OnBlinkedBP(const FVector& FromLocation, const FVector& ToLocation);

private:
	void ClearInvulnerability();

	float CooldownRemaining = 0.0f;

	FTimerHandle InvulnHandle;

	/** Health component we flagged invulnerable, so we clear the right one. */
	TWeakObjectPtr<class USOHealthComponent> InvulnTarget;
};
