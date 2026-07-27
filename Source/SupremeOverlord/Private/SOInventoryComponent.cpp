#include "SOInventoryComponent.h"

#include "SOCharacter.h"
#include "SOEquipmentComponent.h"
#include "SOItemData.h"
#include "SOMaterialData.h"
#include "SORecipeData.h"

USOInventoryComponent::USOInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

int32 USOInventoryComponent::GetMaterialCount(USOMaterialData* Material) const
{
	if (!Material)
	{
		return 0;
	}
	const int32* Found = Materials.Find(Material);
	return Found ? *Found : 0;
}

int32 USOInventoryComponent::AddMaterial(USOMaterialData* Material, int32 Count)
{
	if (!Material || Count <= 0)
	{
		return 0;
	}

	int32& Slot = Materials.FindOrAdd(Material);
	const int32 Cap    = Material->MaxStack > 0 ? Material->MaxStack : TNumericLimits<int32>::Max();
	const int32 Room   = FMath::Max(0, Cap - Slot);
	const int32 Added  = FMath::Min(Count, Room);
	if (Added > 0)
	{
		Slot += Added;
		OnMaterialCountChanged.Broadcast(Material, Slot, Added);
	}
	return Added;
}

int32 USOInventoryComponent::RemoveMaterial(USOMaterialData* Material, int32 Count)
{
	if (!Material || Count <= 0)
	{
		return 0;
	}
	int32* Found = Materials.Find(Material);
	if (!Found)
	{
		return 0;
	}
	const int32 Removed = FMath::Min(Count, *Found);
	*Found -= Removed;
	if (*Found <= 0)
	{
		Materials.Remove(Material);
		OnMaterialCountChanged.Broadcast(Material, 0, -Removed);
	}
	else
	{
		OnMaterialCountChanged.Broadcast(Material, *Found, -Removed);
	}
	return Removed;
}

bool USOInventoryComponent::CanCraft(USORecipeData* Recipe, FString& OutReason) const
{
	if (!Recipe)
	{
		OutReason = TEXT("No recipe supplied.");
		return false;
	}
	if (!Recipe->Result)
	{
		OutReason = TEXT("Recipe has no result.");
		return false;
	}
	for (const FSORecipeIngredient& Ing : Recipe->Ingredients)
	{
		if (!Ing.Material)
		{
			OutReason = TEXT("Recipe row is missing a material asset.");
			return false;
		}
		if (GetMaterialCount(Ing.Material) < Ing.Count)
		{
			OutReason = FString::Printf(TEXT("Need %d %s (have %d)."),
			                            Ing.Count,
			                            *Ing.Material->DisplayName.ToString(),
			                            GetMaterialCount(Ing.Material));
			return false;
		}
	}
	if (Recipe->GoldCost > 0)
	{
		const ASOCharacter* SO = Cast<ASOCharacter>(GetOwner());
		if (!SO || SO->GetGold() < Recipe->GoldCost)
		{
			OutReason = FString::Printf(TEXT("Need %d gold."), Recipe->GoldCost);
			return false;
		}
	}
	OutReason.Reset();
	return true;
}

bool USOInventoryComponent::Craft(USORecipeData* Recipe)
{
	FString Reason;
	if (!CanCraft(Recipe, Reason))
	{
		return false;
	}

	// Consume ingredients + gold.
	for (const FSORecipeIngredient& Ing : Recipe->Ingredients)
	{
		RemoveMaterial(Ing.Material, Ing.Count);
	}

	ASOCharacter* Owner = Cast<ASOCharacter>(GetOwner());
	if (Recipe->GoldCost > 0 && Owner)
	{
		Owner->AddGold(-Recipe->GoldCost);
	}

	USOItemData* Result = Recipe->Result;

	// Auto-equip when the result declares a slot; otherwise leave the granting
	// to the BP hook or a future item inventory system.
	if (Owner && Result && Result->EquipSlot != ESOEquipSlot::None)
	{
		if (USOEquipmentComponent* Eq = Owner->EquipmentComponent)
		{
			Eq->Equip(Result);
		}
	}

	OnRecipeCrafted.Broadcast(Recipe, Result);
	return true;
}
