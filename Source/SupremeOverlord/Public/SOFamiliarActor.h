#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOFamiliarActor.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class ASOCharacter;
class USODamageType;

/**
 * Permanent hovering companion — distinct from summoned ASOMinion instances:
 * it never expires, isn't part of the minion cap, and doesn't fight in melee.
 * It follows the owner at a fixed offset and periodically zaps the nearest
 * enemy in range for light chip damage, Overlord-familiar style.
 *
 * Spawned once by ASOCharacter::BeginPlay when FamiliarClass is set — no key
 * binding needed, it's a passive companion rather than a cast ability.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOFamiliarActor : public AActor
{
	GENERATED_BODY()

public:
	ASOFamiliarActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	void PerformZap();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Familiar|Components")
	TObjectPtr<USceneComponent> FamiliarRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Familiar|Components")
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	/** Set by ASOCharacter right after spawning — who this familiar follows and fights for. */
	UPROPERTY(Transient)
	TWeakObjectPtr<ASOCharacter> OwnerCharacter;

	// ---------- Follow ----------

	/** Hover offset relative to the owner: X = forward/back, Y = left/right, Z = up. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Familiar|Follow")
	FVector FollowOffset = FVector(-150.0f, 100.0f, 120.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Familiar|Follow", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "20.0"))
	float FollowInterpSpeed = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Familiar|Follow", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "50.0"))
	float BobAmplitude = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Familiar|Follow", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "5.0"))
	float BobRateHz = 0.6f;

	// ---------- Zap ----------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Familiar|Zap", meta = (ClampMin = "0.0"))
	float ZapDamage = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Familiar|Zap", meta = (ClampMin = "100.0", UIMin = "100.0", UIMax = "3000.0"))
	float ZapRange = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Familiar|Zap", meta = (ClampMin = "0.2", UIMin = "0.2", UIMax = "10.0"))
	float ZapInterval = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Familiar|Zap")
	TSubclassOf<USODamageType> ZapDamageType;

	UFUNCTION(BlueprintImplementableEvent, Category = "Familiar|Zap")
	void OnZapPerformed(AActor* Target);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Familiar|Audio")
	TObjectPtr<class USoundBase> ZapSFX;

private:
	FTimerHandle ZapTimerHandle;
	float TimeAlive = 0.0f;
};
