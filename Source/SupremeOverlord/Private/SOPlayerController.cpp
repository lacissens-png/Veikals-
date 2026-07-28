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
#include "SODifficultySubsystem.h"
#include "SOHealthComponent.h"
#include "SODialogueComponent.h"
#include "SODialogueNPC.h"
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

	// Interact - dialogue (F) / vendor buy (F) / sell (G).
	InputComponent->BindAction("Interact", IE_Pressed, this, &ASOPlayerController::OnInteractPressed);
	InputComponent->BindAction("Sell",     IE_Pressed, this, &ASOPlayerController::OnSellPressed);

	// Summon minion (T) / dismiss all (Y).
	InputComponent->BindAction("SummonMinion",   IE_Pressed, this, &ASOPlayerController::OnSummonMinionPressed);
	InputComponent->BindAction("DismissMinions", IE_Pressed, this, &ASOPlayerController::OnDismissMinionsPressed);

	// Trap (C) / cycle type (V) / Overlord Mode (Z) / Necromantic Resurrect (U).
	InputComponent->BindAction("PlaceTrap",            IE_Pressed, this, &ASOPlayerController::OnPlaceTrapPressed);
	InputComponent->BindAction("CycleTrap",            IE_Pressed, this, &ASOPlayerController::OnCycleTrapPressed);
	InputComponent->BindAction("ActivateOverlordMode", IE_Pressed, this, &ASOPlayerController::OnActivateOverlordModePressed);
	InputComponent->BindAction("NecroticResurrect",    IE_Pressed, this, &ASOPlayerController::OnNecroticResurrectPressed);

	// Corpse Explosion (X) / Blink (B) / Cursed Ground (H) / Dodge Roll (Space).
	InputComponent->BindAction("CorpseExplosion", IE_Pressed, this, &ASOPlayerController::OnCorpseExplosionPressed);
	InputComponent->BindAction("Blink",           IE_Pressed, this, &ASOPlayerController::OnBlinkPressed);
	InputComponent->BindAction("CursedGround",    IE_Pressed, this, &ASOPlayerController::OnCursedGroundPressed);
	InputComponent->BindAction("DodgeRoll",       IE_Pressed, this, &ASOPlayerController::OnDodgeRollPressed);

	// Waypoint map (M) / travel to discovered waypoint 1-5 (Five..Nine).
	InputComponent->BindAction("ToggleWaypointMap", IE_Pressed, this, &ASOPlayerController::OnToggleWaypointMapPressed);
	InputComponent->BindAction("WaypointSelect1",   IE_Pressed, this, &ASOPlayerController::OnWaypointSelect1);
	InputComponent->BindAction("WaypointSelect2",   IE_Pressed, this, &ASOPlayerController::OnWaypointSelect2);
	InputComponent->BindAction("WaypointSelect3",   IE_Pressed, this, &ASOPlayerController::OnWaypointSelect3);
	InputComponent->BindAction("WaypointSelect4",   IE_Pressed, this, &ASOPlayerController::OnWaypointSelect4);
	InputComponent->BindAction("WaypointSelect5",   IE_Pressed, this, &ASOPlayerController::OnWaypointSelect5);

	// Respec (P) / cycle difficulty (O) / bestiary (L).
	InputComponent->BindAction("RespecTalents",   IE_Pressed, this, &ASOPlayerController::OnRespecTalentsPressed);
	InputComponent->BindAction("CycleDifficulty", IE_Pressed, this, &ASOPlayerController::OnCycleDifficultyPressed);
	InputComponent->BindAction("ToggleBestiary",  IE_Pressed, this, &ASOPlayerController::OnToggleBestiaryPressed);

	// Dialogue choice keys 1-4.
	InputComponent->BindAction("DialogueChoice1", IE_Pressed, this, &ASOPlayerController::OnDialogueChoice1);
	InputComponent->BindAction("DialogueChoice2", IE_Pressed, this, &ASOPlayerController::OnDialogueChoice2);
	InputComponent->BindAction("DialogueChoice3", IE_Pressed, this, &ASOPlayerController::OnDialogueChoice3);
	InputComponent->BindAction("DialogueChoice4", IE_Pressed, this, &ASOPlayerController::OnDialogueChoice4);
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

static USODialogueComponent* FindNearbyDialogue(ASOCharacter* Player)
{
	if (!Player)
	{
		return nullptr;
	}
	TArray<AActor*> NPCs;
	UGameplayStatics::GetAllActorsOfClass(Player, ASODialogueNPC::StaticClass(), NPCs);

	USODialogueComponent* Closest = nullptr;
	float ClosestDistSq = TNumericLimits<float>::Max();
	const FVector PlayerLoc = Player->GetActorLocation();

	for (AActor* A : NPCs)
	{
		ASODialogueNPC* NPC = Cast<ASODialogueNPC>(A);
		if (!NPC || !NPC->DialogueComponent || !NPC->DialogueComponent->EntryNode)
		{
			continue;
		}
		if (!NPC->DialogueComponent->IsPlayerInRange(Player))
		{
			continue;
		}
		const float DistSq = FVector::DistSquared(PlayerLoc, NPC->GetActorLocation());
		if (DistSq < ClosestDistSq)
		{
			ClosestDistSq = DistSq;
			Closest       = NPC->DialogueComponent;
		}
	}
	return Closest;
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

	// Dialogue takes priority over vendor interaction.
	if (USODialogueComponent* Dialogue = FindNearbyDialogue(SOCharacter))
	{
		if (Dialogue->IsInDialogue())
		{
			// F advances a no-choice node or closes if there are choices (player must use 1-4).
			if (Dialogue->GetCurrentNode() && Dialogue->GetCurrentNode()->Choices.Num() == 0)
			{
				Dialogue->SelectChoice(-1);
			}
		}
		else
		{
			Dialogue->StartDialogue(SOCharacter);
		}
		return;
	}

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

static void DispatchDialogueChoice(APlayerController* PC, int32 Index)
{
	ASOCharacter* SOCharacter = PC ? Cast<ASOCharacter>(PC->GetPawn()) : nullptr;
	if (USODialogueComponent* Dialogue = FindNearbyDialogue(SOCharacter))
	{
		if (Dialogue->IsInDialogue())
		{
			Dialogue->SelectChoice(Index);
		}
	}
}

void ASOPlayerController::OnDialogueChoice1() { DispatchDialogueChoice(this, 0); }
void ASOPlayerController::OnDialogueChoice2() { DispatchDialogueChoice(this, 1); }
void ASOPlayerController::OnDialogueChoice3() { DispatchDialogueChoice(this, 2); }
void ASOPlayerController::OnDialogueChoice4() { DispatchDialogueChoice(this, 3); }

void ASOPlayerController::OnSummonMinionPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (!SOCharacter || !SOCharacter->IsAlive())
	{
		return;
	}

	FVector SummonTarget = SOCharacter->GetActorLocation() + SOCharacter->GetActorForwardVector() * 200.0f;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		SummonTarget = Hit.ImpactPoint;
	}

	SOCharacter->CastSummonMinion(SummonTarget);
}

void ASOPlayerController::OnDismissMinionsPressed()
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn()))
	{
		SOCharacter->DismissAllMinions();
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

void ASOPlayerController::OnPlaceTrapPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (!SOCharacter || !SOCharacter->IsAlive())
	{
		return;
	}

	FVector TrapTarget = SOCharacter->GetActorLocation() + SOCharacter->GetActorForwardVector() * 200.0f;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		TrapTarget = Hit.ImpactPoint;
	}

	SOCharacter->PlaceTrap(TrapTarget);
}

void ASOPlayerController::OnCycleTrapPressed()
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn()))
	{
		SOCharacter->CycleTrap();
	}
}

void ASOPlayerController::OnActivateOverlordModePressed()
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn()))
	{
		SOCharacter->ActivateOverlordMode();
	}
}

void ASOPlayerController::OnNecroticResurrectPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (!SOCharacter || !SOCharacter->IsAlive())
	{
		return;
	}

	FVector Target = SOCharacter->GetActorLocation() + SOCharacter->GetActorForwardVector() * 200.0f;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		Target = Hit.ImpactPoint;
	}

	SOCharacter->CastNecroticResurrect(Target);
}

void ASOPlayerController::OnCorpseExplosionPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (!SOCharacter || !SOCharacter->IsAlive())
	{
		return;
	}

	FVector Target = SOCharacter->GetActorLocation() + SOCharacter->GetActorForwardVector() * 200.0f;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		Target = Hit.ImpactPoint;
	}

	SOCharacter->CastCorpseExplosion(Target);
}

void ASOPlayerController::OnBlinkPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (!SOCharacter || !SOCharacter->IsAlive())
	{
		return;
	}

	FVector Target = SOCharacter->GetActorLocation() + SOCharacter->GetActorForwardVector() * 200.0f;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		Target = Hit.ImpactPoint;
	}

	SOCharacter->CastBlink(Target);
}

void ASOPlayerController::OnCursedGroundPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (!SOCharacter || !SOCharacter->IsAlive())
	{
		return;
	}

	FVector Target = SOCharacter->GetActorLocation() + SOCharacter->GetActorForwardVector() * 200.0f;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		Target = Hit.ImpactPoint;
	}

	SOCharacter->PlaceCursedGround(Target);
}

void ASOPlayerController::OnDodgeRollPressed()
{
	ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn());
	if (!SOCharacter || !SOCharacter->IsAlive())
	{
		return;
	}

	FVector Target = SOCharacter->GetActorLocation() + SOCharacter->GetActorForwardVector() * 200.0f;

	FHitResult Hit;
	if (GetHitResultUnderCursor(ClickTraceChannel, /*bTraceComplex=*/ false, Hit) && Hit.bBlockingHit)
	{
		Target = Hit.ImpactPoint;
	}

	SOCharacter->CastDodgeRoll(Target);
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

void ASOPlayerController::OnToggleWaypointMapPressed()
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn()))
	{
		SOCharacter->ToggleWaypointMap();
	}
}

void ASOPlayerController::SelectWaypoint(int32 Index)
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn()))
	{
		SOCharacter->TravelToWaypoint(Index);
	}
}

void ASOPlayerController::OnWaypointSelect1() { SelectWaypoint(0); }
void ASOPlayerController::OnWaypointSelect2() { SelectWaypoint(1); }
void ASOPlayerController::OnWaypointSelect3() { SelectWaypoint(2); }
void ASOPlayerController::OnWaypointSelect4() { SelectWaypoint(3); }
void ASOPlayerController::OnWaypointSelect5() { SelectWaypoint(4); }

void ASOPlayerController::OnRespecTalentsPressed()
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn()))
	{
		SOCharacter->RespecTalents();
	}
}

void ASOPlayerController::OnCycleDifficultyPressed()
{
	if (USODifficultySubsystem* Difficulty = USODifficultySubsystem::Get(GetWorld()))
	{
		Difficulty->CycleDifficulty();
	}
}

void ASOPlayerController::OnToggleBestiaryPressed()
{
	if (ASOCharacter* SOCharacter = Cast<ASOCharacter>(GetPawn()))
	{
		SOCharacter->ToggleBestiary();
	}
}
