// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// In-engine mirror of Tests/CoreTests.cpp for the three mechanisms the design
// calls out as unit-testable. Run with:
//   UnrealEditor-Cmd <Project> -ExecCmds="Automation RunTests Eclipse.Core; Quit" -unattended -nullrhi
// The standalone suite (Tests/Makefile) covers the same rules plus edge cases
// without booting the engine and is what CI runs on every commit.

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "EclipseCore/Reforge.h"
#include "EclipseCore/Runewords.h"
#include "EclipseCore/StatusEffects.h"

namespace
{
	Eclipse::AffixPool MakeTestPool()
	{
		Eclipse::AffixPool Pool;
		for (Eclipse::u32 S = 0; S < Eclipse::AffixStatCount; ++S)
		{
			Eclipse::AffixDefinition D;
			D.Id = S + 1;
			D.Stat = static_cast<Eclipse::EAffixStat>(S);
			D.MinValue = 10.f;
			D.MaxValue = 20.f;
			Pool.Add(D);
		}
		return Pool;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEclipseReforgeAffixesTest, "Eclipse.Core.ReforgeAffixes",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEclipseReforgeAffixesTest::RunTest(const FString& Parameters)
{
	const Eclipse::AffixPool Pool = MakeTestPool();
	Eclipse::Pcg32 GenRng(42);
	Eclipse::ItemGenerationParams P;
	P.Guid = 1;
	P.Rarity = Eclipse::EItemRarity::Rare;
	P.Slot = Eclipse::EItemSlot::TwoHand;
	P.ItemLevel = 800;
	Eclipse::ItemInstance Item = Eclipse::GenerateItem(P, Pool, GenRng);

	TestEqual(TEXT("Select 2 affixes"), (int32)Eclipse::SelectReforgeAffixes(Item, { 0, 2 }), (int32)Eclipse::EReforgeError::None);
	TestEqual(TEXT("Selection is write-once"), (int32)Eclipse::SelectReforgeAffixes(Item, { 1 }), (int32)Eclipse::EReforgeError::SelectionAlreadyLocked);

	Eclipse::MaterialBundle Wallet{ 1000, 1000, 1000 };
	Eclipse::Pcg32 Rng(7);
	TestEqual(TEXT("Locked affix rejected"), (int32)Eclipse::ReforgeAffixes(Item, 1, Wallet, 0, Pool, Rng).Error,
		(int32)Eclipse::EReforgeError::AffixNotSelected);

	const Eclipse::AffixRoll Locked = Item.ExplicitAffixes[1];
	Eclipse::ReforgeResult R = Eclipse::ReforgeAffixes(Item, 0, Wallet, 0, Pool, Rng);
	TestTrue(TEXT("Reforge succeeded"), R.Succeeded());
	TestEqual(TEXT("Three choices"), (int32)R.Session.NumChoices, Eclipse::ReforgeChoiceCount);
	TestEqual(TEXT("Commit"), (int32)Eclipse::CommitReforgeChoice(Item, R.Session, 1), (int32)Eclipse::EReforgeError::None);
	TestTrue(TEXT("Non-selected affix untouched"), Item.ExplicitAffixes[1] == Locked);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEclipseEvaluateRunewordTest, "Eclipse.Core.EvaluateRuneword",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEclipseEvaluateRunewordTest::RunTest(const FString& Parameters)
{
	Eclipse::RuneDatabase Db;
	Eclipse::RuneDefinition Ritual;
	Ritual.Id = 1;
	Ritual.Type = Eclipse::ERuneType::Ritual;
	Ritual.Condition = Eclipse::ERitualCondition::OnUltimateUsed;
	Ritual.OfferingGenerated = 100;
	Db.Add(Ritual);

	Eclipse::RuneDefinition Effect;
	Effect.Id = 2;
	Effect.Type = Eclipse::ERuneType::Effect;
	Effect.Action = Eclipse::ERuneEffectAction::TriggerTimeStop;
	Effect.Param = 4.f;
	Effect.OfferingCost = 100;
	Effect.Cooldown = 10.f;
	Db.Add(Effect);

	Eclipse::ItemInstance Weapon;
	Weapon.Guid = 9;
	Weapon.Sockets = { Eclipse::SocketPair{ 1, 2 } };

	Eclipse::RunewordEvaluator Eval;
	Eval.Rebuild({ &Weapon }, Db);
	std::vector<Eclipse::RuneEffectInvocation> Out;

	TestEqual(TEXT("Ultimate triggers Time Stop"), Eval.EvaluateRuneword({ Eclipse::ERitualCondition::OnUltimateUsed, 1.f }, Out), 1);
	TestEqual(TEXT("Duration param"), Out[0].Param, 4.f);
	TestEqual(TEXT("Cooldown blocks"), Eval.EvaluateRuneword({ Eclipse::ERitualCondition::OnUltimateUsed, 1.f }, Out), 0);
	TestEqual(TEXT("Unrelated condition"), Eval.EvaluateRuneword({ Eclipse::ERitualCondition::OnEnemyKilled, 1.f }, Out), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEclipseBossStaggerTest, "Eclipse.Core.ApplyCrowdControlToBossStagger",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEclipseBossStaggerTest::RunTest(const FString& Parameters)
{
	Eclipse::StaggerConfig Cfg;
	Eclipse::StaggerState State;
	Eclipse::StatusContainer Statuses;
	Eclipse::StatusTarget Boss{ Statuses, Eclipse::EMobTier::Boss, &State, &Cfg };

	Eclipse::StatusApplication Freeze;
	Freeze.Effect = Eclipse::EStatusEffect::Freeze;
	Freeze.Duration = 4.f; // 88 stagger

	TestEqual(TEXT("CC converts to stagger"), (int32)Eclipse::ApplyStatusEffect(Boss, Freeze).Result,
		(int32)Eclipse::EApplyResult::ConvertedToStagger);
	TestFalse(TEXT("Boss not disabled by CC"), Statuses.IsDisabled());

	TestEqual(TEXT("Second CC breaks"), (int32)Eclipse::ApplyStatusEffect(Boss, Freeze).Result,
		(int32)Eclipse::EApplyResult::StaggerBroken);
	TestTrue(TEXT("Stunned while staggered"), Statuses.Has(Eclipse::EStatusEffect::Stun));
	TestTrue(TEXT("Vulnerable >= +20%"), Statuses.GetDamageTakenMultiplier() >= 1.2f);

	TestTrue(TEXT("Recovers after N seconds"), Eclipse::TickStagger(State, Cfg, Cfg.StaggeredDuration + 0.01f));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
