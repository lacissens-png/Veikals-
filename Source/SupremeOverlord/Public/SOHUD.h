#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SOHUD.generated.h"

/**
 * Minimal Canvas-drawn HUD:
 *  - HP bar and numeric readout in the bottom-left
 *  - "YOU DIED" overlay when the player's health component reports dead
 *  - Optional one-line control hints across the top
 *
 * Deliberately Canvas-based (not UMG) so it works out of the box without any
 * widget assets. Swap for a UUserWidget when you want a real designed HUD.
 */
UCLASS()
class SUPREMEOVERLORD_API ASOHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASOHUD();

protected:
	virtual void DrawHUD() override;

public:
	/** Pixel size of the HP bar (width, height). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|HealthBar")
	FVector2D HealthBarSize = FVector2D(340.0f, 26.0f);

	/** Offset (pixels) from the bottom-left corner of the screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|HealthBar")
	FVector2D HealthBarOffset = FVector2D(32.0f, 32.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|HealthBar")
	FLinearColor HealthBarBackgroundColor = FLinearColor(0.05f, 0.0f, 0.0f, 0.85f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|HealthBar")
	FLinearColor HealthBarBorderColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|HealthBar")
	FLinearColor HealthBarFillColor = FLinearColor(0.78f, 0.06f, 0.06f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|HealthBar")
	FLinearColor HealthTextColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|HealthBar", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "3.0"))
	float HealthTextScale = 1.1f;

	/** Show a one-line reminder of the primary keybinds across the top of the screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Hints")
	bool bShowControlHints = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Hints")
	FString ControlHintText = TEXT("LMB Move   |   RMB / Q Primary Attack   |   E Shadow Bolt   |   K Debug Damage");

	/** Show "YOU DIED" overlay when the player's health component reports dead. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Death")
	bool bShowDeathOverlay = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Death")
	FString DeathOverlayText = TEXT("YOU DIED");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Death")
	FLinearColor DeathOverlayColor = FLinearColor(0.85f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Death", meta = (ClampMin = "1.0", UIMin = "1.0", UIMax = "8.0"))
	float DeathOverlayScale = 3.0f;
};
