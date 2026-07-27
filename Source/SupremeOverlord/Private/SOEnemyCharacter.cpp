#include "SOEnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "SODamageType.h"
#include "SOEnemyAIController.h"
#include "SOHealthComponent.h"

ASOEnemyCharacter::ASOEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	HealthComponent = CreateDefaultSubobject<USOHealthComponent>(TEXT("HealthComponent"));

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate              = FRotator(0.0f, 640.0f, 0.0f);
		Movement->MaxWalkSpeed              = MovementSpeed;
		Movement->bConstrainToPlane         = true;
		Movement->bSnapToPlaneAtStart       = true;
	}

	// Ensure the AI controller runs as soon as the enemy is placed/spawned.
	AIControllerClass    = ASOEnemyAIController::StaticClass();
	AutoPossessAI        = EAutoPossessAI::PlacedInWorldOrSpawned;
	AutoPossessPlayer    = EAutoReceiveInput::Disabled;

	// Sensible default so the enemy blocks the player but pawns don't push through each other.
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionProfileName(TEXT("Pawn"));
	}
}

void ASOEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = MovementSpeed;
	}

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ASOEnemyCharacter::HandleDeath);
	}
}

bool ASOEnemyCharacter::IsAlive() const
{
	return HealthComponent ? HealthComponent->IsAlive() : true;
}

bool ASOEnemyCharacter::PerformAttack(AActor* Target)
{
	if (!Target || !IsAlive() || AttackDamage <= 0.0f)
	{
		return false;
	}

	TSubclassOf<UDamageType> DTClass = AttackDamageType ? AttackDamageType : TSubclassOf<UDamageType>(USODamageType::StaticClass());

	const float Applied = UGameplayStatics::ApplyDamage(
		Target,
		AttackDamage,
		GetController(),
		this,
		DTClass);

	return Applied > 0.0f;
}

void ASOEnemyCharacter::HandleDeath(USOHealthComponent* /*OwningComponent*/, AController* /*InstigatedBy*/, AActor* /*DamageCauser*/)
{
	if (AController* MyController = GetController())
	{
		MyController->StopMovement();
		MyController->UnPossess();
	}

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->DisableMovement();
		Movement->StopMovementImmediately();
	}

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (CorpseLifetime > 0.0f)
	{
		SetLifeSpan(CorpseLifetime);
	}
}
