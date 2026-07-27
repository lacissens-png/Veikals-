#include "SOHUD.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SOAttributesComponent.h"
#include "SOCharacter.h"
#include "SOEnemySpawner.h"
#include "SOExperienceComponent.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"
#include "SOWeaponData.h"

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

	// -- Attributes panel (top-left, under the control hint line) ------------
	if (bShowAttributesPanel && MediumFont)
	{
		if (USOAttributesComponent* Attrs = SO->AttributesComponent)
		{
			const float LineHeight = 20.0f * AttributesTextScale;
			float Y = AttributesPanelOffset.Y;

			auto DrawLine = [&](const FString& Text, const FLinearColor& Color)
			{
				FCanvasTextItem Item(FVector2D(AttributesPanelOffset.X, Y),
				                     FText::FromString(Text),
				                     MediumFont,
				                     Color);
				Item.Scale = FVector2D(AttributesTextScale, AttributesTextScale);
				Item.EnableShadow(FLinearColor::Black);
				Canvas->DrawItem(Item);
				Y += LineHeight;
			};

			DrawLine(FString::Printf(TEXT("STR  %d   (F1)"), Attrs->GetAttribute(ESOAttribute::Strength)),  AttributesTextColor);
			DrawLine(FString::Printf(TEXT("INT  %d   (F2)"), Attrs->GetAttribute(ESOAttribute::Intellect)), AttributesTextColor);
			DrawLine(FString::Printf(TEXT("VIT  %d   (F3)"), Attrs->GetAttribute(ESOAttribute::Vitality)),  AttributesTextColor);

			const int32 Unspent = Attrs->GetUnspentPoints();
			if (Unspent > 0)
			{
				DrawLine(FString::Printf(TEXT("Unspent points: %d"), Unspent), UnspentPointsColor);
			}
		}
	}

	// -- Weapon label (bottom-right, colored by rarity) -----------------------
	if (bShowWeaponLabel && MediumFont)
	{
		USOWeaponData* Weapon = SO->GetEquippedWeapon();

		const FString WeaponText = Weapon
			? FString::Printf(TEXT("%s"), *Weapon->DisplayName.ToString())
			: FString(TEXT("Unarmed"));

		const FLinearColor LabelColor = Weapon ? Weapon->GetRarityColor() : FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);

		float TextW = 0.0f;
		float TextH = 0.0f;
		Canvas->TextSize(MediumFont, WeaponText, TextW, TextH, WeaponLabelScale, WeaponLabelScale);

		FCanvasTextItem WeaponItem(FVector2D(ScreenW - TextW - WeaponLabelOffset.X, ScreenH - TextH - WeaponLabelOffset.Y),
		                           FText::FromString(WeaponText),
		                           MediumFont,
		                           LabelColor);
		WeaponItem.Scale = FVector2D(WeaponLabelScale, WeaponLabelScale);
		WeaponItem.EnableShadow(FLinearColor::Black);
		Canvas->DrawItem(WeaponItem);
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

	// -- Skill panel (row of three tiles centered above the XP bar) ----------
	if (bShowSkillPanel && MediumFont)
	{
		const int32 NumTiles       = 3;
		const float RowWidth       = NumTiles * SkillTileSize.X + (NumTiles - 1) * SkillTileGap;
		const float RowY           = ScreenH - XPBarSize.Y - XPBarBottomOffset - SkillTileSize.Y - SkillPanelGap;
		const float RowStartX      = (ScreenW - RowWidth) * 0.5f;
		const float CurrentManaVal = SO->ManaComponent ? SO->ManaComponent->GetCurrentMana() : 0.0f;

		// Primary attack: no mana cost, cooldown scales with weapon.
		DrawSkillTile(
			Canvas,
			FVector2D(RowStartX + 0 * (SkillTileSize.X + SkillTileGap), RowY),
			TEXT("RMB"),
			TEXT("Strike"),
			0.0f,
			CurrentManaVal,
			SO->GetPrimaryAttackCooldownRemaining(),
			SO->GetEffectivePrimaryAttackCooldown(),
			FLinearColor(0.85f, 0.10f, 0.10f, 1.0f),
			MediumFont);

		// Shadow Bolt
		DrawSkillTile(
			Canvas,
			FVector2D(RowStartX + 1 * (SkillTileSize.X + SkillTileGap), RowY),
			TEXT("E"),
			TEXT("Shadow"),
			SO->ShadowBoltManaCost,
			CurrentManaVal,
			SO->GetShadowBoltCooldownRemaining(),
			SO->ShadowBoltCooldown,
			FLinearColor(0.35f, 0.10f, 0.65f, 1.0f),
			MediumFont);

		// Life Drain
		DrawSkillTile(
			Canvas,
			FVector2D(RowStartX + 2 * (SkillTileSize.X + SkillTileGap), RowY),
			TEXT("R"),
			TEXT("Drain"),
			SO->LifeDrainManaCost,
			CurrentManaVal,
			SO->GetLifeDrainCooldownRemaining(),
			SO->LifeDrainCooldown,
			FLinearColor(0.55f, 0.05f, 0.30f, 1.0f),
			MediumFont);
	}

	// -- XP bar (bottom-center) + level number -------------------------------
	if (bShowXPBar)
	{
		USOExperienceComponent* XP = SO->ExperienceComponent;
		if (XP)
		{
			const float XPPct = FMath::Clamp(XP->GetLevelProgressPercent(), 0.0f, 1.0f);

			const float XPBarX = (ScreenW - XPBarSize.X) * 0.5f;
			const float XPBarY = ScreenH - XPBarSize.Y - XPBarBottomOffset;

			{
				FCanvasTileItem XPBorder(FVector2D(XPBarX - Border, XPBarY - Border),
				                         FVector2D(XPBarSize.X + Border * 2.0f, XPBarSize.Y + Border * 2.0f),
				                         HealthBarBorderColor);
				XPBorder.BlendMode = SE_BLEND_Translucent;
				Canvas->DrawItem(XPBorder);
			}
			{
				FCanvasTileItem XPBG(FVector2D(XPBarX, XPBarY), XPBarSize, XPBarBackgroundColor);
				XPBG.BlendMode = SE_BLEND_Translucent;
				Canvas->DrawItem(XPBG);
			}
			if (XPPct > 0.0f)
			{
				FCanvasTileItem XPFill(FVector2D(XPBarX, XPBarY),
				                       FVector2D(XPBarSize.X * XPPct, XPBarSize.Y),
				                       XPBarFillColor);
				XPFill.BlendMode = SE_BLEND_Translucent;
				Canvas->DrawItem(XPFill);
			}

			if (MediumFont)
			{
				const FString LvlText = FString::Printf(TEXT("Lv %d"), XP->GetCurrentLevel());
				float TextW = 0.0f;
				float TextH = 0.0f;
				Canvas->TextSize(MediumFont, LvlText, TextW, TextH, XPLabelScale, XPLabelScale);

				FCanvasTextItem LvlItem(FVector2D(ScreenW * 0.5f - TextW * 0.5f, XPBarY - TextH - 4.0f),
				                        FText::FromString(LvlText),
				                        MediumFont,
				                        XPTextColor);
				LvlItem.Scale = FVector2D(XPLabelScale, XPLabelScale);
				LvlItem.EnableShadow(FLinearColor::Black);
				Canvas->DrawItem(LvlItem);
			}
		}
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

	// -- Wave counter (top-center; sums across every spawner in the level) ---
	if (bShowWaveCounter && MediumFont)
	{
		TArray<AActor*> Spawners;
		UGameplayStatics::GetAllActorsOfClass(this, ASOEnemySpawner::StaticClass(), Spawners);

		if (Spawners.Num() > 0)
		{
			int32 MaxWave        = 0;
			int32 AliveTotal     = 0;
			int32 TargetTotal    = 0;
			bool  bEveryoneDone  = true;

			for (AActor* A : Spawners)
			{
				if (const ASOEnemySpawner* Spawner = Cast<ASOEnemySpawner>(A))
				{
					MaxWave      = FMath::Max(MaxWave, Spawner->GetCurrentWave());
					AliveTotal  += Spawner->GetAliveEnemyCount();
					TargetTotal += Spawner->GetTargetEnemyCount();
					if (!Spawner->AreAllWavesCleared())
					{
						bEveryoneDone = false;
					}
				}
			}

			FString WaveText;
			if (bEveryoneDone)
			{
				WaveText = TEXT("All waves cleared");
			}
			else
			{
				WaveText = FString::Printf(TEXT("Wave %d  -  %d / %d enemies"), MaxWave, AliveTotal, TargetTotal);
			}

			float TW = 0.0f, TH = 0.0f;
			Canvas->TextSize(MediumFont, WaveText, TW, TH, WaveCounterScale, WaveCounterScale);

			FCanvasTextItem WaveItem(FVector2D((ScreenW - TW) * 0.5f, WaveCounterTopOffset),
			                         FText::FromString(WaveText),
			                         MediumFont,
			                         WaveCounterColor);
			WaveItem.Scale = FVector2D(WaveCounterScale, WaveCounterScale);
			WaveItem.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(WaveItem);
		}
	}

	// -- Pause overlay (drawn last so it sits over everything) ---------------
	if (bShowPauseOverlay && UGameplayStatics::IsGamePaused(this))
	{
		{
			FCanvasTileItem Dim(FVector2D(0.0f, 0.0f), FVector2D(ScreenW, ScreenH), PauseDimColor);
			Dim.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(Dim);
		}

		if (LargeFont)
		{
			const float TitleScale     = 2.5f;
			const float ApproxHalfW    = PauseTitleText.Len() * 6.0f * TitleScale;
			FCanvasTextItem Title(FVector2D(ScreenW * 0.5f - ApproxHalfW, ScreenH * 0.42f),
			                      FText::FromString(PauseTitleText),
			                      LargeFont,
			                      FLinearColor(0.98f, 0.85f, 0.4f, 1.0f));
			Title.Scale = FVector2D(TitleScale, TitleScale);
			Title.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(Title);
		}

		if (MediumFont)
		{
			float TW = 0.0f, TH = 0.0f;
			Canvas->TextSize(MediumFont, PauseHintText, TW, TH, 1.2f, 1.2f);
			FCanvasTextItem Hint(FVector2D((ScreenW - TW) * 0.5f, ScreenH * 0.55f),
			                     FText::FromString(PauseHintText),
			                     MediumFont,
			                     FLinearColor(0.95f, 0.95f, 0.95f, 1.0f));
			Hint.Scale = FVector2D(1.2f, 1.2f);
			Hint.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(Hint);
		}
	}
}

void ASOHUD::DrawSkillTile(UCanvas* InCanvas,
                            const FVector2D& Origin,
                            const FString& KeyLabel,
                            const FString& SkillName,
                            float ManaCost,
                            float CurrentMana,
                            float CooldownRemaining,
                            float CooldownTotal,
                            const FLinearColor& TileColor,
                            UFont* SmallFont)
{
	if (!InCanvas || !SmallFont)
	{
		return;
	}

	const float Border = 2.0f;

	// Border
	{
		FCanvasTileItem BorderItem(FVector2D(Origin.X - Border, Origin.Y - Border),
		                           FVector2D(SkillTileSize.X + Border * 2.0f, SkillTileSize.Y + Border * 2.0f),
		                           SkillTileBorder);
		BorderItem.BlendMode = SE_BLEND_Translucent;
		InCanvas->DrawItem(BorderItem);
	}

	// If mana-locked, wash the tile with a desaturated grey to signal "not enough mana".
	const bool bManaLocked = ManaCost > 0.0f && CurrentMana + KINDA_SMALL_NUMBER < ManaCost;
	const FLinearColor UsedColor = bManaLocked
		? FLinearColor(TileColor.R * 0.35f, TileColor.G * 0.35f, TileColor.B * 0.35f, TileColor.A)
		: TileColor;

	// Base tile fill
	{
		FCanvasTileItem Base(Origin, SkillTileSize, UsedColor);
		Base.BlendMode = SE_BLEND_Translucent;
		InCanvas->DrawItem(Base);
	}

	// Cooldown overlay grows top-down as the cooldown ticks.
	if (CooldownRemaining > 0.0f && CooldownTotal > 0.0f)
	{
		const float Pct = FMath::Clamp(CooldownRemaining / CooldownTotal, 0.0f, 1.0f);
		FCanvasTileItem CD(Origin, FVector2D(SkillTileSize.X, SkillTileSize.Y * Pct), SkillCooldownOverlay);
		CD.BlendMode = SE_BLEND_Translucent;
		InCanvas->DrawItem(CD);

		const FString CDText = FString::Printf(TEXT("%.1fs"), CooldownRemaining);
		FCanvasTextItem CDItem(FVector2D(Origin.X + SkillTileSize.X * 0.5f - 14.0f, Origin.Y + SkillTileSize.Y * 0.5f - 8.0f),
		                       FText::FromString(CDText),
		                       SmallFont,
		                       FLinearColor::White);
		CDItem.EnableShadow(FLinearColor::Black);
		InCanvas->DrawItem(CDItem);
	}

	// Key label (top-left of tile).
	{
		FCanvasTextItem KeyItem(FVector2D(Origin.X + 4.0f, Origin.Y + 2.0f),
		                        FText::FromString(KeyLabel),
		                        SmallFont,
		                        FLinearColor(1.0f, 1.0f, 1.0f, 0.95f));
		KeyItem.EnableShadow(FLinearColor::Black);
		InCanvas->DrawItem(KeyItem);
	}

	// Skill name (bottom center of tile).
	{
		float TW = 0.0f, TH = 0.0f;
		InCanvas->TextSize(SmallFont, SkillName, TW, TH);
		FCanvasTextItem NameItem(FVector2D(Origin.X + (SkillTileSize.X - TW) * 0.5f, Origin.Y + SkillTileSize.Y - TH - 3.0f),
		                         FText::FromString(SkillName),
		                         SmallFont,
		                         FLinearColor(1.0f, 1.0f, 1.0f, 0.95f));
		NameItem.EnableShadow(FLinearColor::Black);
		InCanvas->DrawItem(NameItem);
	}

	// Mana cost just under the tile.
	if (ManaCost > 0.0f)
	{
		const FString CostText = FString::Printf(TEXT("%.0f MP"), ManaCost);
		float TW = 0.0f, TH = 0.0f;
		InCanvas->TextSize(SmallFont, CostText, TW, TH);
		FCanvasTextItem CostItem(FVector2D(Origin.X + (SkillTileSize.X - TW) * 0.5f, Origin.Y + SkillTileSize.Y + 3.0f),
		                         FText::FromString(CostText),
		                         SmallFont,
		                         bManaLocked ? FLinearColor(1.0f, 0.35f, 0.35f, 1.0f) : FLinearColor(0.55f, 0.75f, 1.0f, 1.0f));
		CostItem.EnableShadow(FLinearColor::Black);
		InCanvas->DrawItem(CostItem);
	}
}
