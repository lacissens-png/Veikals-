// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "EclipseCore/Aspects.h"
#include "EclipseCore/Reforge.h"
#include "Items/EclipseItemTypes.h"

#include "EclipseForgeComponent.generated.h"

class UEclipseInventoryComponent;

UENUM(BlueprintType)
enum class EEclipseForgeOp : uint8
{
	SelectAffixes,
	Reforge,
	CommitReforge,
	ExtractAspect,
	ImbueAspect
};

UENUM(BlueprintType)
enum class EEclipseForgeError : uint8
{
	None,
	ItemNotFound,
	ItemEquipped,
	NoDatabase,
	RateLimited,
	// Reforge (mirrors Eclipse::EReforgeError)
	ItemNotReforgeable,
	InvalidAffixIndex,
	TooManySelections,
	DuplicateSelection,
	SelectionAlreadyLocked,
	NoSelection,
	AffixNotSelected,
	InsufficientMaterials,
	PoolExhausted,
	NoPendingSession,
	SessionMismatch,
	InvalidChoice,
	// Codex (mirrors Eclipse::ECodexError)
	SourceNotLegendary,
	SourceHasNoAspect,
	UnknownAspect,
	NotInCodex,
	InvalidTargetRarity,
	SlotIncompatible,
	ClassRestricted
};

/** What the client sees of a pending reroll: the 3 candidates, never the RNG state. */
USTRUCT(BlueprintType)
struct FEclipseReforgeOffer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Forge") int64 ItemGuid = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Forge") int32 AffixIndex = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Forge") FEclipseAffixRoll Original;
	UPROPERTY(BlueprintReadOnly, Category = "Forge") TArray<FEclipseAffixRoll> Choices;
	UPROPERTY(BlueprintReadOnly, Category = "Forge") FEclipseMaterialWallet Spent;
};

USTRUCT(BlueprintType)
struct FEclipseCodexEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Codex") int32 AspectId = 0;
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Codex") float BestValue = 0.f;
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Codex") int32 TimesExtracted = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEclipseOnReforgeOffer, const FEclipseReforgeOffer&, Offer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEclipseOnForgeResult, EEclipseForgeOp, Op, EEclipseForgeError, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEclipseOnCodexChanged);

/**
 * The Citadel Forge: Dual-Affix Reforging + Codex of Power, per player.
 *
 * SERVER AUTHORITY MODEL
 *   Client -> Server_* RPC (ids + indices only)
 *          -> server validates, converts FEclipseItemData -> Eclipse::ItemInstance
 *          -> runs the pure core rule with a server-salted Pcg32
 *          -> writes back + marks the inventory entry dirty (owner-only replication)
 *          -> Client_* RPC with the result / offer for UI.
 *   The pending reroll session lives ONLY here on the server; the client commits
 *   by choice index. A tampered client can at most pick one of the 3 offered rolls.
 *
 * Rate limiting stops RPC flooding from burning server CPU.
 */
UCLASS(ClassGroup = (Eclipse), meta = (BlueprintSpawnableComponent))
class ECLIPSEGAME_API UEclipseForgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEclipseForgeComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	// ---- Client requests ---------------------------------------------------------
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Forge")
	void Server_SelectReforgeAffixes(int64 ItemGuid, const TArray<uint8>& AffixIndices);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Forge")
	void Server_RequestReforge(int64 ItemGuid, uint8 AffixIndex, uint8 BloodCatalyst);

	/** ChoiceIndex in [0,2], or -1 to keep the original roll. */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Forge")
	void Server_CommitReforge(int64 ItemGuid, int32 ChoiceIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Forge")
	void Server_ExtractAspect(int64 ItemGuid);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Forge")
	void Server_ImbueAspect(int64 ItemGuid, int32 AspectId);

	// ---- Server -> owning client ----------------------------------------------------
	UFUNCTION(Client, Reliable)
	void Client_ReforgeOffer(const FEclipseReforgeOffer& Offer);

	UFUNCTION(Client, Reliable)
	void Client_ForgeResult(EEclipseForgeOp Op, EEclipseForgeError Error);

	UFUNCTION(BlueprintPure, Category = "Forge")
	TArray<FEclipseCodexEntry> GetCodex() const { return Codex; }

	/** Save-game restore (server). */
	void RestoreCodex(const TArray<FEclipseCodexEntry>& Saved);

	UPROPERTY(BlueprintAssignable) FEclipseOnReforgeOffer OnReforgeOffer;
	UPROPERTY(BlueprintAssignable) FEclipseOnForgeResult OnForgeResult;
	UPROPERTY(BlueprintAssignable) FEclipseOnCodexChanged OnCodexChanged;

protected:
	UFUNCTION() void OnRep_Codex();

	/** Minimum seconds between forge operations per player. */
	UPROPERTY(EditDefaultsOnly, Category = "Forge")
	float MinSecondsBetweenOps = 0.1f;

private:
	UEclipseInventoryComponent* GetInventory() const;
	bool PassesRateLimit();
	void Reply(EEclipseForgeOp Op, EEclipseForgeError Error);
	Eclipse::u64 MakeRollSeed(const Eclipse::ItemInstance& Item) const;
	void SyncCodexFromCore();
	uint8 GetPlayerClassId() const;

	static EEclipseForgeError Map(Eclipse::EReforgeError E);
	static EEclipseForgeError Map(Eclipse::ECodexError E);

	UPROPERTY(ReplicatedUsing = OnRep_Codex)
	TArray<FEclipseCodexEntry> Codex;

	// ---- Server-only state ----
	Eclipse::CodexOfPower CoreCodex;
	Eclipse::ReforgeSession PendingSession;
	Eclipse::u64 ServerSalt = 0;
	double LastOpTime = -1.0;
};
