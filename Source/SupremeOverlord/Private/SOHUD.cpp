#include "SOHUD.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SOAttributesComponent.h"
#include "SOBossCharacter.h"
#include "SOCharacter.h"
#include "SOEnemyCharacter.h"
#include "SOEnemySpawner.h"
#include "SOExperienceComponent.h"
#include "SOHealthComponent.h"
#include "SOManaComponent.h"
#include "SOItemPickup.h"
#include "SOPickupOrb.h"
#include "SODialogueComponent.h"
#include "SODialogueNode.h"
#include "SODialogueNPC.h"
#include "SOQuestComponent.h"
#include "SOQuestData.h"
#include "SOStatusEffectComponent.h"
#include "SOAchievementComponent.h"
#include "SOAuraComponent.h"
#include "SOBestiaryComponent.h"
#include "SOBlinkComponent.h"
#include "SOConsumableComponent.h"
#include "SOCorpseExplosionComponent.h"
#include "SOCorruptionComponent.h"
#include "SODifficultySubsystem.h"
#include "SODodgeRollComponent.h"
#include "SOEquipmentComponent.h"
#include "SOSummonComponent.h"
#include "SOWaypoint.h"
#include "SOWaypointComponent.h"
#include "SOTrap.h"
#include "SOVendorNPC.h"
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

	// -- Status effect icons (row directly above the mana bar) ---------------
	if (bShowStatusEffects && SO->StatusEffectComponent)
	{
		const TArray<FSOStatusEffect>& Effects = SO->StatusEffectComponent->GetActiveEffects();
		if (Effects.Num() > 0 && MediumFont)
		{
			const float ManaTopY  = BarY - ManaBarSize.Y - ManaBarGap;
			const float IconRowY  = ManaTopY - StatusIconSize.Y - StatusIconGap;
			float IconX = BarX;

			for (const FSOStatusEffect& Eff : Effects)
			{
				FLinearColor TileColor;
				FString      Abbrev;
				switch (Eff.Type)
				{
				case ESOStatusEffectType::Burning:   TileColor = FLinearColor(0.90f, 0.30f, 0.05f, 1.0f); Abbrev = TEXT("BRN"); break;
				case ESOStatusEffectType::Poisoned:  TileColor = FLinearColor(0.20f, 0.75f, 0.10f, 1.0f); Abbrev = TEXT("PSN"); break;
				case ESOStatusEffectType::Frozen:    TileColor = FLinearColor(0.40f, 0.80f, 1.00f, 1.0f); Abbrev = TEXT("FRZ"); break;
				case ESOStatusEffectType::Slowed:    TileColor = FLinearColor(0.25f, 0.65f, 0.75f, 1.0f); Abbrev = TEXT("SLW"); break;
				case ESOStatusEffectType::Shocked:   TileColor = FLinearColor(1.00f, 0.92f, 0.15f, 1.0f); Abbrev = TEXT("SHK"); break;
				case ESOStatusEffectType::Blessed:   TileColor = FLinearColor(0.95f, 0.90f, 0.35f, 1.0f); Abbrev = TEXT("BLS"); break;
				case ESOStatusEffectType::Cursed:    TileColor = FLinearColor(0.55f, 0.00f, 0.85f, 1.0f); Abbrev = TEXT("CRS"); break;
				default:                             TileColor = FLinearColor::White;                      Abbrev = TEXT("???"); break;
				}

				// Black border
				{
					FCanvasTileItem BorderTile(FVector2D(IconX - 1.0f, IconRowY - 1.0f),
					                           FVector2D(StatusIconSize.X + 2.0f, StatusIconSize.Y + 2.0f),
					                           FLinearColor::Black);
					BorderTile.BlendMode = SE_BLEND_Translucent;
					Canvas->DrawItem(BorderTile);
				}
				// Colored fill (dimmed so text stays readable)
				{
					FLinearColor FillColor(TileColor.R * 0.55f, TileColor.G * 0.55f, TileColor.B * 0.55f, 0.90f);
					FCanvasTileItem Fill(FVector2D(IconX, IconRowY), StatusIconSize, FillColor);
					Fill.BlendMode = SE_BLEND_Translucent;
					Canvas->DrawItem(Fill);
				}

				// 3-letter abbreviation centered in the tile
				{
					float TW = 0.0f, TH = 0.0f;
					Canvas->TextSize(MediumFont, Abbrev, TW, TH, 0.85f, 0.85f);
					FCanvasTextItem AbbrevItem(FVector2D(IconX + (StatusIconSize.X - TW) * 0.5f, IconRowY + 3.0f),
					                           FText::FromString(Abbrev),
					                           MediumFont,
					                           FLinearColor::White);
					AbbrevItem.Scale = FVector2D(0.85f, 0.85f);
					AbbrevItem.EnableShadow(FLinearColor::Black);
					Canvas->DrawItem(AbbrevItem);
				}
				// Remaining duration at the bottom of the tile
				{
					const FString DurStr = FString::Printf(TEXT("%.1f"), Eff.RemainingDuration);
					float DW = 0.0f, DH = 0.0f;
					Canvas->TextSize(MediumFont, DurStr, DW, DH, 0.75f, 0.75f);
					FCanvasTextItem DurItem(FVector2D(IconX + (StatusIconSize.X - DW) * 0.5f,
					                                  IconRowY + StatusIconSize.Y - DH - 2.0f),
					                        FText::FromString(DurStr),
					                        MediumFont,
					                        FLinearColor(0.95f, 0.95f, 0.95f, 0.90f));
					DurItem.Scale = FVector2D(0.75f, 0.75f);
					DurItem.EnableShadow(FLinearColor::Black);
					Canvas->DrawItem(DurItem);
				}
				// Stack badge in the top-right corner when stacked
				if (Eff.CurrentStacks > 1)
				{
					const FString StackStr = FString::FromInt(Eff.CurrentStacks);
					FCanvasTextItem StackItem(FVector2D(IconX + StatusIconSize.X - 9.0f, IconRowY + 1.0f),
					                          FText::FromString(StackStr),
					                          MediumFont,
					                          FLinearColor(1.0f, 0.90f, 0.20f, 1.0f));
					StackItem.Scale = FVector2D(0.70f, 0.70f);
					StackItem.EnableShadow(FLinearColor::Black);
					Canvas->DrawItem(StackItem);
				}

				IconX += StatusIconSize.X + StatusIconGap;
			}
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

			if (SO->EquipmentComponent)
			{
				const TArray<FString>& SetBonuses = SO->EquipmentComponent->GetActiveSetBonusDescriptions();
				for (const FString& Line : SetBonuses)
				{
					DrawLine(Line, FLinearColor(0.10f, 0.85f, 0.30f, 1.0f)); // matches USOItemData's Set rarity tint
				}
			}
		}
	}

	// -- Quest tracker (left side, below attributes panel) -------------------
	if (bShowQuestTracker && MediumFont && SO->QuestComponent)
	{
		const TArray<FSOActiveQuest>& ActiveQuests = SO->QuestComponent->ActiveQuests;
		const float LineH = 18.0f * QuestTrackerScale;
		float QY = QuestTrackerTopOffset;
		const float QX = HealthBarOffset.X;
		const int32 MaxShow = FMath::Min(MaxQuestsInTracker, ActiveQuests.Num());

		for (int32 q = 0; q < MaxShow; ++q)
		{
			const FSOActiveQuest& AQ = ActiveQuests[q];
			if (!AQ.Data) continue;

			// Quest title
			{
				FCanvasTextItem TitleItem(FVector2D(QX, QY),
				                         AQ.Data->QuestTitle,
				                         MediumFont,
				                         QuestTitleColor);
				TitleItem.Scale = FVector2D(QuestTrackerScale * 1.1f, QuestTrackerScale * 1.1f);
				TitleItem.EnableShadow(FLinearColor::Black);
				Canvas->DrawItem(TitleItem);
				QY += LineH * 1.3f;
			}

			// Objectives
			for (int32 i = 0; i < AQ.Data->Objectives.Num(); ++i)
			{
				const FSOQuestObjective& Obj = AQ.Data->Objectives[i];
				const int32 Have     = AQ.Progress.IsValidIndex(i) ? AQ.Progress[i] : 0;
				const bool  bDone    = Have >= Obj.RequiredCount;
				const FString Check  = bDone ? TEXT("[x] ") : TEXT("[ ] ");
				const FString ObjStr = FString::Printf(TEXT("%s%s  %d / %d"),
				                                       *Check,
				                                       *Obj.Description.ToString(),
				                                       Have, Obj.RequiredCount);

				FCanvasTextItem ObjItem(FVector2D(QX + 8.0f, QY),
				                        FText::FromString(ObjStr),
				                        MediumFont,
				                        bDone ? QuestObjectiveCompleteColor : QuestObjectiveColor);
				ObjItem.Scale = FVector2D(QuestTrackerScale, QuestTrackerScale);
				ObjItem.EnableShadow(FLinearColor::Black);
				Canvas->DrawItem(ObjItem);
				QY += LineH;
			}

			QY += LineH * 0.8f; // inter-quest gap
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

	// -- Difficulty label (top-right, under the gold counter) ---------------
	if (bShowDifficultyLabel && MediumFont)
	{
		if (const USODifficultySubsystem* Difficulty = USODifficultySubsystem::Get(GetWorld()))
		{
			const FString DiffText = FString::Printf(TEXT("Difficulty: %s"), *Difficulty->GetDifficultyDisplayName().ToString());

			FLinearColor DiffColor;
			switch (Difficulty->GetDifficultyTier())
			{
			case ESODifficultyTier::Hard:      DiffColor = FLinearColor(0.95f, 0.75f, 0.25f, 1.0f); break;
			case ESODifficultyTier::Nightmare: DiffColor = FLinearColor(0.95f, 0.45f, 0.15f, 1.0f); break;
			case ESODifficultyTier::Torment:   DiffColor = FLinearColor(0.90f, 0.15f, 0.15f, 1.0f); break;
			default:                           DiffColor = FLinearColor(0.75f, 0.75f, 0.75f, 1.0f); break;
			}

			float TextW = 0.0f, TextH = 0.0f;
			Canvas->TextSize(MediumFont, DiffText, TextW, TextH);

			FCanvasTextItem DiffItem(FVector2D(ScreenW - TextW - GoldCounterOffset.X, GoldCounterOffset.Y + 26.0f),
			                         FText::FromString(DiffText),
			                         MediumFont,
			                         DiffColor);
			DiffItem.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(DiffItem);
		}
	}

	// -- Skill panel (row of seven tiles centered above the XP bar) ---------
	if (bShowSkillPanel && MediumFont)
	{
		const int32 NumTiles       = 12;
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

		// Summon Minion
		{
			const float SummonMana = SO->SummonComponent ? SO->SummonComponent->ManaCostPerSummon : 0.0f;
			const FVector2D TileOrigin(RowStartX + 3 * (SkillTileSize.X + SkillTileGap), RowY);
			DrawSkillTile(
				Canvas,
				TileOrigin,
				TEXT("T"),
				TEXT("Summon"),
				SummonMana,
				CurrentManaVal,
				SO->GetSummonCooldownRemaining(),
				SO->GetSummonCooldown(),
				FLinearColor(0.15f, 0.50f, 0.85f, 1.0f),
				MediumFont);

			// Minion count badge below the tile (replaces or accompanies the mana cost label)
			if (SO->SummonComponent)
			{
				const int32 MaxM    = SO->SummonComponent->MaxMinions;
				// GetActiveCount is non-const so we cast
				const int32 ActiveM = const_cast<USOSummonComponent*>(SO->SummonComponent.Get())->GetActiveCount();
				const FString CountStr = FString::Printf(TEXT("%d / %d"), ActiveM, MaxM);
				float CW = 0.0f, CH = 0.0f;
				Canvas->TextSize(MediumFont, CountStr, CW, CH);
				// Sits below the mana cost line (~24px below tile bottom)
				FCanvasTextItem CountItem(
					FVector2D(TileOrigin.X + (SkillTileSize.X - CW) * 0.5f,
					          TileOrigin.Y + SkillTileSize.Y + 22.0f),
					FText::FromString(CountStr),
					MediumFont,
					FLinearColor(0.65f, 0.85f, 1.0f, 1.0f));
				CountItem.EnableShadow(FLinearColor::Black);
				Canvas->DrawItem(CountItem);
			}
		}

		// Trap placement (C key) - golden tile
		{
			const FVector2D TileOrigin(RowStartX + 4 * (SkillTileSize.X + SkillTileGap), RowY);
			DrawSkillTile(
				Canvas,
				TileOrigin,
				TEXT("C"),
				TEXT("Trap"),
				0.0f,
				CurrentManaVal,
				SO->GetTrapPlaceCooldownRemaining(),
				SO->GetTrapPlaceCooldown(),
				FLinearColor(0.70f, 0.55f, 0.05f, 1.0f),
				MediumFont);

			// Trap type label below the tile
			if (bShowTrapIndicator)
			{
				FString TrapName;
				switch (SO->SelectedTrapType)
				{
				case ESOTrapType::ShadowSnare:   TrapName = TEXT("Snare");  break;
				case ESOTrapType::ArcaneMine:    TrapName = TEXT("Mine");   break;
				case ESOTrapType::NecroticSpore: TrapName = TEXT("Spore");  break;
				default:                         TrapName = TEXT("???");    break;
				}
				float TW = 0.0f, TH = 0.0f;
				Canvas->TextSize(MediumFont, TrapName, TW, TH);
				FCanvasTextItem TrapItem(
					FVector2D(TileOrigin.X + (SkillTileSize.X - TW) * 0.5f,
					          TileOrigin.Y + SkillTileSize.Y + 22.0f),
					FText::FromString(TrapName),
					MediumFont,
					TrapIndicatorColor);
				TrapItem.EnableShadow(FLinearColor::Black);
				Canvas->DrawItem(TrapItem);
			}
		}

		// Overlord Mode (Z key) - purple tile that brightens when corruption is full
		if (SO->CorruptionComponent)
		{
			const float CorFrac      = SO->CorruptionComponent->GetCorruptionFraction();
			const bool  bFull        = CorFrac >= 1.0f;
			const bool  bModeActive  = SO->CorruptionComponent->IsOverlordModeActive();
			const float MaxCor       = SO->CorruptionComponent->MaxCorruption;
			const float CurCor       = SO->CorruptionComponent->GetCorruption();

			FLinearColor OverlordColor = bModeActive
				? FLinearColor(0.90f, 0.40f, 1.00f, 1.0f)
				: (bFull ? FLinearColor(0.65f, 0.10f, 0.95f, 1.0f)
				         : FLinearColor(0.30f, 0.02f, 0.50f, 1.0f));

			DrawSkillTile(
				Canvas,
				FVector2D(RowStartX + 5 * (SkillTileSize.X + SkillTileGap), RowY),
				TEXT("Z"),
				TEXT("Overlord"),
				bModeActive ? 0.0f : MaxCor,
				bModeActive ? MaxCor : CurCor,
				0.0f,
				1.0f,
				OverlordColor,
				MediumFont);
		}

		// Necromantic Resurrect (U key) - sickly green tile
		{
			const float NecroMana    = SO->SummonComponent ? SO->SummonComponent->ManaCostPerResurrect : 0.0f;
			const float NecroCDLeft  = SO->GetNecromancyCooldownRemaining();
			const float NecroCDTotal = SO->GetNecromancyCooldown();

			DrawSkillTile(
				Canvas,
				FVector2D(RowStartX + 6 * (SkillTileSize.X + SkillTileGap), RowY),
				TEXT("U"),
				TEXT("Resurrect"),
				NecroMana,
				CurrentManaVal,
				NecroCDLeft,
				NecroCDTotal,
				FLinearColor(0.15f, 0.55f, 0.20f, 1.0f),
				MediumFont);
		}

		// Corpse Explosion (X key) - sickly bone-white tile
		if (SO->CorpseExplosionComponent)
		{
			DrawSkillTile(
				Canvas,
				FVector2D(RowStartX + 7 * (SkillTileSize.X + SkillTileGap), RowY),
				TEXT("X"),
				TEXT("Corpse"),
				SO->CorpseExplosionComponent->ManaCost,
				CurrentManaVal,
				SO->GetCorpseExplosionCooldownRemaining(),
				SO->CorpseExplosionComponent->Cooldown,
				FLinearColor(0.55f, 0.50f, 0.42f, 1.0f),
				MediumFont);
		}

		// Shadow Step / Blink (B key) - deep shadow-blue tile
		if (SO->BlinkComponent)
		{
			DrawSkillTile(
				Canvas,
				FVector2D(RowStartX + 8 * (SkillTileSize.X + SkillTileGap), RowY),
				TEXT("B"),
				TEXT("Blink"),
				SO->BlinkComponent->ManaCost,
				CurrentManaVal,
				SO->GetBlinkCooldownRemaining(),
				SO->BlinkComponent->Cooldown,
				FLinearColor(0.10f, 0.20f, 0.55f, 1.0f),
				MediumFont);
		}

		// Cursed Ground (H key) - sickly violet tile
		{
			DrawSkillTile(
				Canvas,
				FVector2D(RowStartX + 9 * (SkillTileSize.X + SkillTileGap), RowY),
				TEXT("H"),
				TEXT("Curse"),
				SO->CursedGroundManaCost,
				CurrentManaVal,
				SO->GetCursedGroundCooldownRemaining(),
				SO->GetCursedGroundCooldown(),
				FLinearColor(0.40f, 0.10f, 0.45f, 1.0f),
				MediumFont);
		}

		// Dodge Roll (Space key) - neutral steel-grey tile, free but cooldown-gated
		if (SO->DodgeRollComponent)
		{
			DrawSkillTile(
				Canvas,
				FVector2D(RowStartX + 10 * (SkillTileSize.X + SkillTileGap), RowY),
				TEXT("Space"),
				TEXT("Roll"),
				SO->DodgeRollComponent->ManaCost,
				CurrentManaVal,
				SO->GetDodgeRollCooldownRemaining(),
				SO->DodgeRollComponent->Cooldown,
				FLinearColor(0.35f, 0.38f, 0.42f, 1.0f),
				MediumFont);
		}

		// Potion (I key) - deep red tile; ManaCost slot repurposed to show charge count via the label below
		if (SO->ConsumableComponent)
		{
			const FVector2D TileOrigin(RowStartX + 11 * (SkillTileSize.X + SkillTileGap), RowY);
			DrawSkillTile(
				Canvas,
				TileOrigin,
				TEXT("I"),
				TEXT("Potion"),
				0.0f,
				CurrentManaVal,
				SO->GetPotionCooldownRemaining(),
				SO->ConsumableComponent->UseCooldown,
				FLinearColor(0.55f, 0.08f, 0.10f, 1.0f),
				MediumFont);

			const FString ChargeStr = FString::Printf(TEXT("%d / %d"),
				SO->ConsumableComponent->GetCurrentCharges(), SO->ConsumableComponent->MaxCharges);
			float CW = 0.0f, CH = 0.0f;
			Canvas->TextSize(MediumFont, ChargeStr, CW, CH);
			FCanvasTextItem ChargeItem(
				FVector2D(TileOrigin.X + (SkillTileSize.X - CW) * 0.5f, TileOrigin.Y + SkillTileSize.Y + 22.0f),
				FText::FromString(ChargeStr),
				MediumFont,
				FLinearColor(1.0f, 0.6f, 0.6f, 1.0f));
			ChargeItem.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(ChargeItem);
		}
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

	// -- Corruption bar (sits just above the XP bar, same width) -------------
	if (bShowCorruptionBar && SO->CorruptionComponent)
	{
		USOCorruptionComponent* Cor = SO->CorruptionComponent;
		const float CorFrac = FMath::Clamp(Cor->GetCorruptionFraction(), 0.0f, 1.0f);

		const float CorBarX = (ScreenW - CorruptionBarSize.X) * 0.5f;
		const float XPBarY  = ScreenH - XPBarSize.Y - XPBarBottomOffset;
		const float CorBarY = XPBarY - CorruptionBarSize.Y - CorruptionBarGap;

		{
			FCanvasTileItem CorBorder(
				FVector2D(CorBarX - Border, CorBarY - Border),
				FVector2D(CorruptionBarSize.X + Border * 2.0f, CorruptionBarSize.Y + Border * 2.0f),
				HealthBarBorderColor);
			CorBorder.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(CorBorder);
		}
		{
			FCanvasTileItem CorBG(FVector2D(CorBarX, CorBarY), CorruptionBarSize, CorruptionBarBackgroundColor);
			CorBG.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(CorBG);
		}
		if (CorFrac > 0.0f)
		{
			const FLinearColor FillCol = (CorFrac >= 1.0f) ? CorruptionBarFullColor : CorruptionBarFillColor;
			FCanvasTileItem CorFill(
				FVector2D(CorBarX, CorBarY),
				FVector2D(CorruptionBarSize.X * CorFrac, CorruptionBarSize.Y),
				FillCol);
			CorFill.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(CorFill);
		}
	}

	// -- "OVERLORD MODE!" flash (centered, shown while Overlord Mode is active) --
	if (SO->CorruptionComponent && SO->CorruptionComponent->IsOverlordModeActive() && LargeFont)
	{
		const FString FlashText = TEXT("OVERLORD MODE!");
		const float   ApproxHW  = FlashText.Len() * 6.0f * OverlordModeFlashScale;
		FCanvasTextItem FlashItem(
			FVector2D(ScreenW * 0.5f - ApproxHW, ScreenH * 0.38f),
			FText::FromString(FlashText),
			LargeFont,
			OverlordModeFlashColor);
		FlashItem.Scale = FVector2D(OverlordModeFlashScale, OverlordModeFlashScale);
		FlashItem.EnableShadow(FLinearColor::Black);
		Canvas->DrawItem(FlashItem);
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

	// -- Vendor prompt (centered above the skill panel) ----------------------
	if (bShowVendorPrompt && MediumFont)
	{
		TArray<AActor*> Vendors;
		UGameplayStatics::GetAllActorsOfClass(this, ASOVendorNPC::StaticClass(), Vendors);

		ASOVendorNPC* Nearest = nullptr;
		for (AActor* A : Vendors)
		{
			if (ASOVendorNPC* Vendor = Cast<ASOVendorNPC>(A))
			{
				if (Vendor->IsPlayerInRange(SO))
				{
					Nearest = Vendor;
					break;
				}
			}
		}

		if (Nearest)
		{
			const FString Name = Nearest->VendorDisplayName.IsEmpty()
				? FString(TEXT("Vendor"))
				: Nearest->VendorDisplayName.ToString();
			const FString Prompt = FString::Printf(TEXT("%s     [F] Buy Next     [G] Sell Weapon"), *Name);

			float TW = 0.0f, TH = 0.0f;
			Canvas->TextSize(MediumFont, Prompt, TW, TH, VendorPromptScale, VendorPromptScale);
			const float PromptY = ScreenH * 0.5f + 90.0f;

			FCanvasTextItem PromptItem(FVector2D((ScreenW - TW) * 0.5f, PromptY),
			                           FText::FromString(Prompt),
			                           MediumFont,
			                           VendorPromptColor);
			PromptItem.Scale = FVector2D(VendorPromptScale, VendorPromptScale);
			PromptItem.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(PromptItem);
		}
	}

	// -- Boss bar (closest live boss to the player) --------------------------
	if (bShowBossBar && MediumFont)
	{
		TArray<AActor*> Bosses;
		UGameplayStatics::GetAllActorsOfClass(this, ASOBossCharacter::StaticClass(), Bosses);

		ASOBossCharacter* Closest = nullptr;
		float ClosestDistSq = TNumericLimits<float>::Max();
		const FVector PlayerLoc = SO->GetActorLocation();
		for (AActor* A : Bosses)
		{
			ASOBossCharacter* Boss = Cast<ASOBossCharacter>(A);
			if (!Boss || !Boss->IsAlive())
			{
				continue;
			}
			const float DistSq = FVector::DistSquared(PlayerLoc, Boss->GetActorLocation());
			if (DistSq < ClosestDistSq)
			{
				ClosestDistSq = DistSq;
				Closest       = Boss;
			}
		}

		if (Closest && Closest->HealthComponent)
		{
			const float BX = (ScreenW - BossBarSize.X) * 0.5f;
			const float BY = BossBarTopOffset;
			const float Pct = FMath::Clamp(Closest->HealthComponent->GetHealthPercent(), 0.0f, 1.0f);

			{
				FCanvasTileItem BorderItem(FVector2D(BX - Border, BY - Border),
				                           FVector2D(BossBarSize.X + Border * 2.0f, BossBarSize.Y + Border * 2.0f),
				                           HealthBarBorderColor);
				BorderItem.BlendMode = SE_BLEND_Translucent;
				Canvas->DrawItem(BorderItem);
			}
			{
				FCanvasTileItem BG(FVector2D(BX, BY), BossBarSize, BossBarBackgroundColor);
				BG.BlendMode = SE_BLEND_Translucent;
				Canvas->DrawItem(BG);
			}
			if (Pct > 0.0f)
			{
				FCanvasTileItem Fill(FVector2D(BX, BY),
				                     FVector2D(BossBarSize.X * Pct, BossBarSize.Y),
				                     BossBarFillColor);
				Fill.BlendMode = SE_BLEND_Translucent;
				Canvas->DrawItem(Fill);
			}

			const FString Name = Closest->BossDisplayName.IsEmpty()
				? FString(TEXT("Boss"))
				: Closest->BossDisplayName.ToString();
			float TW = 0.0f, TH = 0.0f;
			Canvas->TextSize(MediumFont, Name, TW, TH, BossNameScale, BossNameScale);
			FCanvasTextItem NameItem(FVector2D((ScreenW - TW) * 0.5f, BY - TH - 4.0f),
			                         FText::FromString(Name),
			                         MediumFont,
			                         BossNameColor);
			NameItem.Scale = FVector2D(BossNameScale, BossNameScale);
			NameItem.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(NameItem);
		}
	}

	// -- Minimap (top-right radar around the player) -------------------------
	if (bShowMinimap && MinimapWorldRange > 0.0f)
	{
		const float MMX = ScreenW - MinimapSize.X - MinimapOffset.X;
		const float MMY = MinimapOffset.Y;
		const FVector2D MMOrigin(MMX, MMY);

		{
			FCanvasTileItem BorderTile(FVector2D(MMX - Border, MMY - Border),
			                           FVector2D(MinimapSize.X + Border * 2.0f, MinimapSize.Y + Border * 2.0f),
			                           MinimapBorderColor);
			BorderTile.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(BorderTile);
		}
		{
			FCanvasTileItem BG(FVector2D(MMX, MMY), MinimapSize, MinimapBackgroundColor);
			BG.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(BG);
		}

		const FVector PlayerLoc = SO->GetActorLocation();

		auto WorldToMinimap = [&](const FVector& WorldLoc) -> FVector2D
		{
			// The minimap is a top-down view — X world -> Y screen (up = -Y), Y world -> X screen.
			const float NormX = FMath::Clamp((WorldLoc.Y - PlayerLoc.Y) / MinimapWorldRange, -1.0f, 1.0f);
			const float NormY = FMath::Clamp((PlayerLoc.X - WorldLoc.X) / MinimapWorldRange, -1.0f, 1.0f);
			return FVector2D(MMX + MinimapSize.X * 0.5f + NormX * MinimapSize.X * 0.5f,
			                 MMY + MinimapSize.Y * 0.5f + NormY * MinimapSize.Y * 0.5f);
		};

		// Player pip (a slightly larger yellow square at the center).
		{
			const float HalfSize = MinimapDotSize + 1.0f;
			FCanvasTileItem PlayerPip(FVector2D(MMX + MinimapSize.X * 0.5f - HalfSize, MMY + MinimapSize.Y * 0.5f - HalfSize),
			                          FVector2D(HalfSize * 2.0f, HalfSize * 2.0f),
			                          MinimapPlayerColor);
			PlayerPip.BlendMode = SE_BLEND_Translucent;
			Canvas->DrawItem(PlayerPip);
		}

		// Aura ring — 16 translucent dots forming a circle around the player pip.
		if (SO->AuraComponent && SO->AuraComponent->bAuraActive)
		{
			const float AuraPxRadius =
				(SO->AuraComponent->AuraRadius / MinimapWorldRange) * MinimapSize.X * 0.5f;
			const float PipCX = MMX + MinimapSize.X * 0.5f;
			const float PipCY = MMY + MinimapSize.Y * 0.5f;
			const FLinearColor AuraDotColor(0.45f, 0.05f, 0.75f, 0.45f);
			for (int32 a = 0; a < 16; ++a)
			{
				const float Angle = (static_cast<float>(a) / 16.0f) * 2.0f * PI;
				const FVector2D DotPos(
					PipCX + FMath::Cos(Angle) * AuraPxRadius,
					PipCY + FMath::Sin(Angle) * AuraPxRadius);
				DrawMinimapDot(Canvas, MMOrigin, DotPos, AuraDotColor);
			}
		}

		// Enemies (red) — bosses tinted orange.
		{
			TArray<AActor*> Enemies;
			UGameplayStatics::GetAllActorsOfClass(this, ASOEnemyCharacter::StaticClass(), Enemies);
			for (AActor* A : Enemies)
			{
				ASOEnemyCharacter* Enemy = Cast<ASOEnemyCharacter>(A);
				if (!Enemy || !Enemy->IsAlive())
				{
					continue;
				}
				const FLinearColor Color = A->IsA(ASOBossCharacter::StaticClass()) ? MinimapBossColor : MinimapEnemyColor;
				DrawMinimapDot(Canvas, MMOrigin, WorldToMinimap(Enemy->GetActorLocation()), Color);
			}
		}

		// Vendors (blue).
		{
			TArray<AActor*> Vendors;
			UGameplayStatics::GetAllActorsOfClass(this, ASOVendorNPC::StaticClass(), Vendors);
			for (AActor* A : Vendors)
			{
				if (A)
				{
					DrawMinimapDot(Canvas, MMOrigin, WorldToMinimap(A->GetActorLocation()), MinimapVendorColor);
				}
			}
		}

		// Orbs (green) and item pickups (gold).
		{
			TArray<AActor*> Orbs;
			UGameplayStatics::GetAllActorsOfClass(this, ASOPickupOrb::StaticClass(), Orbs);
			for (AActor* A : Orbs)
			{
				if (A)
				{
					DrawMinimapDot(Canvas, MMOrigin, WorldToMinimap(A->GetActorLocation()), MinimapOrbColor);
				}
			}
			TArray<AActor*> Items;
			UGameplayStatics::GetAllActorsOfClass(this, ASOItemPickup::StaticClass(), Items);
			for (AActor* A : Items)
			{
				if (A)
				{
					DrawMinimapDot(Canvas, MMOrigin, WorldToMinimap(A->GetActorLocation()), MinimapItemColor);
				}
			}
		}
	}

	// -- Dialogue box (bottom-center when a conversation is active) ----------
	if (bShowDialogueBox && MediumFont && !UGameplayStatics::IsGamePaused(this))
	{
		TArray<AActor*> DialogueActors;
		UGameplayStatics::GetAllActorsOfClass(this, ASODialogueNPC::StaticClass(), DialogueActors);

		USODialogueComponent* ActiveDialogue = nullptr;
		for (AActor* A : DialogueActors)
		{
			if (ASODialogueNPC* NPC = Cast<ASODialogueNPC>(A))
			{
				if (NPC->DialogueComponent && NPC->DialogueComponent->IsInDialogue())
				{
					ActiveDialogue = NPC->DialogueComponent;
					break;
				}
			}
		}

		if (ActiveDialogue)
		{
			USODialogueNode* Node = ActiveDialogue->GetCurrentNode();
			if (Node)
			{
				const float PanelW   = ScreenW * DialoguePanelWidthFraction;
				const float PanelX   = (ScreenW - PanelW) * 0.5f;
				const float PanelY   = ScreenH - DialoguePanelHeight - DialoguePanelBottomOffset;
				const float BorderPx = 2.0f;
				const float Pad      = 16.0f;

				// Border
				{
					FCanvasTileItem BorderTile(FVector2D(PanelX - BorderPx, PanelY - BorderPx),
					                           FVector2D(PanelW + BorderPx * 2.0f, DialoguePanelHeight + BorderPx * 2.0f),
					                           DialoguePanelBorderColor);
					BorderTile.BlendMode = SE_BLEND_Translucent;
					Canvas->DrawItem(BorderTile);
				}
				// Background
				{
					FCanvasTileItem PanelBG(FVector2D(PanelX, PanelY),
					                        FVector2D(PanelW, DialoguePanelHeight),
					                        DialoguePanelColor);
					PanelBG.BlendMode = SE_BLEND_Translucent;
					Canvas->DrawItem(PanelBG);
				}

				float TextY = PanelY + Pad;

				// Speaker name (gold, slightly larger)
				if (!Node->SpeakerName.IsEmpty())
				{
					const float SpeakerScale = DialogueTextScale * 1.1f;
					FCanvasTextItem SpeakerItem(FVector2D(PanelX + Pad, TextY),
					                            Node->SpeakerName,
					                            MediumFont,
					                            DialogueSpeakerColor);
					SpeakerItem.Scale = FVector2D(SpeakerScale, SpeakerScale);
					SpeakerItem.EnableShadow(FLinearColor::Black);
					Canvas->DrawItem(SpeakerItem);
					float NW = 0.0f, NH = 0.0f;
					Canvas->TextSize(MediumFont, Node->SpeakerName.ToString(), NW, NH, SpeakerScale, SpeakerScale);
					TextY += NH + 8.0f;
				}

				// Body text
				if (!Node->BodyText.IsEmpty())
				{
					FCanvasTextItem BodyItem(FVector2D(PanelX + Pad, TextY),
					                         Node->BodyText,
					                         MediumFont,
					                         DialogueBodyColor);
					BodyItem.Scale = FVector2D(DialogueTextScale, DialogueTextScale);
					BodyItem.EnableShadow(FLinearColor::Black);
					Canvas->DrawItem(BodyItem);
					float BW = 0.0f, BH = 0.0f;
					Canvas->TextSize(MediumFont, Node->BodyText.ToString(), BW, BH, DialogueTextScale, DialogueTextScale);
					TextY += BH + 14.0f;
				}

				// Numbered choices or [F] Continue hint
				if (Node->Choices.Num() > 0)
				{
					for (int32 i = 0; i < Node->Choices.Num() && i < 4; ++i)
					{
						const FString ChoiceStr = FString::Printf(TEXT("[%d]  %s"),
						                                          i + 1,
						                                          *Node->Choices[i].ChoiceText.ToString());
						FCanvasTextItem ChoiceItem(FVector2D(PanelX + Pad + 8.0f, TextY),
						                           FText::FromString(ChoiceStr),
						                           MediumFont,
						                           DialogueChoiceColor);
						ChoiceItem.Scale = FVector2D(DialogueTextScale, DialogueTextScale);
						ChoiceItem.EnableShadow(FLinearColor::Black);
						Canvas->DrawItem(ChoiceItem);
						float CW = 0.0f, CH = 0.0f;
						Canvas->TextSize(MediumFont, ChoiceStr, CW, CH, DialogueTextScale, DialogueTextScale);
						TextY += CH + 4.0f;
					}
				}
				else
				{
					FCanvasTextItem ContinueItem(FVector2D(PanelX + Pad + 8.0f, TextY),
					                             FText::FromString(TEXT("[F]  Continue")),
					                             MediumFont,
					                             FLinearColor(0.70f, 0.70f, 0.70f, 0.90f));
					ContinueItem.Scale = FVector2D(DialogueTextScale * 0.9f, DialogueTextScale * 0.9f);
					ContinueItem.EnableShadow(FLinearColor::Black);
					Canvas->DrawItem(ContinueItem);
				}
			}
		}
	}

	// -- Waypoint map overlay --------------------------------------------------
	if (SO->WaypointComponent && SO->WaypointComponent->IsMapOpen() && MediumFont)
	{
		const TArray<ASOWaypoint*>& Waypoints = SO->WaypointComponent->GetDiscoveredWaypoints();

		const float PanelWidth  = 420.0f;
		const float RowHeight   = 30.0f;
		const float PanelHeight = 90.0f + FMath::Max(1, Waypoints.Num()) * RowHeight;
		const float PanelX      = (ScreenW - PanelWidth) * 0.5f;
		const float PanelY      = (ScreenH - PanelHeight) * 0.5f;

		FCanvasTileItem Panel(FVector2D(PanelX, PanelY), FVector2D(PanelWidth, PanelHeight),
		                      FLinearColor(0.03f, 0.03f, 0.07f, 0.92f));
		Panel.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(Panel);

		if (LargeFont)
		{
			FCanvasTextItem Title(FVector2D(PanelX + 24.0f, PanelY + 16.0f),
			                      FText::FromString(TEXT("WAYPOINTS")),
			                      LargeFont,
			                      FLinearColor(0.85f, 0.75f, 1.0f, 1.0f));
			Title.Scale = FVector2D(1.3f, 1.3f);
			Title.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(Title);
		}

		if (Waypoints.Num() == 0)
		{
			FCanvasTextItem Empty(FVector2D(PanelX + 24.0f, PanelY + 60.0f),
			                      FText::FromString(TEXT("No waypoints discovered yet.")),
			                      MediumFont,
			                      FLinearColor(0.7f, 0.7f, 0.7f, 1.0f));
			Empty.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(Empty);
		}
		else
		{
			for (int32 i = 0; i < Waypoints.Num() && i < 5; ++i)
			{
				const ASOWaypoint* WP = Waypoints[i];
				const FString Name = WP ? WP->WaypointName.ToString() : TEXT("Unknown");
				const FString Line = FString::Printf(TEXT("%d. %s"), i + 1, *Name);

				FCanvasTextItem RowItem(FVector2D(PanelX + 24.0f, PanelY + 60.0f + i * RowHeight),
				                        FText::FromString(Line),
				                        MediumFont,
				                        FLinearColor(0.92f, 0.92f, 0.92f, 1.0f));
				RowItem.EnableShadow(FLinearColor::Black);
				Canvas->DrawItem(RowItem);
			}
		}

		FCanvasTextItem CloseHint(FVector2D(PanelX + 24.0f, PanelY + PanelHeight - 26.0f),
		                          FText::FromString(TEXT("5-9 Travel   |   M Close")),
		                          MediumFont,
		                          FLinearColor(0.6f, 0.6f, 0.6f, 1.0f));
		CloseHint.Scale = FVector2D(0.85f, 0.85f);
		CloseHint.EnableShadow(FLinearColor::Black);
		Canvas->DrawItem(CloseHint);
	}

	// -- Bestiary / kill codex overlay ----------------------------------------
	if (SO->BestiaryComponent && SO->BestiaryComponent->IsCodexOpen() && MediumFont)
	{
		const TArray<FString> Entries = SO->BestiaryComponent->GetEntryDescriptions();
		const int32 ShownEntries = FMath::Min(Entries.Num(), 10);

		const float PanelWidth  = 420.0f;
		const float RowHeight   = 24.0f;
		const float PanelHeight = 100.0f + FMath::Max(1, ShownEntries) * RowHeight;
		const float PanelX      = (ScreenW - PanelWidth) * 0.5f;
		const float PanelY      = (ScreenH - PanelHeight) * 0.5f;

		FCanvasTileItem Panel(FVector2D(PanelX, PanelY), FVector2D(PanelWidth, PanelHeight),
		                      FLinearColor(0.05f, 0.03f, 0.03f, 0.92f));
		Panel.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(Panel);

		if (LargeFont)
		{
			const FString Title = FString::Printf(TEXT("BESTIARY  (%d kills, %d species)"),
				SO->BestiaryComponent->GetTotalKills(), SO->BestiaryComponent->GetDiscoveredSpeciesCount());
			FCanvasTextItem TitleItem(FVector2D(PanelX + 24.0f, PanelY + 16.0f),
			                          FText::FromString(Title),
			                          LargeFont,
			                          FLinearColor(0.90f, 0.55f, 0.25f, 1.0f));
			TitleItem.Scale = FVector2D(1.1f, 1.1f);
			TitleItem.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(TitleItem);
		}

		if (ShownEntries == 0)
		{
			FCanvasTextItem Empty(FVector2D(PanelX + 24.0f, PanelY + 60.0f),
			                      FText::FromString(TEXT("No kills recorded yet.")),
			                      MediumFont,
			                      FLinearColor(0.7f, 0.7f, 0.7f, 1.0f));
			Empty.EnableShadow(FLinearColor::Black);
			Canvas->DrawItem(Empty);
		}
		else
		{
			for (int32 i = 0; i < ShownEntries; ++i)
			{
				FCanvasTextItem RowItem(FVector2D(PanelX + 24.0f, PanelY + 60.0f + i * RowHeight),
				                        FText::FromString(Entries[i]),
				                        MediumFont,
				                        FLinearColor(0.92f, 0.92f, 0.92f, 1.0f));
				RowItem.EnableShadow(FLinearColor::Black);
				Canvas->DrawItem(RowItem);
			}
		}

		FCanvasTextItem CloseHint(FVector2D(PanelX + 24.0f, PanelY + PanelHeight - 26.0f),
		                          FText::FromString(TEXT("L Close")),
		                          MediumFont,
		                          FLinearColor(0.6f, 0.6f, 0.6f, 1.0f));
		CloseHint.Scale = FVector2D(0.85f, 0.85f);
		CloseHint.EnableShadow(FLinearColor::Black);
		Canvas->DrawItem(CloseHint);
	}

	// -- Achievement toast (top-center, brief) --------------------------------
	if (SO->AchievementComponent && SO->AchievementComponent->IsToastActive() && LargeFont)
	{
		const FString ToastText = FString::Printf(TEXT("ACHIEVEMENT UNLOCKED: %s"),
			*SO->AchievementComponent->GetToastText().ToString());

		float TW = 0.0f, TH = 0.0f;
		Canvas->TextSize(LargeFont, ToastText, TW, TH, 1.1f, 1.1f);

		FCanvasTextItem Toast(FVector2D((ScreenW - TW) * 0.5f, 70.0f),
		                      FText::FromString(ToastText),
		                      LargeFont,
		                      FLinearColor(1.0f, 0.85f, 0.35f, 1.0f));
		Toast.Scale = FVector2D(1.1f, 1.1f);
		Toast.EnableShadow(FLinearColor::Black);
		Canvas->DrawItem(Toast);
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

void ASOHUD::DrawMinimapDot(UCanvas* InCanvas,
                             const FVector2D& MinimapOriginPx,
                             const FVector2D& OriginPx,
                             const FLinearColor& Color) const
{
	if (!InCanvas)
	{
		return;
	}
	// Clip check against the minimap rect so out-of-range dots don't smear across the screen.
	if (OriginPx.X < MinimapOriginPx.X || OriginPx.X > MinimapOriginPx.X + MinimapSize.X ||
	    OriginPx.Y < MinimapOriginPx.Y || OriginPx.Y > MinimapOriginPx.Y + MinimapSize.Y)
	{
		return;
	}

	const float Half = MinimapDotSize;
	FCanvasTileItem Dot(FVector2D(OriginPx.X - Half, OriginPx.Y - Half),
	                    FVector2D(Half * 2.0f, Half * 2.0f),
	                    Color);
	Dot.BlendMode = SE_BLEND_Translucent;
	InCanvas->DrawItem(Dot);
}
