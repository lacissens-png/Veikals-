#include "SOPlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NavigationSystem.h"
#include "Particles/ParticleSystem.h"
#include "SOAttributesComponent.h"
#include "SOCharacter.h"
#include "SODamageType.h"
#include "SOHealthComponent.h"
#include "SOVendorNPC.h"

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

	// Debug helper - defaults to the K key, remap under Project Settings > Input.
	InputComponent->BindAction("DebugDamageSelf", IE_Pressed, this, &ASOPlayerController::DebugDamageSelf);

	// Primary attack - RMB or Q by default (see DefaultInput.ini).
	InputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASOPlayerController::OnPrimaryAttackPressed);

	// Shadow Bolt - E by default (see DefaultInput.ini).
	InputComponent->BindAction("ShadowBolt",   IE_Pressed, this, &ASOPlayerController::OnShadowBoltPressed);

	// Life Drain - R by default.
	InputComponent->BindAction("LifeDrain",    IE_Pressed, this, &ASOPlayerController::OnLifeDrainPressed);

	// Pause / quit - ESC + F10. Executed while paused too.
	FInputActionBinding& PauseBinding = InputComponent->BindAction("TogglePause", IE_Pressed, this, &ASOPlayerController::OnTogglePausePressed);
	PauseBinding.bExecuteWhenPaused = true;

	FInputActionBinding& QuitBinding  = InputComponent->BindAction("QuitGame",    IE_Pressed, this, &ASOPlayerController::OnQuitPressed);
	QuitBinding.bExecuteWhenPaused = true;

	// Attribute allocation - keyboard shortcuts.
	InputComponent->BindAction("AllocateStrength",  IE_Pressed, this, &ASOPlayerController::OnAllocateStrengthPressed);
	InputComponent->BindAction("AllocateIntellect", IE_Pressed, this, &ASOPlayerController::OnAllocateIntellectPressed);
	InputComponent->BindAction("AllocateVitality",  IE_Pressed, this, &ASOPlayerController::OnAllocateVitalityPressed);

	// Save/Load - F5 quicksave, F9 quickload. Allowed while paused.
	FInputActionBinding& SaveBind = InputComponent->BindAction("QuickSave", IE_Pressed, this, &ASOPlayerController::OnQuickSavePressed);
	SaveBind.bExecuteWhenPaused = true;
	FInputActionBinding& LoadBind = InputComponent->BindAction("QuickLoad", IE_Pressed, this, &ASOPlayerController::OnQuickLoadPressed);
	LoadBind.bExecuteWhenPaused = true;

	// Interact - vendor buy (F) / sell (G).
	InputComponent->BindAction("Interact", IE_Pressed, this, &ASOPlayerController::OnInteractPressed);
	InputComponent->BindAction("Sell",     IE_Pressed, this, &ASOPlayerController::OnSellPressed);
}

bool ASOPlayerController::CanIssueMoveOrders() const
{
	const APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return false;
	}

	if (const ASOCharacter* SOCharacter = Cast<ASOCharacter>(ControlledPawn))
	{
		return SOCharacter->IsAlive();
	}
	return true;
}

void ASOPlayerController::OnMoveToPressed()
{
	if (!CanIssueMoveOrders())
	{
		return;
	}

	bMoveToHeld = true;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		MovePawnToLocation(Hit.ImpactPoint);
	}
}

void ASOPlayerController::OnMoveToHeld()
{
	if (!bMoveToHeld || !CanIssueMoveOrders())
	{
		return;
	}

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		// While held we re-issue the move without re-spawning the decal so we don't flood the world.
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.ImpactPoint);
	}
}

void ASOPlayerController::CommandMoveTo(FVector WorldLocation)
{
	if (!CanIssueMoveOrders())
	{
		return;
	}
	MovePawnToLocation(WorldLocation);
}

void ASOPlayerController::DebugDamageSelf()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn || DebugDamageAmount <= 0.0f)
	{
		return;
	}

	UGameplayStatics::ApplyDamage(
		ControlledPawn,
		DebugDamageAmount,
		this,
		ControlledPawn,
		USODamageType::StaticClass());
}

void ASOPlayerController::OnPrimaryAttackPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (!SOCharacter || !SOCharacter->IsAlive())
	{
		return;
	}

	// Resolve the cursor to a world location. If the cursor is over nothing
	// (edge of screen, no ground) we fall back to the character's forward
	// vector so the swing still fires in a sensible direction.
	FVector AttackTarget = SOCharacter->GetActorLocation() + SOCharacter->GetActorForwardVector() * SOCharacter->PrimaryAttackRange;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		AttackTarget = Hit.ImpactPoint;
	}

	SOCharacter->PerformPrimaryAttack(AttackTarget);
}

void ASOPlayerController::OnShadowBoltPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (!SOCharacter || !SOCharacter->IsAlive())
	{
		return;
	}

	FVector AimTarget = SOCharacter->GetActorLocation() + SOCharacter->GetActorForwardVector() * 500.0f;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		AimTarget = Hit.ImpactPoint;
	}

	SOCharacter->CastShadowBolt(AimTarget);
}

void ASOPlayerController::OnLifeDrainPressed()
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn()))
	{
		SOCharacter->CastLifeDrain();
	}
}

void ASOPlayerController::OnTogglePausePressed()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const bool bNowPaused = !UGameplayStatics::IsGamePaused(this);
	UGameplayStatics::SetGamePaused(this, bNowPaused);
}

void ASOPlayerController::OnQuitPressed()
{
	// Only accept quit while paused so a stray F10 during combat doesn't kill the session.
	if (!UGameplayStatics::IsGamePaused(this))
	{
		return;
	}
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, /*bIgnorePlatformRestrictions*/ false);
}

static void SOAllocate(APlayerController* PC, ESOAttribute Attribute)
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(PC ? PC->GetPawn() : nullptr))
	{
		if (USOAttributesComponent* Attrs = SOCharacter->AttributesComponent)
		{
			Attrs->AllocatePoint(Attribute);
		}
	}
}

void ASOPlayerController::OnAllocateStrengthPressed()  { SOAllocate(this, ESOAttribute::Strength);  }
void ASOPlayerController::OnAllocateIntellectPressed() { SOAllocate(this, ESOAttribute::Intellect); }
void ASOPlayerController::OnAllocateVitalityPressed()  { SOAllocate(this, ESOAttribute::Vitality);  }

void ASOPlayerController::OnQuickSavePressed()
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn()))
	{
		SOCharacter->QuickSave();
	}
}

void ASOPlayerController::OnQuickLoadPressed()
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn()))
	{
		SOCharacter->QuickLoad();
	}
}

static ASOVendorNPC* FindNearbyVendor(ASOCharacter* Player)
{
	if (!Player)
	{
		return nullptr;
	}
	TArray<AActor*> Vendors;
	UGameplayStatics::GetAllActorsOfClass(Player, ASOVendorNPC::StaticClass(), Vendors);

	ASOVendorNPC* Closest = nullptr;
	float ClosestDistSq = TNumericLimits<float>::Max();
	const FVector PlayerLoc = Player->GetActorLocation();
	for (AActor* A : Vendors)
	{
		ASOVendorNPC* Vendor = Cast<ASOVendorNPC>(A);
		if (!Vendor || !Vendor->IsPlayerInRange(Player))
		{
			continue;
		}
		const float DistSq = FVector::DistSquared(PlayerLoc, Vendor->GetActorLocation());
		if (DistSq < ClosestDistSq)
		{
			ClosestDistSq = DistSq;
			Closest       = Vendor;
		}
	}
	return Closest;
}

void ASOPlayerController::OnInteractPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (ASOVendorNPC* Vendor = FindNearbyVendor(SOCharacter))
	{
		Vendor->TryBuyNext(SOCharacter);
	}
}

void ASOPlayerController::OnSellPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (ASOVendorNPC* Vendor = FindNearbyVendor(SOCharacter))
	{
		Vendor->TrySellCurrentWeapon(SOCharacter);
	}
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
