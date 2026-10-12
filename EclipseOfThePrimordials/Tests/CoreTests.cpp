// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// CoreTests.cpp
// -----------------------------------------------------------------------------
// Standalone unit tests for the EclipseCore rules module. No engine required:
//
//     cd Tests && make test
//
// The same cases can be ported 1:1 to UE Automation Specs (see
// Source/EclipseGame/Private/Tests/EclipseCoreAutomationTests.cpp) so they also
// run in CI through `UnrealEditor-Cmd -ExecCmds="Automation RunTests Eclipse"`.
// -----------------------------------------------------------------------------

#include "EclipseCore/Aspects.h"
#include "EclipseCore/DamageMath.h"
#include "EclipseCore/Reforge.h"
#include "EclipseCore/Runewords.h"
#include "EclipseCore/SpatialHash.h"
#include "EclipseCore/StatusEffects.h"

#include <cmath>
#include <cstdio>
#include <functional>
#include <set>
#include <string>
#include <vector>

using namespace Eclipse;

// ---------------------------------------------------------------------------
// Minimal test harness
// ---------------------------------------------------------------------------
namespace
{
	int GFailures = 0;
	int GChecks = 0;
	std::vector<std::pair<std::string, std::function<void()>>>& Registry()
	{
		static std::vector<std::pair<std::string, std::function<void()>>> R;
		return R;
	}
	struct Registrar
	{
		Registrar(const char* Name, std::function<void()> Fn) { Registry().emplace_back(Name, std::move(Fn)); }
	};
}

#define TEST(Name) \
	static void Name(); \
	static Registrar Name##_reg(#Name, Name); \
	static void Name()

#define CHECK(Cond) \
	do { ++GChecks; if (!(Cond)) { ++GFailures; std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #Cond); } } while (0)

#define CHECK_NEAR(A, B, Eps) CHECK(std::fabs((A) - (B)) <= (Eps))

// ---------------------------------------------------------------------------
// Fixtures
// ---------------------------------------------------------------------------
namespace
{
	AffixPool MakePool()
	{
		AffixPool Pool;
		u32 Id = 1;
		for (std::size_t S = 0; S < AffixStatCount; ++S)
		{
			AffixDefinition D;
			D.Id = Id++;
			D.Stat = static_cast<EAffixStat>(S);
			D.MinValue = 10.f;
			D.MaxValue = 20.f;
			D.Weight = 100;
			Pool.Add(D);
		}
		return Pool;
	}

	ItemInstance MakeRare(const AffixPool& Pool, u64 Seed = 42, EItemRarity Rarity = EItemRarity::Rare)
	{
		Pcg32 Rng(Seed);
		ItemGenerationParams P;
		P.Guid = 1000 + Seed;
		P.BaseId = 7;
		P.Slot = EItemSlot::TwoHand;
		P.Rarity = Rarity;
		P.ItemLevel = 800;
		P.SocketPairs = 2;
		return GenerateItem(P, Pool, Rng);
	}

	MaterialBundle RichWallet() { return { 100000, 100000, 100000 }; }
}

// ===========================================================================
// Item generation
// ===========================================================================

TEST(ItemGeneration_NoDuplicateStats_AndAncestralHasGreater)
{
	const AffixPool Pool = MakePool();
	for (u64 Seed = 1; Seed < 200; ++Seed)
	{
		const ItemInstance Item = MakeRare(Pool, Seed, EItemRarity::AncestralEldritch);
		CHECK(Item.ExplicitAffixes.size() == 4);
		std::set<EAffixStat> Stats;
		bool bGreater = false;
		for (const AffixRoll& R : Item.ExplicitAffixes)
		{
			Stats.insert(Pool.Find(R.DefinitionId)->Stat);
			bGreater |= R.bGreater;
			CHECK(R.Quality >= 0.5f); // Ancestral quality floor.
		}
		CHECK(Stats.size() == Item.ExplicitAffixes.size());
		CHECK(bGreater);
		CHECK(Item.Sockets.size() == 2);
	}
}

// ===========================================================================
// ReforgeAffixes()
// ===========================================================================

TEST(Reforge_SelectionRules)
{
	const AffixPool Pool = MakePool();
	ItemInstance Item = MakeRare(Pool);

	CHECK(SelectReforgeAffixes(Item, {}) == EReforgeError::NoSelection);
	CHECK(SelectReforgeAffixes(Item, { 0, 1, 2 }) == EReforgeError::TooManySelections);
	CHECK(SelectReforgeAffixes(Item, { 1, 1 }) == EReforgeError::DuplicateSelection);
	CHECK(SelectReforgeAffixes(Item, { 9 }) == EReforgeError::InvalidAffixIndex);
	CHECK(Item.ReforgeSelectionMask == 0); // Failed attempts leave no partial state.

	CHECK(SelectReforgeAffixes(Item, { 0, 2 }) == EReforgeError::None);
	CHECK(Item.ReforgeSelectionMask == 0b101);
	CHECK(SelectReforgeAffixes(Item, { 1 }) == EReforgeError::SelectionAlreadyLocked); // Write-once.

	ItemInstance Magic = MakeRare(Pool, 3, EItemRarity::Magic);
	CHECK(SelectReforgeAffixes(Magic, { 0 }) == EReforgeError::ItemNotReforgeable);
}

TEST(Reforge_LockedAffixesCannotBeRerolled)
{
	const AffixPool Pool = MakePool();
	ItemInstance Item = MakeRare(Pool);
	MaterialBundle Wallet = RichWallet();
	Pcg32 Rng(1);

	CHECK(ReforgeAffixes(Item, 0, Wallet, 0, Pool, Rng).Error == EReforgeError::NoSelection);
	CHECK(SelectReforgeAffixes(Item, { 0, 2 }) == EReforgeError::None);
	const MaterialBundle Before = Wallet;
	CHECK(ReforgeAffixes(Item, 1, Wallet, 0, Pool, Rng).Error == EReforgeError::AffixNotSelected);
	CHECK(Wallet == Before); // Nothing charged on failure.
}

TEST(Reforge_ReturnsThreeDistinctLegalChoices_AndOnlyTouchesSelectedSlot)
{
	const AffixPool Pool = MakePool();
	ItemInstance Item = MakeRare(Pool);
	CHECK(SelectReforgeAffixes(Item, { 1, 2 }) == EReforgeError::None);
	const std::vector<AffixRoll> Snapshot = Item.ExplicitAffixes;

	MaterialBundle Wallet = RichWallet();
	Pcg32 Rng(99);
	ReforgeResult R = ReforgeAffixes(Item, 1, Wallet, 0, Pool, Rng);
	CHECK(R.Succeeded());
	CHECK(R.Session.NumChoices == ReforgeChoiceCount);
	CHECK(Item.ExplicitAffixes == Snapshot); // Not mutated until commit.

	const StatMask Others = CollectStats(Item, Pool, 1);
	std::set<EAffixStat> ChoiceStats;
	for (int C = 0; C < R.Session.NumChoices; ++C)
	{
		const EAffixStat S = Pool.Find(R.Session.Choices[C].DefinitionId)->Stat;
		CHECK(!Others.test(Idx(S)));        // Never duplicates another affix on the item.
		ChoiceStats.insert(S);
	}
	CHECK(ChoiceStats.size() == 3);         // Mutually distinct.

	CHECK(CommitReforgeChoice(Item, R.Session, 2) == EReforgeError::None);
	CHECK(Item.ExplicitAffixes[1] == R.Session.Choices[2]);
	CHECK(Item.ExplicitAffixes[0] == Snapshot[0]); // Locked lines bit-identical.
	CHECK(Item.ExplicitAffixes[2] == Snapshot[2]);
	CHECK(!R.Session.bActive);
	CHECK(CommitReforgeChoice(Item, R.Session, 0) == EReforgeError::NoPendingSession); // Single use.
}

TEST(Reforge_KeepOriginal_InvalidChoice_AndTamperDetection)
{
	const AffixPool Pool = MakePool();
	ItemInstance Item = MakeRare(Pool);
	CHECK(SelectReforgeAffixes(Item, { 0 }) == EReforgeError::None);
	MaterialBundle Wallet = RichWallet();
	Pcg32 Rng(5);

	ReforgeResult R = ReforgeAffixes(Item, 0, Wallet, 0, Pool, Rng);
	const AffixRoll Original = Item.ExplicitAffixes[0];
	CHECK(CommitReforgeChoice(Item, R.Session, 7) == EReforgeError::InvalidChoice);
	CHECK(R.Session.bActive); // Bad index does not forfeit the paid session.
	CHECK(CommitReforgeChoice(Item, R.Session, KeepOriginalChoice) == EReforgeError::None);
	CHECK(Item.ExplicitAffixes[0] == Original);

	// Session opened, then the item is modified behind its back -> rejected.
	ReforgeResult R2 = ReforgeAffixes(Item, 0, Wallet, 0, Pool, Rng);
	Item.ExplicitAffixes[0].Value += 1.f;
	CHECK(CommitReforgeChoice(Item, R2.Session, 0) == EReforgeError::SessionMismatch);

	// Session for a different item.
	ItemInstance Other = MakeRare(Pool, 77);
	CHECK(SelectReforgeAffixes(Other, { 0 }) == EReforgeError::None);
	ReforgeResult R3 = ReforgeAffixes(Item, 0, Wallet, 0, Pool, Rng);
	CHECK(CommitReforgeChoice(Other, R3.Session, 0) == EReforgeError::SessionMismatch);
}

TEST(Reforge_MaterialsChargedOnce_CostScales_CatalystRaisesFloor)
{
	const AffixPool Pool = MakePool();
	ItemInstance Item = MakeRare(Pool, 11, EItemRarity::AncestralEldritch);
	CHECK(SelectReforgeAffixes(Item, { 3 }) == EReforgeError::None);

	const MaterialBundle Cost0 = GetReforgeCost(Item);
	CHECK(Cost0.SoulDust == 120 && Cost0.ObsidianOre == 20 && Cost0.PrimordialBlood == 2);

	MaterialBundle Poor = { Cost0.SoulDust - 1, Cost0.ObsidianOre, Cost0.PrimordialBlood };
	Pcg32 Rng(8);
	CHECK(ReforgeAffixes(Item, 3, Poor, 0, Pool, Rng).Error == EReforgeError::InsufficientMaterials);
	CHECK(Item.ReforgeCount == 0);

	MaterialBundle Wallet = RichWallet();
	ReforgeResult R = ReforgeAffixes(Item, 3, Wallet, 3, Pool, Rng);
	CHECK(R.Succeeded());
	CHECK(R.Spent.PrimordialBlood == Cost0.PrimordialBlood + 3);
	CHECK(Wallet.SoulDust == 100000 - Cost0.SoulDust);
	for (int C = 0; C < R.Session.NumChoices; ++C)
	{
		CHECK(R.Session.Choices[C].Quality >= 0.5f + 0.3f - 1e-5f); // Ancestral floor + 3 blood.
	}

	const MaterialBundle Cost1 = GetReforgeCost(Item);
	CHECK(Cost1.SoulDust == 150); // ceil(120 * 1.25)
}

TEST(Reforge_DeterministicForSameSeed)
{
	const AffixPool Pool = MakePool();
	ItemInstance A = MakeRare(Pool), B = MakeRare(Pool);
	SelectReforgeAffixes(A, { 0 });
	SelectReforgeAffixes(B, { 0 });
	MaterialBundle WA = RichWallet(), WB = RichWallet();
	Pcg32 RA(MixSeed(A.Seed, A.ReforgeCount)), RB(MixSeed(B.Seed, B.ReforgeCount));
	const ReforgeResult X = ReforgeAffixes(A, 0, WA, 0, Pool, RA);
	const ReforgeResult Y = ReforgeAffixes(B, 0, WB, 0, Pool, RB);
	for (int C = 0; C < ReforgeChoiceCount; ++C)
	{
		CHECK(X.Session.Choices[C] == Y.Session.Choices[C]);
	}
}

TEST(Reforge_PoolExhaustedChargesNothing)
{
	AffixPool Tiny;
	AffixDefinition D;
	D.Id = 1; D.Stat = EAffixStat::Strength; D.MinValue = 1; D.MaxValue = 2;
	Tiny.Add(D);
	D.Id = 2; D.Stat = EAffixStat::Armor;
	D.SlotMask = SlotBit(EItemSlot::Helm); // Not legal on the weapon.
	Tiny.Add(D);

	ItemInstance Item;
	Item.Guid = 1; Item.Rarity = EItemRarity::Rare; Item.Slot = EItemSlot::TwoHand; Item.ItemLevel = 10;
	Item.ExplicitAffixes = { AffixRoll{ 1, 1.5f, 0.5f, false } };
	CHECK(SelectReforgeAffixes(Item, { 0 }) == EReforgeError::None);

	MaterialBundle Wallet = RichWallet();
	Pcg32 Rng(1);
	ReforgeResult R = ReforgeAffixes(Item, 0, Wallet, 0, Tiny, Rng);
	CHECK(R.Succeeded());               // Same stat re-roll is a legal candidate.
	CHECK(R.Session.NumChoices == 1);   // Only one legal stat exists.

	AffixPool Empty;
	MaterialBundle W2 = RichWallet();
	CHECK(ReforgeAffixes(Item, 0, W2, 0, Empty, Rng).Error == EReforgeError::PoolExhausted);
	CHECK(W2 == RichWallet());
}

// ===========================================================================
// Aspects / Codex of Power
// ===========================================================================

TEST(Codex_ExtractKeepsBest_ImbueUpgradesRareToLegendary)
{
	AspectRegistry Reg;
	Reg.Add({ 501, WeaponSlotsMask, 10.f, 30.f, AnyClass, "Aspect of the Dark Young" });
	Reg.Add({ 502, SlotBit(EItemSlot::Helm), 1.f, 2.f, static_cast<u8>(1), "Chrono Helm Aspect" });

	CodexOfPower Codex;
	ItemInstance Leg;
	Leg.Rarity = EItemRarity::Legendary;
	Leg.Aspect = { 501, 15.f };
	bool bImproved = false;
	CHECK(Codex.ExtractAspect(Leg, Reg, bImproved) == ECodexError::None && bImproved);
	Leg.Aspect.Value = 12.f;
	CHECK(Codex.ExtractAspect(Leg, Reg, bImproved) == ECodexError::None && !bImproved);
	Leg.Aspect.Value = 99.f; // Out of range (tampered save) -> clamped.
	CHECK(Codex.ExtractAspect(Leg, Reg, bImproved) == ECodexError::None && bImproved);
	CHECK_NEAR(Codex.Find(501)->BestValue, 30.f, 1e-6f);
	CHECK(Codex.Find(501)->TimesExtracted == 3);

	ItemInstance RareNoAspect;
	RareNoAspect.Rarity = EItemRarity::Rare;
	CHECK(Codex.ExtractAspect(RareNoAspect, Reg, bImproved) == ECodexError::SourceNotLegendary);

	MaterialBundle Wallet = RichWallet();
	ItemInstance Target;
	Target.Rarity = EItemRarity::Rare;
	Target.Slot = EItemSlot::MainHand;
	CHECK(Codex.ImbueAspect(Target, 501, 0, Reg, Wallet) == ECodexError::None);
	CHECK(Target.Rarity == EItemRarity::Legendary);
	CHECK(Target.Aspect.AspectId == 501);
	CHECK(Codex.ImbueAspect(Target, 501, 0, Reg, Wallet) == ECodexError::InvalidTargetRarity); // Now Legendary.

	ItemInstance Ancestral;
	Ancestral.Rarity = EItemRarity::AncestralEldritch;
	Ancestral.Slot = EItemSlot::Chest;
	CHECK(Codex.ImbueAspect(Ancestral, 501, 0, Reg, Wallet) == ECodexError::SlotIncompatible);
	Ancestral.Slot = EItemSlot::TwoHand;
	CHECK(Codex.ImbueAspect(Ancestral, 501, 0, Reg, Wallet) == ECodexError::None);
	CHECK(Ancestral.Rarity == EItemRarity::AncestralEldritch);

	CHECK(Codex.ImbueAspect(Ancestral, 502, 0, Reg, Wallet) == ECodexError::NotInCodex);

	MaterialBundle Broke;
	ItemInstance Rare2;
	Rare2.Rarity = EItemRarity::Rare;
	CHECK(Codex.ImbueAspect(Rare2, 501, 0, Reg, Broke) == ECodexError::InsufficientMaterials);
	CHECK(Rare2.Rarity == EItemRarity::Rare);
}

// ===========================================================================
// EvaluateRuneword()
// ===========================================================================

namespace
{
	RuneDatabase MakeRunes()
	{
		RuneDatabase Db;
		RuneDefinition R;
		// Ritual: Ultimate used -> 100 offering
		R = {}; R.Id = 1; R.Type = ERuneType::Ritual; R.Condition = ERitualCondition::OnUltimateUsed; R.OfferingGenerated = 100; Db.Add(R);
		// Ritual: Heavy damage (>= 20% max life) -> 50 offering
		R = {}; R.Id = 2; R.Type = ERuneType::Ritual; R.Condition = ERitualCondition::OnHeavyDamageTaken; R.Threshold = 0.2f; R.OfferingGenerated = 50; Db.Add(R);
		// Ritual: CC applied -> 10 offering
		R = {}; R.Id = 3; R.Type = ERuneType::Ritual; R.Condition = ERitualCondition::OnCrowdControlApplied; R.OfferingGenerated = 10; Db.Add(R);
		// Effect: Time Stop 4s, cost 100, cd 10s
		R = {}; R.Id = 101; R.Type = ERuneType::Effect; R.Action = ERuneEffectAction::TriggerTimeStop; R.Param = 4.f; R.OfferingCost = 100; R.Cooldown = 10.f; Db.Add(R);
		// Effect: Flesh Colossus, cost 100
		R = {}; R.Id = 102; R.Type = ERuneType::Effect; R.Action = ERuneEffectAction::SpawnFleshColossus; R.Param = 12.f; R.OfferingCost = 100; Db.Add(R);
		// Effect: Reality Slash, cost 30, no cd
		R = {}; R.Id = 103; R.Type = ERuneType::Effect; R.Action = ERuneEffectAction::CastRealitySlash; R.Param = 1.f; R.OfferingCost = 30; Db.Add(R);
		return Db;
	}
}

TEST(Runeword_SocketingAndValidation)
{
	const RuneDatabase Db = MakeRunes();
	ItemInstance Weapon;
	Weapon.Guid = 1;
	Weapon.Sockets.resize(1);

	CHECK(InsertRune(Weapon, 3, 1, Db) == ESocketError::InvalidPairIndex);
	CHECK(InsertRune(Weapon, 0, 999, Db) == ESocketError::UnknownRune);
	CHECK(InsertRune(Weapon, 0, 101, Db) == ESocketError::None);   // Effect auto-routed.
	CHECK(Weapon.Sockets[0].EffectRuneId == 101);
	CHECK(InsertRune(Weapon, 0, 103, Db) == ESocketError::SlotOccupied);

	const RuneDefinition *R = nullptr, *E = nullptr;
	CHECK(ValidateRunewordPair(Weapon.Sockets[0], Db, R, E) == ERunewordValidity::Incomplete);
	CHECK(InsertRune(Weapon, 0, 1, Db) == ESocketError::None);
	CHECK(ValidateRunewordPair(Weapon.Sockets[0], Db, R, E) == ERunewordValidity::Valid);
	CHECK(R->Id == 1 && E->Id == 101);

	SocketPair Swapped{ 101, 1 }; // Corrupt save: types reversed.
	CHECK(ValidateRunewordPair(Swapped, Db, R, E) == ERunewordValidity::RitualSlotHoldsEffect);

	u32 Removed = 0;
	CHECK(RemoveRune(Weapon, 0, ERuneType::Effect, Removed) == ESocketError::None && Removed == 101);
	CHECK(RemoveRune(Weapon, 0, ERuneType::Effect, Removed) == ESocketError::SlotEmpty);
}

TEST(Runeword_TriggersBindsAndRespectsCooldown)
{
	const RuneDatabase Db = MakeRunes();
	ItemInstance Weapon;
	Weapon.Guid = 10;
	Weapon.Sockets = { SocketPair{ 1, 101 }, SocketPair{ 3, 103 } };
	ItemInstance Chest;
	Chest.Guid = 11;
	Chest.Sockets = { SocketPair{ 2, 102 } };

	RunewordEvaluator Eval;
	Eval.Rebuild({ &Weapon, &Chest }, Db);
	CHECK(Eval.GetBindings().size() == 3);
	CHECK(Eval.GetBindingCountFor(ERitualCondition::OnUltimateUsed) == 1);
	CHECK(Eval.GetBindingCountFor(ERitualCondition::OnEnemyKilled) == 0);

	std::vector<RuneEffectInvocation> Out;
	CHECK(Eval.EvaluateRuneword({ ERitualCondition::OnUltimateUsed, 1.f }, Out) == 1);
	CHECK(Out.back().Action == ERuneEffectAction::TriggerTimeStop);
	CHECK_NEAR(Out.back().Param, 4.f, 1e-6f);

	// On cooldown: offering banks but nothing fires.
	CHECK(Eval.EvaluateRuneword({ ERitualCondition::OnUltimateUsed, 1.f }, Out) == 0);
	Eval.Tick(10.f);
	CHECK(Eval.EvaluateRuneword({ ERitualCondition::OnUltimateUsed, 1.f }, Out) == 1); // Banked offering spent.

	// Heavy damage below threshold does nothing; above, two hits fund the Colossus.
	CHECK(Eval.EvaluateRuneword({ ERitualCondition::OnHeavyDamageTaken, 0.1f }, Out) == 0);
	CHECK(Eval.EvaluateRuneword({ ERitualCondition::OnHeavyDamageTaken, 0.25f }, Out) == 0);
	CHECK(Eval.EvaluateRuneword({ ERitualCondition::OnHeavyDamageTaken, 0.25f }, Out) == 1);
	CHECK(Out.back().Action == ERuneEffectAction::SpawnFleshColossus);
	CHECK(Out.back().ItemGuid == 11);

	// CC: 10 offering per event, cost 30 -> every third CC casts Reality Slash.
	int Slashes = 0;
	for (int I = 0; I < 9; ++I)
	{
		Slashes += Eval.EvaluateRuneword({ ERitualCondition::OnCrowdControlApplied, 1.f }, Out);
	}
	CHECK(Slashes == 3);
}

TEST(Runeword_RebuildPreservesStateAndDropsRemovedPairs)
{
	const RuneDatabase Db = MakeRunes();
	ItemInstance Weapon;
	Weapon.Guid = 10;
	Weapon.Sockets = { SocketPair{ 1, 101 } };
	ItemInstance Ring; // No sockets: irrelevant equipment change.
	Ring.Guid = 12;

	RunewordEvaluator Eval;
	Eval.Rebuild({ &Weapon }, Db);
	std::vector<RuneEffectInvocation> Out;
	CHECK(Eval.EvaluateRuneword({ ERitualCondition::OnUltimateUsed, 1.f }, Out) == 1);

	Eval.Rebuild({ &Weapon, &Ring }, Db); // Re-equip must not reset the cooldown.
	CHECK(Eval.GetBindings()[0].CooldownRemaining > 9.f);

	Weapon.Sockets[0].EffectRuneId = 0; // Rune removed.
	Eval.Rebuild({ &Weapon, &Ring }, Db);
	CHECK(Eval.GetBindings().empty());
	CHECK(Eval.EvaluateRuneword({ ERitualCondition::OnUltimateUsed, 1.f }, Out) == 0);
}

// ===========================================================================
// ApplyCrowdControlToBossStagger() + status pipeline
// ===========================================================================

TEST(Stagger_FillBreakRecover)
{
	StaggerConfig Cfg;
	StaggerState S;

	StatusApplication Burn{ EStatusEffect::Burn, 5.f, 10.f };
	CHECK(ApplyCrowdControlToBossStagger(S, Cfg, Burn, false).Result == EStaggerResult::NotCrowdControl);

	StatusApplication Stun{ EStatusEffect::Stun, 2.f, 0.f };   // 2s * 25 = 50
	StaggerOutcome O = ApplyCrowdControlToBossStagger(S, Cfg, Stun, false);
	CHECK(O.Result == EStaggerResult::Filled);
	CHECK_NEAR(O.Added, 50.f, 1e-4f);

	StatusApplication Freeze{ EStatusEffect::Freeze, 3.f, 0.f }; // 66 -> clamps at 100
	O = ApplyCrowdControlToBossStagger(S, Cfg, Freeze, false);
	CHECK(O.Result == EStaggerResult::Broke);
	CHECK_NEAR(O.Added, 50.f, 1e-4f); // Overflow discarded.
	CHECK(S.IsStaggered());
	CHECK(S.BreakCount == 1);
	CHECK(ApplyCrowdControlToBossStagger(S, Cfg, Stun, false).Result == EStaggerResult::AlreadyStaggered);

	CHECK(!TickStagger(S, Cfg, Cfg.StaggeredDuration - 0.1f));
	CHECK(TickStagger(S, Cfg, 0.2f)); // Recovery frame.
	CHECK(!S.IsStaggered());
	CHECK_NEAR(S.Current, 0.f, 1e-6f);
	CHECK_NEAR(S.GetMax(Cfg), 125.f, 1e-4f); // Max grew by 25%.
}

TEST(Stagger_DecayAfterDelay_AndUnstoppableScaling)
{
	StaggerConfig Cfg;
	StaggerState S;
	StatusApplication Stun{ EStatusEffect::Stun, 2.f, 0.f };
	ApplyCrowdControlToBossStagger(S, Cfg, Stun, false); // 50
	TickStagger(S, Cfg, Cfg.DecayDelay);                 // Still in grace.
	CHECK_NEAR(S.Current, 50.f, 1e-4f);
	TickStagger(S, Cfg, 1.f);                            // 1s of decay.
	CHECK_NEAR(S.Current, 50.f - Cfg.DecayPerSecond, 1e-3f);

	StaggerState U;
	const StaggerOutcome O = ApplyCrowdControlToBossStagger(U, Cfg, Stun, true);
	CHECK_NEAR(O.Added, 25.f, 1e-4f); // Halved while Unstoppable.

	StatusApplication Knock{ EStatusEffect::Knockback, 0.f, 1000.f };
	StaggerState K;
	CHECK_NEAR(ApplyCrowdControlToBossStagger(K, Cfg, Knock, false).Added, Cfg.KnockbackFlatFill, 1e-4f);
}

TEST(Pipeline_BossNeverDisabledUntilBreak_ThenStunnedAndVulnerable)
{
	StatusContainer Statuses;
	StaggerState Stagger;
	StaggerConfig Cfg;
	StatusTarget Boss{ Statuses, EMobTier::Boss, &Stagger, &Cfg };

	ApplyOutcome O = ApplyStatusEffect(Boss, { EStatusEffect::Freeze, 4.f, 0.f });
	CHECK(O.Result == EApplyResult::ConvertedToStagger);
	CHECK(O.Landed());
	CHECK(!Statuses.IsDisabled()); // CC did not disable the boss.

	O = ApplyStatusEffect(Boss, { EStatusEffect::Stun, 1.f, 0.f }); // 88 + 25 -> break
	CHECK(O.Result == EApplyResult::StaggerBroken);
	CHECK(Statuses.Has(EStatusEffect::Stun));
	CHECK(Statuses.IsDisabled());
	CHECK(Statuses.GetDamageTakenMultiplier() >= 1.2f);
	CHECK_NEAR(Statuses.Get(EStatusEffect::Stun).Remaining, Cfg.StaggeredDuration, 1e-4f);

	// Scripts can't purge the stagger window by going Unstoppable.
	CHECK(ApplyStatusEffect(Boss, { EStatusEffect::Unstoppable, 3.f, 0.f }).Result == EApplyResult::Rejected);
	CHECK(Statuses.IsDisabled());
}

TEST(Pipeline_UnstoppableTierScalingAndKnockbackImmunity)
{
	StatusContainer Trash, Captain, Brute;
	StatusTarget T{ Trash, EMobTier::TrashSwarmer };
	StatusTarget C{ Captain, EMobTier::Captain };
	StatusTarget B{ Brute, EMobTier::PrimordialBrute };

	CHECK_NEAR(ApplyStatusEffect(T, { EStatusEffect::Stun, 2.f, 0.f }).EffectiveDuration, 2.f, 1e-6f);
	CHECK_NEAR(ApplyStatusEffect(C, { EStatusEffect::Stun, 2.f, 0.f }).EffectiveDuration, 1.5f, 1e-6f);
	CHECK_NEAR(ApplyStatusEffect(B, { EStatusEffect::Stun, 2.f, 0.f }).EffectiveDuration, 1.f, 1e-6f);
	CHECK(ApplyStatusEffect(B, { EStatusEffect::Knockback, 0.f, 500.f }).Result == EApplyResult::ResistedByImmunity);

	// Unstoppable purges CC and blocks new CC, but not debuffs/DoTs.
	CHECK(ApplyStatusEffect(T, { EStatusEffect::Unstoppable, 3.f, 0.f }).Result == EApplyResult::Applied);
	CHECK(!Trash.Has(EStatusEffect::Stun));
	CHECK(ApplyStatusEffect(T, { EStatusEffect::Freeze, 2.f, 0.f }).Result == EApplyResult::ResistedByImmunity);
	CHECK(ApplyStatusEffect(T, { EStatusEffect::Burn, 2.f, 5.f }).Landed());
}

TEST(Status_DotStackingVulnerableAndSlow)
{
	StatusContainer S;
	S.ApplyRaw({ EStatusEffect::Bleed, 4.f, 10.f });
	CHECK(S.ApplyRaw({ EStatusEffect::Bleed, 4.f, 10.f }) == EApplyResult::Stacked);
	S.ApplyRaw({ EStatusEffect::Burn, 1.f, 20.f });
	S.ApplyRaw({ EStatusEffect::Vulnerable, 3.f, 0.05f }); // Below contract -> clamped to 20%.
	S.ApplyRaw({ EStatusEffect::Slow, 3.f, 0.4f });

	CHECK_NEAR(S.GetDamageTakenMultiplier(), 1.2f, 1e-6f);
	CHECK_NEAR(S.GetMoveSpeedScale(), 0.6f, 1e-6f);

	// 2 bleed stacks * 10 dps * 2s + burn 20 dps * 1s (expires mid-step).
	const float Dot = S.Tick(2.f);
	CHECK_NEAR(Dot, 40.f + 20.f, 1e-3f);
	CHECK(!S.Has(EStatusEffect::Burn));
	CHECK(S.Has(EStatusEffect::Bleed));

	for (int I = 0; I < 20; ++I)
	{
		S.ApplyRaw({ EStatusEffect::Bleed, 4.f, 10.f });
	}
	CHECK(S.Get(EStatusEffect::Bleed).Stacks == StatusContainer::MaxBleedStacks);
}

// ===========================================================================
// Damage math + spatial hash
// ===========================================================================

TEST(Damage_ArmorPenetrationAndExecute)
{
	// 1000 armor -> 50% mitigation; Reality Slash ignores 50% -> 500 armor -> 33.3%.
	CHECK_NEAR(ComputeFinalDamage(300.f, 1000.f, 0.f, 1.f), 150.f, 1e-3f);
	CHECK_NEAR(ComputeFinalDamage(300.f, 1000.f, 0.5f, 1.f), 200.f, 1e-3f);
	CHECK_NEAR(ComputeFinalDamage(100.f, 0.f, 0.f, 1.2f), 120.f, 1e-3f);
	CHECK_NEAR(ComputeArmorMitigation(1e9f, 0.f), 0.85f, 1e-6f);
	CHECK_NEAR(ComputeLinkedDamageShare(100.f, 0.7f), 70.f, 1e-4f);

	CHECK(ShouldExecute(100.f, 100.f, EMobTier::TrashSwarmer, 0.2f));
	CHECK(!ShouldExecute(50.f, 100.f, EMobTier::Captain, 0.2f));
	CHECK(ShouldExecute(15.f, 100.f, EMobTier::Captain, 0.2f));
	CHECK(!ShouldExecute(15.f, 100.f, EMobTier::PrimordialBrute, 0.2f));
	CHECK(!ShouldExecute(1.f, 100.f, EMobTier::Boss, 1.f));
}

TEST(SpatialHash_MatchesBruteForce)
{
	Pcg32 Rng(2024);
	const u32 N = 600;
	std::vector<float> X(N), Y(N);
	std::vector<bool> Alive(N);
	for (u32 I = 0; I < N; ++I)
	{
		X[I] = Rng.Range(-8000.f, 8000.f);
		Y[I] = Rng.Range(-8000.f, 8000.f);
		Alive[I] = (I % 7) != 0; // Some dead slots.
	}

	SpatialHashGrid2D Grid(400.f, 256); // Small table forces bucket collisions.
	Grid.Build(N, [&](u32 I, float& OX, float& OY) { OX = X[I]; OY = Y[I]; return static_cast<bool>(Alive[I]); });

	for (int Q = 0; Q < 200; ++Q)
	{
		const float QX = Rng.Range(-8000.f, 8000.f), QY = Rng.Range(-8000.f, 8000.f);
		const float R = Rng.Range(50.f, 3000.f);
		std::multiset<u32> FromGrid;
		Grid.QueryRadius(QX, QY, R, [&](const SpatialHashGrid2D::Entry& E, float) { FromGrid.insert(E.Index); });

		std::multiset<u32> Brute;
		for (u32 I = 0; I < N; ++I)
		{
			const float DX = X[I] - QX, DY = Y[I] - QY;
			if (Alive[I] && DX * DX + DY * DY <= R * R)
			{
				Brute.insert(I);
			}
		}
		CHECK(FromGrid == Brute); // Exact, and no duplicates despite collisions.
	}
}

// ---------------------------------------------------------------------------

int main()
{
	for (auto& [Name, Fn] : Registry())
	{
		const int Before = GFailures;
		Fn();
		std::printf("%s %s\n", GFailures == Before ? "[PASS]" : "[FAIL]", Name.c_str());
	}
	std::printf("\n%d checks, %d failures\n", GChecks, GFailures);
	return GFailures == 0 ? 0 : 1;
}
