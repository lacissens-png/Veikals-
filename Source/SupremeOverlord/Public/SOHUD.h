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

	/** Show a row of skill tiles above the XP bar. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Skills")
	bool bShowSkillPanel = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Skills")
	FVector2D SkillTileSize = FVector2D(64.0f, 64.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Skills", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "200.0"))
	float SkillTileGap = 12.0f;

	/** How high above the XP bar the skill row is drawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Skills", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "200.0"))
	float SkillPanelGap = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Skills")
	FLinearColor SkillTileBorder = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Skills")
	FLinearColor SkillCooldownOverlay = FLinearColor(0.0f, 0.0f, 0.0f, 0.65f);

	/** Show the XP bar and level number at the bottom-center of the screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|XP")
	bool bShowXPBar = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|XP")
	FVector2D XPBarSize = FVector2D(520.0f, 10.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|XP", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "200.0"))
	float XPBarBottomOffset = 24.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|XP")
	FLinearColor XPBarBackgroundColor = FLinearColor(0.02f, 0.02f, 0.02f, 0.85f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|XP")
	FLinearColor XPBarFillColor = FLinearColor(0.95f, 0.80f, 0.20f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|XP")
	FLinearColor XPTextColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|XP", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "3.0"))
	float XPLabelScale = 1.1f;

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
	FString ControlHintText = TEXT("LMB Move   |   RMB/Q Strike   |   E Shadow   |   R Drain   |   T Summon   |   Y Dismiss   |   C Trap   |   V Cycle   |   Z Overlord   |   U Resurrect   |   X Corpse   |   B Blink   |   H Curse   |   Space Roll   |   F1/2/3 Attrs   |   ESC Pause");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Attributes")
	bool bShowAttributesPanel = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Attributes")
	FVector2D AttributesPanelOffset = FVector2D(32.0f, 44.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Attributes", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "3.0"))
	float AttributesTextScale = 1.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Attributes")
	FLinearColor AttributesTextColor = FLinearColor(0.92f, 0.92f, 0.92f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Attributes")
	FLinearColor UnspentPointsColor = FLinearColor(1.0f, 0.85f, 0.25f, 1.0f);

	/** Show a Wave X / Enemies Y counter across the top when a spawner is active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Waves")
	bool bShowWaveCounter = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Waves", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "200.0"))
	float WaveCounterTopOffset = 44.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Waves", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "4.0"))
	float WaveCounterScale = 1.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Waves")
	FLinearColor WaveCounterColor = FLinearColor(0.95f, 0.75f, 0.35f, 1.0f);

	/** Show a nameplate + HP bar for the closest live boss on screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Boss")
	bool bShowBossBar = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Boss")
	FVector2D BossBarSize = FVector2D(600.0f, 22.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Boss", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "300.0"))
	float BossBarTopOffset = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Boss")
	FLinearColor BossBarBackgroundColor = FLinearColor(0.08f, 0.0f, 0.0f, 0.9f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Boss")
	FLinearColor BossBarFillColor = FLinearColor(0.68f, 0.05f, 0.15f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Boss")
	FLinearColor BossNameColor = FLinearColor(0.98f, 0.9f, 0.6f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Boss", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "4.0"))
	float BossNameScale = 1.4f;

	/** Show a "[F] Buy   [G] Sell" prompt when the player is inside a vendor's interaction sphere. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Vendor")
	bool bShowVendorPrompt = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Vendor", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "3.0"))
	float VendorPromptScale = 1.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Vendor")
	FLinearColor VendorPromptColor = FLinearColor(0.95f, 0.9f, 0.55f, 1.0f);

	/** Show a top-right radar minimap tracking enemies, orbs, and vendors around the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	bool bShowMinimap = true;

	/** Pixel size of the square minimap rendered in the top-right. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	FVector2D MinimapSize = FVector2D(180.0f, 180.0f);

	/** Offset from the top-right corner (X = right margin, Y = top margin). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	FVector2D MinimapOffset = FVector2D(24.0f, 60.0f);

	/** How many cm of world one edge of the minimap covers. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap", meta = (ClampMin = "500.0", UIMin = "500.0", UIMax = "10000.0"))
	float MinimapWorldRange = 3500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	FLinearColor MinimapBackgroundColor = FLinearColor(0.03f, 0.03f, 0.05f, 0.65f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	FLinearColor MinimapBorderColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.95f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	FLinearColor MinimapPlayerColor = FLinearColor(0.95f, 0.95f, 0.35f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	FLinearColor MinimapEnemyColor = FLinearColor(0.85f, 0.05f, 0.05f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	FLinearColor MinimapBossColor = FLinearColor(1.0f, 0.35f, 0.10f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	FLinearColor MinimapVendorColor = FLinearColor(0.35f, 0.75f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	FLinearColor MinimapOrbColor = FLinearColor(0.35f, 1.0f, 0.6f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap")
	FLinearColor MinimapItemColor = FLinearColor(1.0f, 0.85f, 0.3f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Minimap", meta = (ClampMin = "2.0", UIMin = "2.0", UIMax = "20.0"))
	float MinimapDotSize = 5.0f;

	// -----------------------------------------------------------------------
	// Corruption bar (sits between skill panel and XP bar)
	// -----------------------------------------------------------------------

	/** Show a thin corruption meter centered at the bottom above the XP bar. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Corruption")
	bool bShowCorruptionBar = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Corruption")
	FVector2D CorruptionBarSize = FVector2D(520.0f, 8.0f);

	/** Gap (pixels) between the top of the XP bar and the bottom of the corruption bar. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Corruption", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "40.0"))
	float CorruptionBarGap = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Corruption")
	FLinearColor CorruptionBarBackgroundColor = FLinearColor(0.04f, 0.0f, 0.06f, 0.85f);

	/** Bar fill color when corruption is below maximum. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Corruption")
	FLinearColor CorruptionBarFillColor = FLinearColor(0.45f, 0.05f, 0.75f, 1.0f);

	/** Bar fill color when the meter is full (Overlord Mode available). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Corruption")
	FLinearColor CorruptionBarFullColor = FLinearColor(0.80f, 0.25f, 1.00f, 1.0f);

	/** Text scale for the "OVERLORD MODE!" flash drawn while mode is active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Corruption", meta = (ClampMin = "1.0", UIMin = "1.0", UIMax = "6.0"))
	float OverlordModeFlashScale = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Corruption")
	FLinearColor OverlordModeFlashColor = FLinearColor(0.95f, 0.60f, 1.00f, 1.0f);

	// -----------------------------------------------------------------------
	// Trap indicator
	// -----------------------------------------------------------------------

	/** Show the currently selected trap type label below the PlaceTrap skill tile. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Traps")
	bool bShowTrapIndicator = true;

	/** Color of the trap-type name label. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Traps")
	FLinearColor TrapIndicatorColor = FLinearColor(1.0f, 0.80f, 0.20f, 1.0f);

	/** Show "YOU DIED" overlay when the player's health component reports dead. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Death")
	bool bShowDeathOverlay = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Death")
	FString DeathOverlayText = TEXT("YOU DIED");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Death")
	FLinearColor DeathOverlayColor = FLinearColor(0.85f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Death", meta = (ClampMin = "1.0", UIMin = "1.0", UIMax = "8.0"))
	float DeathOverlayScale = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Pause")
	bool bShowPauseOverlay = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Pause")
	FLinearColor PauseDimColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.55f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Pause")
	FString PauseTitleText = TEXT("PAUSED");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Pause")
	FString PauseHintText = TEXT("ESC  Resume     |     F10  Quit");

	/** Show the active quest tracker on the left side of the screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Quests")
	bool bShowQuestTracker = true;

	/** How many quests to display at once (oldest accepted first). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Quests", meta = (ClampMin = "1", UIMin = "1", UIMax = "5"))
	int32 MaxQuestsInTracker = 2;

	/** Pixels from the top of the screen where the quest panel begins. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Quests", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float QuestTrackerTopOffset = 170.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Quests", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "3.0"))
	float QuestTrackerScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Quests")
	FLinearColor QuestTitleColor = FLinearColor(0.98f, 0.85f, 0.35f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Quests")
	FLinearColor QuestObjectiveColor = FLinearColor(0.92f, 0.92f, 0.92f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Quests")
	FLinearColor QuestObjectiveCompleteColor = FLinearColor(0.4f, 1.0f, 0.4f, 1.0f);

	// -----------------------------------------------------------------------
	// Status effect icons (row above the HP bar)
	// -----------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|StatusEffects")
	bool bShowStatusEffects = true;

	/** Pixel size of each status effect icon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|StatusEffects")
	FVector2D StatusIconSize = FVector2D(26.0f, 26.0f);

	/** Gap between icons. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|StatusEffects", meta = (ClampMin = "0.0"))
	float StatusIconGap = 4.0f;

	// -----------------------------------------------------------------------
	// Dialogue box (bottom center when a conversation is active)
	// -----------------------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Dialogue")
	bool bShowDialogueBox = true;

	/** Width of the dialogue panel as a fraction of screen width. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Dialogue", meta = (ClampMin = "0.2", ClampMax = "1.0"))
	float DialoguePanelWidthFraction = 0.7f;

	/** Height of the dialogue panel in pixels. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Dialogue", meta = (ClampMin = "50.0"))
	float DialoguePanelHeight = 220.0f;

	/** Gap from the bottom edge of the screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Dialogue", meta = (ClampMin = "0.0"))
	float DialoguePanelBottomOffset = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Dialogue")
	FLinearColor DialoguePanelColor = FLinearColor(0.04f, 0.04f, 0.08f, 0.90f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Dialogue")
	FLinearColor DialoguePanelBorderColor = FLinearColor(0.25f, 0.18f, 0.45f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Dialogue")
	FLinearColor DialogueSpeakerColor = FLinearColor(0.98f, 0.85f, 0.35f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Dialogue")
	FLinearColor DialogueBodyColor = FLinearColor(0.92f, 0.92f, 0.92f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Dialogue")
	FLinearColor DialogueChoiceColor = FLinearColor(0.75f, 0.85f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|HUD|Dialogue", meta = (ClampMin = "0.5", UIMin = "0.5", UIMax = "3.0"))
	float DialogueTextScale = 1.1f;

private:
	/** Renders one skill tile with cooldown overlay + labels. */
	void DrawSkillTile(class UCanvas* InCanvas,
	                   const FVector2D& Origin,
	                   const FString& KeyLabel,
	                   const FString& SkillName,
	                   float ManaCost,
	                   float CurrentMana,
	                   float CooldownRemaining,
	                   float CooldownTotal,
	                   const FLinearColor& TileColor,
	                   class UFont* SmallFont);

	/** Renders one minimap dot at OriginPx (already screen-space), clipped to the map rect. */
	void DrawMinimapDot(class UCanvas* InCanvas,
	                    const FVector2D& MinimapOriginPx,
	                    const FVector2D& OriginPx,
	                    const FLinearColor& Color) const;
};
