#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOShadowBoltProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class USODamageType;

/**
 * Ranged spell projectile. Fired by the player (or later, a caster enemy)
 * toward a target location. Standard sphere-collides-with-first-thing +
 * apply damage + destroy pipeline.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOShadowBoltProjectile : public AActor
{
	GENERATED_BODY()

public:
	ASOShadowBoltProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               FVector NormalImpulse, const FHitResult& Hit);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|ShadowBolt")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|ShadowBolt")
	TObjectPtr<UStaticMeshComponent> BoltMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|ShadowBolt")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	/** Damage applied to the first valid target hit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|ShadowBolt", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float Damage = 40.0f;

	/** Cruising speed of the bolt in cm/s. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|ShadowBolt", meta = (ClampMin = "100.0", UIMin = "100.0", UIMax = "10000.0"))
	float Speed = 2200.0f;

	/** Seconds before the projectile self-destructs if it never hits anything. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|ShadowBolt", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "30.0"))
	float MaxLifetime = 3.0f;

	/** Damage type used on hit. Leave null to fall back to USODamageType. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|ShadowBolt")
	TSubclassOf<USODamageType> DamageType;

	/** Blueprint hook for impact VFX/SFX. HitActor may be null if we hit level geometry with no actor. */
	UFUNCTION(BlueprintImplementableEvent, Category = "SupremeOverlord|ShadowBolt")
	void OnBoltImpact(const FHitResult& Hit, AActor* HitActor);
};
