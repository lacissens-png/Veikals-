#include "SOQuestComponent.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SOCharacter.h"
#include "SOEnemyCharacter.h"
#include "SOEnemySpawner.h"
#include "SOBossCharacter.h"
#include "SOEquipmentComponent.h"
#include "SOExperienceComponent.h"
#include "SOInventoryComponent.h"
#include "SOItemData.h"
#include "SOMaterialData.h"
#include "SOVassalComponent.h"
#include "SOVassalData.h"

// ---------------------------------------------------------------------------
// FSOActiveQuest helpers
// ---------------------------------------------------------------------------
bool FSOActiveQuest::IsObjectiveComplete(int32 Idx) const
{
	if (!Data || !Progress.IsValidIndex(Idx))
	{
		return false;
	}
	return Progress[Idx] >= Data->Objectives[Idx].RequiredCount;
}

bool FSOActiveQuest::IsAllComplete() const
{
	if (!Data)
	{
		return false;
	}
	for (int32 i = 0; i < Data->Objectives.Num(); ++i)
	{
		if (!IsObjectiveComplete(i))
		{
			return false;
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
// USOQuestComponent
// ---------------------------------------------------------------------------
USOQuestComponent::USOQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.25f; // poll ReachLocation at 4 Hz
}

void USOQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// Bind to all SOEnemySpawner wave-cleared delegates in the world
	TArray<AActor*> Spawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASOEnemySpawner::StaticClass(), Spawners);
	for (AActor* A : Spawners)
	{
		if (ASOEnemySpawner* Spawner = Cast<ASOEnemySpawner>(A))
		{
			Spawner->OnWaveCleared.AddDynamic(this, &USOQuestComponent::HandleWaveCleared);
		}
	}

	// Bind to inventory so CollectMaterial objectives update in real time
	if (ASOCharacter* Char = Cast<ASOCharacter>(GetOwner()))
	{
		if (Char->InventoryComponent)
		{
			Char->InventoryComponent->OnMaterialCountChanged.AddDynamic(
				this, &USOQuestComponent::HandleMaterialCountChanged);
		}
	}
}

void USOQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const FVector PlayerLoc = Owner->GetActorLocation();

	// Reverse-index iteration: TryCompleteQuest() removes the current entry
	// from ActiveQuests via RemoveAll, which shifts every later element down
	// by one. A forward/range-based loop's cached end iterator goes stale the
	// moment that happens; walking backwards means a removal only ever
	// affects indices we've already visited, never the ones still ahead.
	for (int32 QuestIdx = ActiveQuests.Num() - 1; QuestIdx >= 0; --QuestIdx)
	{
		FSOActiveQuest& AQ = ActiveQuests[QuestIdx];
		if (!AQ.Data)
		{
			continue;
		}

		for (int32 i = 0; i < AQ.Data->Objectives.Num(); ++i)
		{
			const FSOQuestObjective& Obj = AQ.Data->Objectives[i];
			if (Obj.Type != ESOQuestObjectiveType::ReachLocation || AQ.IsObjectiveComplete(i))
			{
				continue;
			}

			if (FVector::DistSquared(PlayerLoc, Obj.TargetLocation) <= FMath::Square(Obj.AcceptanceRadius))
			{
				AQ.Progress[i] = Obj.RequiredCount;
				OnObjectiveUpdated.Broadcast(AQ.Data, i);
				TryCompleteQuest(AQ);
				// TryCompleteQuest may have removed AQ's backing entry - stop
				// touching it and move on to the next quest.
				break;
			}
		}
	}
}

bool USOQuestComponent::AcceptQuest(USOQuestData* Quest)
{
	if (!Quest || IsQuestActive(Quest) || IsQuestComplete(Quest))
	{
		return false;
	}

	// Check prerequisite
	if (!Quest->PrerequisiteQuest.IsNull())
	{
		USOQuestData* Prereq = Quest->PrerequisiteQuest.LoadSynchronous();
		if (Prereq && !IsQuestComplete(Prereq))
		{
			return false;
		}
	}

	FSOActiveQuest AQ;
	AQ.Data = Quest;
	AQ.Progress.SetNum(Quest->Objectives.Num());

	const int32 SlotIndex = ActiveQuests.Add(AQ);
	OnQuestAccepted.Broadcast(Quest, SlotIndex);

	// Seed material objectives from current inventory
	SeedMaterialObjectives();

	return true;
}

void USOQuestComponent::FailQuest(USOQuestData* Quest)
{
	for (int32 i = ActiveQuests.Num() - 1; i >= 0; --i)
	{
		if (ActiveQuests[i].Data == Quest)
		{
			ActiveQuests.RemoveAt(i);
			OnQuestFailed.Broadcast(Quest);
			return;
		}
	}
}

bool USOQuestComponent::IsQuestActive(USOQuestData* Quest) const
{
	for (const FSOActiveQuest& AQ : ActiveQuests)
	{
		if (AQ.Data == Quest) return true;
	}
	return false;
}

bool USOQuestComponent::IsQuestComplete(USOQuestData* Quest) const
{
	return CompletedQuests.Contains(Quest);
}

int32 USOQuestComponent::GetObjectiveProgress(USOQuestData* Quest, int32 ObjectiveIndex) const
{
	for (const FSOActiveQuest& AQ : ActiveQuests)
	{
		if (AQ.Data == Quest && AQ.Progress.IsValidIndex(ObjectiveIndex))
		{
			return AQ.Progress[ObjectiveIndex];
		}
	}
	return 0;
}

void USOQuestComponent::RestoreQuestState(const TArray<FSOActiveQuest>& InActiveQuests, const TArray<USOQuestData*>& InCompletedQuests)
{
	ActiveQuests = InActiveQuests;

	CompletedQuests.Reset();
	for (USOQuestData* Quest : InCompletedQuests)
	{
		if (Quest)
		{
			CompletedQuests.Add(Quest);
		}
	}
}

void USOQuestComponent::NotifyEnemyKilled(AActor* EnemyActor)
{
	if (!EnemyActor)
	{
		return;
	}

	// See TickComponent for why this walks ActiveQuests backwards by index -
	// AdvanceObjective() can complete and remove the current entry mid-loop.
	for (int32 QuestIdx = ActiveQuests.Num() - 1; QuestIdx >= 0; --QuestIdx)
	{
		FSOActiveQuest& AQ = ActiveQuests[QuestIdx];
		if (!AQ.Data) continue;

		USOQuestData* const QuestData = AQ.Data;

		for (int32 i = 0; i < AQ.Data->Objectives.Num(); ++i)
		{
			if (AQ.IsObjectiveComplete(i)) continue;

			const FSOQuestObjective& Obj = AQ.Data->Objectives[i];

			bool bMatches = false;
			if (Obj.Type == ESOQuestObjectiveType::KillEnemies)
			{
				if (Obj.TargetClass.IsNull())
				{
					bMatches = EnemyActor->IsA(ASOEnemyCharacter::StaticClass());
				}
				else
				{
					UClass* Target = Obj.TargetClass.LoadSynchronous();
					bMatches = Target && EnemyActor->IsA(Target);
				}
			}
			else if (Obj.Type == ESOQuestObjectiveType::DefeatBoss)
			{
				if (Obj.TargetClass.IsNull())
				{
					bMatches = EnemyActor->IsA(ASOBossCharacter::StaticClass());
				}
				else
				{
					UClass* Target = Obj.TargetClass.LoadSynchronous();
					bMatches = Target && EnemyActor->IsA(Target);
				}
			}

			if (bMatches)
			{
				AdvanceObjective(AQ, i, 1);

				// AdvanceObjective may have completed the quest and removed
				// AQ's backing entry (its content, at this same array slot,
				// could now belong to a different quest entirely thanks to
				// the shift) - detect that via the captured pointer, not AQ,
				// and stop touching this quest's objectives if so.
				if (!IsQuestActive(QuestData))
				{
					break;
				}
			}
		}
	}
}

void USOQuestComponent::HandleMaterialCountChanged(USOMaterialData* /*Material*/, int32 /*NewCount*/, int32 /*Delta*/)
{
	SeedMaterialObjectives();
}

void USOQuestComponent::HandleWaveCleared(int32 /*WaveIndex*/)
{
	// See NotifyEnemyKilled for why this walks ActiveQuests backwards by index.
	for (int32 QuestIdx = ActiveQuests.Num() - 1; QuestIdx >= 0; --QuestIdx)
	{
		FSOActiveQuest& AQ = ActiveQuests[QuestIdx];
		if (!AQ.Data) continue;

		USOQuestData* const QuestData = AQ.Data;

		for (int32 i = 0; i < AQ.Data->Objectives.Num(); ++i)
		{
			if (AQ.IsObjectiveComplete(i)) continue;
			if (AQ.Data->Objectives[i].Type == ESOQuestObjectiveType::ClearWaves)
			{
				AdvanceObjective(AQ, i, 1);

				if (!IsQuestActive(QuestData))
				{
					break;
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------
void USOQuestComponent::AdvanceObjective(FSOActiveQuest& AQ, int32 ObjIdx, int32 Delta)
{
	if (!AQ.Data || !AQ.Progress.IsValidIndex(ObjIdx)) return;

	const int32 Required = AQ.Data->Objectives[ObjIdx].RequiredCount;
	AQ.Progress[ObjIdx] = FMath::Min(AQ.Progress[ObjIdx] + Delta, Required);
	OnObjectiveUpdated.Broadcast(AQ.Data, ObjIdx);

	if (AQ.IsObjectiveComplete(ObjIdx))
	{
		TryCompleteQuest(AQ);
	}
}

void USOQuestComponent::TryCompleteQuest(FSOActiveQuest& AQ)
{
	if (!AQ.IsAllComplete()) return;

	USOQuestData* Data = AQ.Data;

	// Remove from active before broadcasting so re-entrant Accept calls see it as complete
	ActiveQuests.RemoveAll([&](const FSOActiveQuest& Q) { return Q.Data == Data; });
	CompletedQuests.Add(Data);

	GrantReward(Data->Reward);
	OnQuestCompleted.Broadcast(Data);
}

void USOQuestComponent::GrantReward(const FSOQuestReward& Reward)
{
	ASOCharacter* Character = Cast<ASOCharacter>(GetOwner());
	if (!Character) return;

	if (Reward.XP > 0 && Character->ExperienceComponent)
	{
		Character->ExperienceComponent->GainXP(Reward.XP);
	}

	if (Reward.Gold > 0)
	{
		Character->AddGold(Reward.Gold);
	}

	if (!Reward.ItemReward.IsNull())
	{
		if (USOItemData* Item = Reward.ItemReward.LoadSynchronous())
		{
			if (Character->EquipmentComponent)
			{
				Character->EquipmentComponent->Equip(Item);
			}
		}
	}

	if (!Reward.VassalReward.IsNull())
	{
		if (USOVassalData* Vassal = Reward.VassalReward.LoadSynchronous())
		{
			if (Character->VassalComponent)
			{
				Character->VassalComponent->RecruitVassal(Vassal);
			}
		}
	}
}

void USOQuestComponent::SeedMaterialObjectives()
{
	ASOCharacter* Character = Cast<ASOCharacter>(GetOwner());
	if (!Character || !Character->InventoryComponent)
	{
		return;
	}

	// See TickComponent for why this walks ActiveQuests backwards by index.
	for (int32 QuestIdx = ActiveQuests.Num() - 1; QuestIdx >= 0; --QuestIdx)
	{
		FSOActiveQuest& AQ = ActiveQuests[QuestIdx];
		if (!AQ.Data) continue;

		for (int32 i = 0; i < AQ.Data->Objectives.Num(); ++i)
		{
			const FSOQuestObjective& Obj = AQ.Data->Objectives[i];
			if (Obj.Type != ESOQuestObjectiveType::CollectMaterial) continue;
			if (AQ.IsObjectiveComplete(i)) continue;
			if (Obj.TargetMaterial.IsNull()) continue;

			USOMaterialData* Mat = Cast<USOMaterialData>(Obj.TargetMaterial.LoadSynchronous());
			if (!Mat) continue;

			const int32 Have    = Character->InventoryComponent->GetMaterialCount(Mat);
			const int32 Clamped = FMath::Clamp(Have, 0, Obj.RequiredCount);
			if (Clamped != AQ.Progress[i])
			{
				AQ.Progress[i] = Clamped;
				OnObjectiveUpdated.Broadcast(AQ.Data, i);
				if (AQ.IsObjectiveComplete(i))
				{
					TryCompleteQuest(AQ);
					break; // AQ's backing entry may be gone/shifted now.
				}
			}
		}
	}
}

FSOActiveQuest* USOQuestComponent::FindActiveQuest(USOQuestData* Data)
{
	for (FSOActiveQuest& AQ : ActiveQuests)
	{
		if (AQ.Data == Data) return &AQ;
	}
	return nullptr;
}
