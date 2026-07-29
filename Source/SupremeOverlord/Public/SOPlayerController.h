#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SOPlayerController.generated.h"

class UMaterialInterface;

UCLASS()
class SUPREMEOVERLORD_API ASOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASOPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/** Called when the "MoveTo" input action is pressed. Ray-casts under the cursor and issues a move order. */
	void OnMoveToPressed();

	/** Called every frame while "MoveTo" is held so hold-to-move works Diablo-style. */
	void OnMoveToHeld();

	/** Called when the "MoveTo" input action is released. */
	void OnMoveToReleased();

	/** Issues the actual navigation move to the given world location and spawns the click FX. */
	void MovePawnToLocation(const FVector& WorldLocation);

	/** Spawns the ground decal at the given location. */
	void SpawnClickIndicator(const FVector& WorldLocation);

public:
	/** Material used for the ground decal that appears when the player issues a move order. Assign an M_ClickDecal in the editor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Feedback")
	TObjectPtr<UMaterialInterface> ClickDecalMaterial;

	/** Optional particle system spawned at the clicked location. Assign in editor if desired. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Feedback")
	TObjectPtr<class UParticleSystem> ClickParticleFX;

	/** World-space size of the click decal (X/Y is the ground footprint, Z is the projection depth). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Feedback")
	FVector ClickDecalSize = FVector(24.0f, 96.0f, 96.0f);

	/** How long the click decal stays before fading out. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Feedback", meta = (ClampMin = "0.1", UIMin = "0.1", UIMax = "5.0"))
	float ClickDecalLifetime = 0.6f;

	/** Trace channel used when converting the mouse cursor into a world position. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Input")
	TEnumAsByte<ECollisionChannel> ClickTraceChannel = ECC_Visibility;

	/** Called from Blueprint to trigger a move-to at an explicit location (handy for UI-driven commands). */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Movement")
	void CommandMoveTo(FVector WorldLocation);

	/** How much damage the debug key applies to the possessed pawn per press. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Debug", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1000.0"))
	float DebugDamageAmount = 10.0f;

	/** Fires DebugDamageAmount at the possessed pawn. Bound to the "DebugDamageSelf" action. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Debug")
	void DebugDamageSelf();

	/** Bound to the "PrimaryAttack" input action. Resolves the cursor into world space and asks the pawn to swing. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Combat")
	void OnPrimaryAttackPressed();

	/** Bound to the "ShadowBolt" input action. Resolves the cursor and asks the pawn to cast. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Combat")
	void OnShadowBoltPressed();

	/** Bound to the "LifeDrain" input action. AoE around the caster with self-heal. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Combat")
	void OnLifeDrainPressed();

	/** Bound to the "TogglePause" and "QuitGame" actions. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|UI")
	void OnTogglePausePressed();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|UI")
	void OnQuitPressed();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Attributes")
	void OnAllocateStrengthPressed();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Attributes")
	void OnAllocateIntellectPressed();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Attributes")
	void OnAllocateVitalityPressed();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Save")
	void OnQuickSavePressed();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Save")
	void OnQuickLoadPressed();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Interact")
	void OnInteractPressed();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Interact")
	void OnSellPressed();

	/** Resolves the cursor to a world location and asks the character to summon a minion there. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Summoning")
	void OnSummonMinionPressed();

	/** Destroys all active minions. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Summoning")
	void OnDismissMinionsPressed();

	/** Bound to "PlaceTrap" (C). Resolves the cursor and places the selected trap type there. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Traps")
	void OnPlaceTrapPressed();

	/** Bound to "CycleTrap" (V). Advances the selected trap type to the next variant. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Traps")
	void OnCycleTrapPressed();

	/** Bound to "ActivateOverlordMode" (Z). Triggers Overlord Mode when the corruption meter is full. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Corruption")
	void OnActivateOverlordModePressed();

	/** Bound to "NecroticResurrect" (U). Raises the nearest dead enemy at the cursor position as a temporary minion. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Summoning")
	void OnNecroticResurrectPressed();

	/** Bound to "CorpseExplosion" (X). Resolves the cursor and detonates the nearest corpse there. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|CorpseExplosion")
	void OnCorpseExplosionPressed();

	/** Bound to "Blink" (B). Resolves the cursor and teleports the pawn toward it. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Blink")
	void OnBlinkPressed();

	/** Bound to "CursedGround" (H). Resolves the cursor and places a hazard zone there. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|CursedGround")
	void OnCursedGroundPressed();

	/** Bound to "DodgeRoll" (Space). Rolls in the current movement direction, or toward the cursor if standing still. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|DodgeRoll")
	void OnDodgeRollPressed();

	/** Bound to "ToggleWaypointMap" (M). Opens/closes the waypoint fast-travel overlay. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Waypoint")
	void OnToggleWaypointMapPressed();

	/** Bound to "WaypointSelect1".."WaypointSelect5" (Five-Nine). Travels to the Nth discovered waypoint while the map is open. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Waypoint")
	void OnWaypointSelect1();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Waypoint")
	void OnWaypointSelect2();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Waypoint")
	void OnWaypointSelect3();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Waypoint")
	void OnWaypointSelect4();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Waypoint")
	void OnWaypointSelect5();

	/** Bound to "RespecTalents" (P). Refunds all unlocked talent nodes for a gold fee. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Talents")
	void OnRespecTalentsPressed();

	/** Bound to "CycleDifficulty" (O). Advances the global difficulty tier. Not a character ability — applies to the whole run. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Difficulty")
	void OnCycleDifficultyPressed();

	/** Bound to "ToggleBestiary" (L). Opens/closes the kill codex overlay. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Bestiary")
	void OnToggleBestiaryPressed();

	/** Bound to "UsePotion" (I). Consumes one potion charge for a heal/mana burst. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Consumable")
	void OnUsePotionPressed();

	/** Bound to "RealitySlash" (J). Instant-kills (or heavily damages) the nearest enemy to the cursor. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|RealitySlash")
	void OnRealitySlashPressed();

	/** Dialogue choice key 1 — selects option index 0 in the active dialogue. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Dialogue")
	void OnDialogueChoice1();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Dialogue")
	void OnDialogueChoice2();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Dialogue")
	void OnDialogueChoice3();

	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Dialogue")
	void OnDialogueChoice4();

private:
	/** Shared guard for MoveTo/Repeat: returns true only if the pawn exists and reports alive. */
	bool CanIssueMoveOrders() const;

	/** Shared implementation for OnWaypointSelect1..5 — travels to discovered waypoint Index (0-based). */
	void SelectWaypoint(int32 Index);

	bool bMoveToHeld = false;
};
