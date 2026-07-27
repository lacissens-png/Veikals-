#include "SOCasterEnemyCharacter.h"

#include "Engine/World.h"
#include "SOShadowBoltProjectile.h"

ASOCasterEnemyCharacter::ASOCasterEnemyCharacter()
{
	// Kite-friendly defaults so the AI stops well outside melee range.
	MovementSpeed   = 320.0f;
	SightRadius     = 1800.0f;
	LoseSightRadius = 2600.0f;
	AttackRange     = 1100.0f;
	AttackDamage    = 0.0f;   // Damage is carried by the projectile.
	AttackCooldown  = 2.0f;
	CorpseLifetime  = 5.0f;
}

bool ASOCasterEnemyCharacter::PerformAttack(AActor* Target)
{
	if (!Target || !IsAlive() || !BoltClass)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	const FVector MyLocation = GetActorLocation();
	FVector ToTarget         = Target->GetActorLocation() - MyLocation;
	ToTarget.Z               = 0.0f;
	FVector AimDirection     = ToTarget.GetSafeNormal();
	if (AimDirection.IsNearlyZero())
	{
		AimDirection = GetActorForwardVector();
	}

	const FVector Muzzle  = MyLocation + AimDirection * MuzzleForward + FVector(0.0f, 0.0f, MuzzleHeight);
	const FRotator SpawnR = AimDirection.Rotation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Params.Owner      = this;
	Params.Instigator = this;

	ASOShadowBoltProjectile* Bolt = World->SpawnActor<ASOShadowBoltProjectile>(BoltClass, Muzzle, SpawnR, Params);
	if (!Bolt)
	{
		return false;
	}

	Bolt->Damage = BoltDamage;
	OnCasterAttackFired(Muzzle, Bolt);
	return true;
}
