#include "SOCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASOCharacter::ASOCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength         = CameraDistance;
	CameraBoom->bDoCollisionTest        = false;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch           = false;
	CameraBoom->bInheritYaw             = false;
	CameraBoom->bInheritRoll            = false;
	CameraBoom->SetRelativeRotation(FRotator(-CameraPitch, CameraYaw, 0.0f));

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCamera->bUsePawnControlRotation = false;

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate              = FRotator(0.0f, RotationRateYaw, 0.0f);
		Movement->MaxWalkSpeed              = MovementSpeed;
		Movement->bConstrainToPlane         = true;
		Movement->bSnapToPlaneAtStart       = true;
	}
}

void ASOCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ApplyCameraSettings();
	ApplyMovementSettings();
}

void ASOCharacter::BeginPlay()
{
	Super::BeginPlay();
	ApplyCameraSettings();
	ApplyMovementSettings();
}

void ASOCharacter::ApplyCameraSettings()
{
	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = CameraDistance;
		CameraBoom->SetRelativeRotation(FRotator(-CameraPitch, CameraYaw, 0.0f));
	}
}

void ASOCharacter::ApplyMovementSettings()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = MovementSpeed;
		Movement->RotationRate = FRotator(0.0f, RotationRateYaw, 0.0f);
	}
}
