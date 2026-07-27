#include "SOVendorNPC.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "SOCharacter.h"
#include "SOEquipmentComponent.h"
#include "SOItemData.h"
#include "SOWeaponData.h"
#include "UObject/ConstructorHelpers.h"

ASOVendorNPC::ASOVendorNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->InitSphereRadius(InteractionRadius);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractionSphere->SetGenerateOverlapEvents(true);
	SetRootComponent(InteractionSphere);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(InteractionSphere);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetWorldScale3D(FVector(0.7f, 0.7f, 1.6f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BasicCyl(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (BasicCyl.Succeeded())
	{
		BodyMesh->SetStaticMesh(BasicCyl.Object);
	}
}

void ASOVendorNPC::BeginPlay()
{
	Super::BeginPlay();

	if (InteractionSphere)
	{
		InteractionSphere->SetSphereRadius(InteractionRadius);
		InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASOVendorNPC::HandleBeginOverlap);
		InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ASOVendorNPC::HandleEndOverlap);
	}
}

void ASOVendorNPC::HandleBeginOverlap(UPrimitiveComponent* /*OverlappedComp*/, AActor* OtherActor, UPrimitiveComponent* /*OtherComp*/,
                                      int32 /*OtherBodyIndex*/, bool /*bFromSweep*/, const FHitResult& /*SweepResult*/)
{
	ASOCharacter* Player = Cast<ASOCharacter>(OtherActor);
	if (!Player)
	{
		return;
	}
	PlayersInRange.AddUnique(TWeakObjectPtr<ASOCharacter>(Player));
	OnPlayerEnteredRange.Broadcast(Player);
}

void ASOVendorNPC::HandleEndOverlap(UPrimitiveComponent* /*OverlappedComp*/, AActor* OtherActor, UPrimitiveComponent* /*OtherComp*/,
                                    int32 /*OtherBodyIndex*/)
{
	ASOCharacter* Player = Cast<ASOCharacter>(OtherActor);
	if (!Player)
	{
		return;
	}
	PlayersInRange.RemoveAllSwap([Player](const TWeakObjectPtr<ASOCharacter>& P)
	{
		return !P.IsValid() || P.Get() == Player;
	});
	OnPlayerLeftRange.Broadcast(Player);
}

int32 ASOVendorNPC::ComputeSellbackPrice(USOItemData* Item) const
{
	if (!Item)
	{
		return 0;
	}
	for (const FSOVendorItem& Row : Stock)
	{
		if (Row.Item == Item)
		{
			return FMath::Max(1, FMath::RoundToInt(Row.Price * SellbackFraction));
		}
	}
	return SellbackFlatFallback;
}

bool ASOVendorNPC::TryBuyNext(ASOCharacter* Buyer)
{
	if (!Buyer || Stock.Num() == 0)
	{
		return false;
	}

	// Look through every stock row starting from the cursor for the next affordable one the buyer doesn't already own.
	for (int32 i = 0; i < Stock.Num(); ++i)
	{
		const int32 Idx = (StockCursor + i) % Stock.Num();
		const FSOVendorItem& Row = Stock[Idx];
		if (!Row.Item)
		{
			continue;
		}

		if (Buyer->GetGold() < Row.Price)
		{
			continue;
		}

		// Skip the row if the buyer already has this exact item in the matching equip slot.
		if (USOEquipmentComponent* Eq = Buyer->EquipmentComponent)
		{
			if (Row.Item->EquipSlot != ESOEquipSlot::None && Eq->GetItemInSlot(Row.Item->EquipSlot) == Row.Item)
			{
				continue;
			}
		}

		// Transact.
		Buyer->AddGold(-Row.Price);
		if (USOEquipmentComponent* Eq = Buyer->EquipmentComponent)
		{
			Eq->Equip(Row.Item);
		}
		StockCursor = (Idx + 1) % Stock.Num();
		OnPurchase.Broadcast(Buyer, Row.Item, Row.Price);
		return true;
	}
	return false;
}

bool ASOVendorNPC::TrySellCurrentWeapon(ASOCharacter* Seller)
{
	if (!Seller)
	{
		return false;
	}
	USOEquipmentComponent* Eq = Seller->EquipmentComponent;
	if (!Eq)
	{
		return false;
	}
	USOItemData* Weapon = Eq->GetItemInSlot(ESOEquipSlot::MainHand);
	if (!Weapon)
	{
		return false;
	}

	const int32 Price = ComputeSellbackPrice(Weapon);
	Eq->Unequip(ESOEquipSlot::MainHand);
	Seller->AddGold(Price);

	OnSellback.Broadcast(Seller, Weapon, Price);
	return true;
}
