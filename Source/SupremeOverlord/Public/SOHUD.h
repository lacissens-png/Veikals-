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

	/** Pixel size of the mana bar (width, height). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|ManaBar")
	FVector2D ManaBarSize = FVector2D(340.0f, 18.0f);

	/** Gap in pixels between the top of the HP bar and the bottom of the mana bar. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|ManaBar", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "40.0"))
	float ManaBarGap = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|ManaBar")
	FLinearColor ManaBarBackgroundColor = FLinearColor(0.0f, 0.02f, 0.06f, 0.85f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|ManaBar")
	FLinearColor ManaBarFillColor = FLinearColor(0.15f, 0.35f, 0.95f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|ManaBar", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "3.0"))
	float ManaTextScale = 1.0f;

	/** Show the equipped weapon name in the bottom-right of the screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Weapon")
	bool bShowWeaponLabel = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Weapon")
	FVector2D WeaponLabelOffset = FVector2D(24.0f, 32.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Weapon", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "3.0"))
	float WeaponLabelScale = 1.1f;

	/** Show a gold counter in the top-right of the screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Gold")
	bool bShowGoldCounter = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Gold")
	FVector2D GoldCounterOffset = FVector2D(24.0f, 24.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Gold")
	FLinearColor GoldCounterColor = FLinearColor(1.0f, 0.84f, 0.15f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Gold", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "4.0"))
	float GoldCounterScale = 1.3f;

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
