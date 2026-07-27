#include "SOHUD.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "GameFramework/PlayerController.h"
#include "SOCharacter.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"

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

	// -- Mana bar sits directly above the HP bar ------------------------------
	if (USOManaComponent* Mana = SO->ManaComponent)
	{
		const float CurrentMana = Mana->GetCurrentMana();
		const float MaxManaVal  = FMath::Max(Mana->MaxMana, 1.0f);
		const float ManaPct     = FMath::Clamp(Mana->GetManaPercent(), 0.0f, 1.0f);

		const float ManaX = BarX;
		const float ManaY = BarY - ManaBarSize.Y - ManaBarGap;

		{
			FCanvasTileItem ManaBorder(FVector2D(ManaX - Border, ManaY - Border),
			                           FVector2D(ManaBarSize.X + Border * 2.0f, ManaBarSize.Y + Border * 2.0f),
			                           HealthBarBorderColor);
			ManaBorder.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(ManaBorder);
		}
		{
			FCanvasTileItem ManaBG(FVector2D(ManaX, ManaY), ManaBarSize, ManaBarBackgroundColor);
			ManaBG.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(ManaBG);
		}
		if (ManaPct > 0.0f)
		{
			FCanvasTileItem ManaFill(FVector2D(ManaX, ManaY),
			                         FVector2D(ManaBarSize.X * ManaPct, ManaBarSize.Y),
			                         ManaBarFillColor);
			ManaFill.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(ManaFill);
		}
		if (MediumFont)
		{
			const FString ManaText = FString::Printf(TEXT("MP  %.0f / %.0f"), CurrentMana, MaxManaVal);
			FCanvasTextItem ManaItem(FVector2D(ManaX + 10.0f, ManaY + 1.0f),
			                         FText::FromString(ManaText),
			                         MediumFont,
			                         HealthTextColor);
			ManaItem.Scale = FVector2D(ManaTextScale, ManaTextScale);
			ManaItem.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(ManaItem);
		}
	}

	// -- Gold counter (top-right) --------------------------------------------
	if (bShowGoldCounter && MediumFont)
	{
		const FString GoldText = FString::Printf(TEXT("Gold: %d"), SO->GetGold());

		float TextW = 0.0f;
		float TextH = 0.0f;
		Canvas->TextSize(MediumFont, GoldText, TextW, TextH, GoldCounterScale, GoldCounterScale);

		FCanvasTextItem GoldItem(FVector2D(ScreenW - TextW - GoldCounterOffset.X, GoldCounterOffset.Y),
		                         FText::FromString(GoldText),
		                         MediumFont,
		                         GoldCounterColor);
		GoldItem.Scale = FVector2D(GoldCounterScale, GoldCounterScale);
		GoldItem.EnableShadow(FLinearColor::Black);
		Canvas->DrawItem(GoldItem);
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
