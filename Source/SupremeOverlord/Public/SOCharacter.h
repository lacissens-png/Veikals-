#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SOCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SUPREMEOVERLORD_API ASOCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASOCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	/** Fixed isometric spring arm - not driven by pawn rotation. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera used for the isometric view. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Camera")
	TObjectPtr<UCameraComponent> TopDownCamera;

	/** Distance from the character to the camera along the spring arm. Tweak in editor to zoom in/out. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Camera", meta = (ClampMin = "100.0", ClampMax = "5000.0", UIMin = "100.0", UIMax = "3000.0"))
	float CameraDistance = 1200.0f;

	/** Downward pitch of the camera in degrees. 45-60 gives the classic ARPG isometric feel. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Camera", meta = (ClampMin = "20.0", ClampMax = "89.0", UIMin = "30.0", UIMax = "75.0"))
	float CameraPitch = 55.0f;

	/** World yaw the camera locks to. Diablo-style views typically sit at -45 for a slight iso rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Camera")
	float CameraYaw = -45.0f;

	/** Ground movement speed applied to the CharacterMovementComponent. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Movement", meta = (ClampMin = "100.0", UIMin = "100.0", UIMax = "1500.0"))
	float MovementSpeed = 600.0f;

	/** Rotation rate (yaw deg/s) used to orient the character toward its move direction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Movement", meta = (ClampMin = "60.0", UIMin = "60.0", UIMax = "1440.0"))
	float RotationRateYaw = 720.0f;

	/** Applies CameraDistance / CameraPitch / CameraYaw to the spring arm. Also callable from Blueprint after tweaking values. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Camera")
	void ApplyCameraSettings();

	/** Applies MovementSpeed / RotationRateYaw to the movement component. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Movement")
	void ApplyMovementSettings();
};
