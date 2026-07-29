#include "SOBestiaryComponent.h"

#include "SOEnemyCharacter.h"

USOBestiaryComponent::USOBestiaryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USOBestiaryComponent::RecordKill(TSubclassOf<ASOEnemyCharacter> EnemyClass)
{
	if (!EnemyClass)
	{
		return;
	}

	int32& Count = KillCounts.FindOrAdd(EnemyClass);
	++Count;

	OnBestiaryEntryUpdated.Broadcast(EnemyClass, Count);
}

int32 USOBestiaryComponent::GetKillCount(TSubclassOf<ASOEnemyCharacter> EnemyClass) const
{
	if (const int32* Found = KillCounts.Find(EnemyClass))
	{
		return *Found;
	}
	return 0;
}

int32 USOBestiaryComponent::GetTotalKills() const
{
	int32 Total = 0;
	for (const TPair<TSubclassOf<ASOEnemyCharacter>, int32>& Pair : KillCounts)
	{
		Total += Pair.Value;
	}
	return Total;
}

TArray<FString> USOBestiaryComponent::GetEntryDescriptions() const
{
	TArray<TPair<FString, int32>> Entries;
	Entries.Reserve(KillCounts.Num());

	for (const TPair<TSubclassOf<ASOEnemyCharacter>, int32>& Pair : KillCounts)
	{
		FString Name;
		if (const ASOEnemyCharacter* CDO = Pair.Key ? Pair.Key->GetDefaultObject<ASOEnemyCharacter>() : nullptr)
		{
			Name = CDO->BestiaryDisplayName.IsEmpty() ? Pair.Key->GetName() : CDO->BestiaryDisplayName.ToString();
		}
		else
		{
			Name = TEXT("Unknown");
		}
		Entries.Emplace(Name, Pair.Value);
	}

	Entries.Sort([](const TPair<FString, int32>& A, const TPair<FString, int32>& B) { return A.Value > B.Value; });

	TArray<FString> Lines;
	Lines.Reserve(Entries.Num());
	for (const TPair<FString, int32>& Entry : Entries)
	{
		Lines.Add(FString::Printf(TEXT("%s: %d"), *Entry.Key, Entry.Value));
	}
	return Lines;
}

void USOBestiaryComponent::RestoreKillCount(TSubclassOf<ASOEnemyCharacter> EnemyClass, int32 Count)
{
	if (!EnemyClass || Count <= 0)
	{
		return;
	}
	KillCounts.FindOrAdd(EnemyClass) = Count;
}

void USOBestiaryComponent::ToggleCodex()
{
	bCodexOpen = !bCodexOpen;
}
