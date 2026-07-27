#include "SOHUD.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "GameFramework/PlayerController.h"
#include "SOCharacter.h"
#include "SOHealthComponent.h"

ASOHUD::ASOHUD()
{
}

void ASOHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas)
	{
		return;
	}

	// The HUD is owned by the player controller — fetch its pawn.
	APlayerController* PC = GetOwningPlayerController();
	ASOCharacter*      SO = PC ? Cast<ASOCharacter>(PC->GetPawn()) : nullptr;

	UFont* MediumFont = GEngine ? GEngine->GetMediumFont() : nullptr;
	UFont* LargeFont  = GEngine ? GEngine->GetLargeFont()  : MediumFont;

	// -- Control hints --------------------------------------------------------
	if (bShowControlHints && MediumFont && !ControlHintText.IsEmpty())
	{
		FCanvasTextItem HintText(FVector2D(HealthBarOffset.X, 14.0f),
		                         FText::FromString(ControlHintText),
		                         MediumFont,
		                         FLinearColor(0.92f, 0.92f, 0.92f, 0.85f));
		HintText.EnableShadow(FLinearColor::Black);
		Canvas->DrawItem(HintText);
	}

	if (!SO || !SO->HealthComponent)
	{
		return;
	}

	USOHealthComponent* Health = SO->HealthComponent;
	const float CurrentHP = Health->GetCurrentHealth();
	const float MaxHP     = FMath::Max(Health->MaxHealth, 1.0f);
	const float Pct       = FMath::Clamp(Health->GetHealthPercent(), 0.0f, 1.0f);

	const float ScreenW = Canvas->SizeX;
	const float ScreenH = Canvas->SizeY;

	const float BarX = HealthBarOffset.X;
	const float BarY = ScreenH - HealthBarSize.Y - HealthBarOffset.Y;

	// -- HP bar background (with a 1px darker border) -------------------------
	const float Border = 2.0f;
	{
		FCanvasTileItem BorderTile(FVector2D(BarX - Border, BarY - Border),
		                           FVector2D(HealthBarSize.X + Border * 2.0f, HealthBarSize.Y + Border * 2.0f),
		                           HealthBarBorderColor);
		BorderTile.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(BorderTile);
	}
	{
		FCanvasTileItem BG(FVector2D(BarX, BarY), HealthBarSize, HealthBarBackgroundColor);
		BG.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(BG);
	}

	// -- HP bar fill ----------------------------------------------------------
	if (Pct > 0.0f)
	{
		FCanvasTileItem Fill(FVector2D(BarX, BarY),
		                     FVector2D(HealthBarSize.X * Pct, HealthBarSize.Y),
		                     HealthBarFillColor);
		Fill.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(Fill);
	}

	// -- HP numeric readout centered vertically on the bar --------------------
	if (MediumFont)
	{
		const FString HPText = FString::Printf(TEXT("HP  %.0f / %.0f"), CurrentHP, MaxHP);
		FCanvasTextItem HPItem(FVector2D(BarX + 10.0f, BarY + 3.0f),
		                       FText::FromString(HPText),
		                       MediumFont,
		                       HealthTextColor);
		HPItem.Scale = FVector2D(HealthTextScale, HealthTextScale);
		HPItem.EnableShadow(FLinearColor::Black);
		Canvas->DrawItem(HPItem);
	}

	// -- Death overlay --------------------------------------------------------
	if (bShowDeathOverlay && Health->IsDead() && LargeFont)
	{
		// Rough visual centering — Canvas has TextSize but eyeballing is fine here.
		const float ApproxHalfWidth = DeathOverlayText.Len() * 6.0f * DeathOverlayScale;
		FCanvasTextItem DeadItem(FVector2D(ScreenW * 0.5f - ApproxHalfWidth, ScreenH * 0.45f),
		                         FText::FromString(DeathOverlayText),
		                         LargeFont,
		                         DeathOverlayColor);
		DeadItem.Scale = FVector2D(DeathOverlayScale, DeathOverlayScale);
		DeadItem.EnableShadow(FLinearColor::Black);
		Canvas->DrawItem(DeadItem);
	}
}
