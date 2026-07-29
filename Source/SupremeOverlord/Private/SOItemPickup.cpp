#include "SOItemPickup.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SOCharacter.h"
#include "SOEquipmentComponent.h"
#include "SOItemData.h"
#include "SOWeaponData.h"
#include "UObject/ConstructorHelpers.h"

ASOItemPickup::ASOItemPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(60.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionSphere->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionSphere);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(CollisionSphere);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetWorldScale3D(FVector(0.35f, 0.35f, 0.35f));

	// Cube for weapons - reads as loot at a glance and the rarity tint carries it visually.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BasicCube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (BasicCube.Succeeded())
	{
		ItemMesh->SetStaticMesh(BasicCube.Object);
	}
}

void ASOItemPickup::BeginPlay()
{
	Super::BeginPlay();

	SpawnBaseLocation = GetActorLocation();

	if (CollisionSphere)
	{
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASOItemPickup::HandleOverlap);
	}

	// Retint on begin play in case CarriedItem was set before overlap events were bound.
	SetItem(CarriedItem);

	if (DespawnAfter > 0.0f)
	{
		SetLifeSpan(DespawnAfter);
	}
}

void ASOItemPickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bClaimed)
	{
		return;
	}

	TimeAlive += DeltaSeconds;

	if (ItemMesh)
	{
		ItemMesh->AddLocalRotation(FRotator(0.0f, SpinRateDegPerSec * DeltaSeconds, 0.0f));
	}

	if (BobAmplitude > 0.0f)
	{
		FVector Loc = GetActorLocation();
		const float BobOffset = FMath::Sin(TimeAlive * BobRateHz * 2.0f * PI) * BobAmplitude;
		Loc.Z = SpawnBaseLocation.Z + BobOffset;
		SetActorLocation(Loc);
	}
}

void ASOItemPickup::SetItem(USOItemData* NewItem)
{
	CarriedItem = NewItem;

	if (!ItemMesh)
	{
		return;
	}

	const FLinearColor Tint = CarriedItem ? CarriedItem->GetRarityColor() : FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);

	if (UMaterialInterface* BaseMat = ItemMesh->GetMaterial(0))
	{
		UMaterialInstanceDynamic* MID = ItemMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (MID)
		{
			// Set the two most common tint parameter names so this works with the engine default material
			// or a custom BP-authored one without extra setup.
			MID->SetVectorParameterValue(TEXT("Color"),     Tint);
			MID->SetVectorParameterValue(TEXT("BaseColor"), Tint);
		}
		(void)BaseMat;
	}
}

void ASOItemPickup::HandleOverlap(UPrimitiveComponent* /*OverlappedComp*/, AActor* OtherActor, UPrimitiveComponent* /*OtherComp*/,
                                  int32 /*OtherBodyIndex*/, bool /*bFromSweep*/, const FHitResult& /*SweepResult*/)
{
	if (bClaimed || !OtherActor)
	{
		return;
	}

	ASOCharacter* Picker = Cast<ASOCharacter>(OtherActor);
	if (!Picker || !Picker->IsAlive())
	{
		return;
	}

	if (!ApplyToCharacter(Picker))
	{
		return;
	}

	bClaimed = true;
	if (PickupSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSFX, GetActorLocation());
	}

	if (CarriedItem && (CarriedItem->Rarity == ESOItemRarity::Legendary
	                 || CarriedItem->Rarity == ESOItemRarity::Unique
	                 || CarriedItem->Rarity == ESOItemRarity::Set))
	{
		const TCHAR* Kind = TEXT("LEGENDARY");
		if (CarriedItem->Rarity == ESOItemRarity::Unique) Kind = TEXT("UNIQUE");
		else if (CarriedItem->Rarity == ESOItemRarity::Set) Kind = TEXT("SET ITEM");
		Picker->ShowLegendaryDropToast(FString::Printf(TEXT("%s ACQUIRED: %s"), Kind, *CarriedItem->DisplayName.ToString()));
	}

	OnPickedUp(Picker);
	Destroy();
}

bool ASOItemPickup::ApplyToCharacter(ASOCharacter* Picker)
{
	if (!Picker || !CarriedItem)
	{
		return false;
	}

	// Route every equippable item through the equipment component; it in
	// turn calls back into EquipWeapon for MainHand so existing damage code
	// keeps scaling correctly.
	if (CarriedItem->EquipSlot != ESOEquipSlot::None)
	{
		if (USOEquipmentComponent* Eq = Picker->EquipmentComponent)
		{
			return Eq->Equip(CarriedItem);
		}
		// Fallback: legacy path for weapons only.
		if (USOWeaponData* Weapon = Cast<USOWeaponData>(CarriedItem))
		{
			Picker->EquipWeapon(Weapon);
			return true;
		}
	}

	// Non-equippable item — leave it in the world so an inventory system can grab it later.
	return false;
}
