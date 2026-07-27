#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOVendorNPC.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USOItemData;
class ASOCharacter;

/** One row of a vendor's stock. */
USTRUCT(BlueprintType)
struct FSOVendorItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VendorItem")
	TObjectPtr<USOItemData> Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VendorItem", meta = (ClampMin = "0", UIMin = "0", UIMax = "100000"))
	int32 Price = 100;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnVendorPlayerEnteredRange, ASOCharacter*, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSOOnVendorPlayerLeftRange,    ASOCharacter*, Player);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSOOnVendorPurchase,
	ASOCharacter*, Buyer,
	USOItemData*,  Item,
	int32,         Price);

/**
 * Merchant that sells items for gold. Players interact by walking into
 * InteractionRadius and pressing "Interact" (F by default). Without a
 * dedicated shop UI, the vendor cycles through its stock: each interact
 * press attempts to buy the next affordable row.
 *
 * On sell, the vendor also auto-buys the player's currently equipped
 * weapon at SellbackFraction of its listed price (falls back to
 * SellbackFlatFallback when the item isn't in the vendor's stock table).
 */
UCLASS()
class SUPREMEOVERLORD_API ASOVendorNPC : public AActor
{
	GENERATED_BODY()

public:
	ASOVendorNPC();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Vendor")
	TObjectPtr<USphereComponent> InteractionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SupremeOverlord|Vendor")
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Vendor")
	FText VendorDisplayName = FText::FromString(TEXT("Wandering Merchant"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Vendor", meta = (ClampMin = "50.0", UIMin = "50.0", UIMax = "2000.0"))
	float InteractionRadius = 220.0f;

	/** Items this vendor sells, in cycle order. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Vendor")
	TArray<FSOVendorItem> Stock;

	/** Fraction of an item's listed price the vendor pays when the player sells it back. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Vendor", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float SellbackFraction = 0.35f;

	/** Fallback sell price when the item isn't in the vendor's stock table. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupremeOverlord|Vendor", meta = (ClampMin = "0", UIMin = "0", UIMax = "10000"))
	int32 SellbackFlatFallback = 25;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Vendor")
	FSOOnVendorPlayerEnteredRange OnPlayerEnteredRange;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Vendor")
	FSOOnVendorPlayerLeftRange    OnPlayerLeftRange;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Vendor")
	FSOOnVendorPurchase           OnPurchase;

	UPROPERTY(BlueprintAssignable, Category = "SupremeOverlord|Vendor")
	FSOOnVendorPurchase           OnSellback;

	/** Attempts to sell the next affordable stock item to Buyer. Returns true on success. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Vendor")
	bool TryBuyNext(ASOCharacter* Buyer);

	/** Buys back the buyer's currently equipped weapon at SellbackFraction. */
	UFUNCTION(BlueprintCallable, Category = "SupremeOverlord|Vendor")
	bool TrySellCurrentWeapon(ASOCharacter* Seller);

	/** True if Player is currently inside our interaction sphere. */
	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Vendor")
	bool IsPlayerInRange(ASOCharacter* Player) const { return PlayersInRange.Contains(Player); }

	UFUNCTION(BlueprintPure, Category = "SupremeOverlord|Vendor")
	const TArray<TWeakObjectPtr<ASOCharacter>>& GetPlayersInRange() const { return PlayersInRange; }

private:
	int32 ComputeSellbackPrice(USOItemData* Item) const;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<ASOCharacter>> PlayersInRange;

	int32 StockCursor = 0;
};
