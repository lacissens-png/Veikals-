#include "SOPlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Particles/ParticleSystem.h"

ASOPlayerController::ASOPlayerController()
{
	bShowMouseCursor           = true;
	bEnableClickEvents         = true;
	bEnableMouseOverEvents     = true;
	DefaultMouseCursor         = EMouseCursor::Crosshairs;
}

void ASOPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set up an input mode that keeps the cursor available at all times - required for point-and-click.
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void ASOPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent)
	{
		return;
	}

	// Bind to the "MoveTo" action mapping (defined in DefaultInput.ini / Project Settings > Input).
	InputComponent->BindAction("MoveTo", IE_Pressed,  this, &ASOPlayerController::OnMoveToPressed);
	InputComponent->BindAction("MoveTo", IE_Released, this, [this]() { bMoveToHeld = false; });
	InputComponent->BindAction("MoveTo", IE_Repeat,   this, &ASOPlayerController::OnMoveToHeld);
}

void ASOPlayerController::OnMoveToPressed()
{
	bMoveToHeld = true;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		MovePawnToLocation(Hit.ImpactPoint);
	}
}

void ASOPlayerController::OnMoveToHeld()
{
	if (!bMoveToHeld)
	{
		return;
	}

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		// While held we re-issue the move without re-spawning the decal so we don't flood the world.
		if (APawn* ControlledPawn = GetPawn())
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.ImpactPoint);
			(void)ControlledPawn;
		}
	}
}

void ASOPlayerController::CommandMoveTo(FVector WorldLocation)
{
	MovePawnToLocation(WorldLocation);
}

void ASOPlayerController::MovePawnToLocation(const FVector& WorldLocation)
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys)
	{
		// No nav system present - we still spawn the decal so the designer notices the missing NavMesh.
		SpawnClickIndicator(WorldLocation);
		return;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, WorldLocation);
	SpawnClickIndicator(WorldLocation);
}

void ASOPlayerController::SpawnClickIndicator(const FVector& WorldLocation)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (ClickDecalMaterial)
	{
		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
			World,
			ClickDecalMaterial,
			ClickDecalSize,
			WorldLocation,
			FRotator(-90.0f, 0.0f, 0.0f),
			ClickDecalLifetime);
		if (Decal)
		{
			Decal->SetFadeScreenSize(0.0f);
		}
	}

	if (ClickParticleFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(World, ClickParticleFX, WorldLocation, FRotator::ZeroRotator, true);
	}
}
