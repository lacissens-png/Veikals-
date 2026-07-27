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

private:
	/** Shared guard for MoveTo/Repeat: returns true only if the pawn exists and reports alive. */
	bool CanIssueMoveOrders() const;

	bool bMoveToHeld = false;
};
