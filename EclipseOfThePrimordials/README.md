# Eclipse of the Primordials — Gameplay Architecture

Unreal Engine 5.4 · C++ · Gameplay Ability System (GAS) · dedicated-server authoritative

This folder holds the gameplay code base for the four core pillars:

1. A high-density swarm system for mobs, with boss stagger and status effects.
2. Item generation, dual-affix reforging, and the Codex of Power (aspects).
3. Runeword socketing with dynamic Ritual → Effect binding.
4. The four classes and their abilities.

> **Build status.** The engine-agnostic **EclipseCore** module compiles and its unit tests pass. They have been run with GCC 13 (`-Wall -Wextra -Wpedantic -Werror`), Clang 18, and ASan/UBSan, with 19 tests and 1,944 checks. The **EclipseGame** (UE/GAS) layer is written against UE 5.4 APIs but has **not** been compiled in this environment, because no engine is installed here. Expect a short fix-up pass on the first UBT build.

---

## 1. Folder & class file layout

```
EclipseOfThePrimordials/
├── EclipseOfThePrimordials.uproject
├── Config/DefaultGame.ini                     UEclipseGameSettings defaults
├── Tests/
│   ├── CoreTests.cpp                          standalone unit tests (no engine)
│   └── Makefile                               `make test`
└── Source/
    ├── EclipseOfThePrimordials{,Editor,Server}.Target.cs
    │
    ├── EclipseCore/                           ── PURE C++17 RULES (no UObject) ──
    │   └── Public/EclipseCore/
    │       ├── EclipseCoreTypes.h             ints, Clamp/Min/Max, export macro
    │       ├── Random.h                       PCG32 + SplitMix seed mixing
    │       ├── StatusEffects.h/.cpp           EStatusEffect, StatusContainer,
    │       │                                  Stagger Meter, ApplyStatusEffect() pipeline,
    │       │                                  ApplyCrowdControlToBossStagger()
    │       ├── DamageMath.h/.cpp              armor/penetration, execute rule, link share
    │       ├── SpatialHash.h                  counting-sort 2D hash grid (swarm queries)
    │       ├── Items.h/.cpp                   ItemInstance, rarity, AffixPool, GenerateItem()
    │       ├── Reforge.h/.cpp                 SelectReforgeAffixes(), ReforgeAffixes(),
    │       │                                  CommitReforgeChoice()
    │       ├── Aspects.h/.cpp                 AspectRegistry, CodexOfPower (extract / imbue)
    │       ├── Runewords.h/.cpp               runes, sockets, RunewordEvaluator,
    │       │                                  EvaluateRuneword()
    │       └── ClassIds.h                     EClassId, resource models
    │
    └── EclipseGame/                           ── UNREAL / GAS LAYER ──
        ├── Public/
        │   ├── EclipseGameplayTags.h          native tags (Event.Ritual.*, Status.*, Data.*)
        │   ├── EclipseGameSettings.h          UDeveloperSettings
        │   ├── Combat/
        │   │   ├── EclipseCombatTypes.h       UENUM/USTRUCT mirrors (static_assert-synced)
        │   │   ├── EclipseAttributeSet.h      Health, Armor, ClassResource, IncomingDamage
        │   │   ├── EclipseDamageExecution.h   GE execution → core ComputeFinalDamage
        │   │   ├── EclipseStatusComponent.h   actor statuses (heroes, bosses)
        │   │   ├── EclipseStaggerComponent.h  boss Stagger Meter (replicated view)
        │   │   └── EclipseCombatStatics.h     HitSphere / HitLine / HitMobs choke point
        │   ├── Swarm/
        │   │   ├── EclipseSwarmTypes.h        FEclipseMobHandle, UEclipseMobArchetype, auras
        │   │   ├── EclipseSwarmSubsystem.h    SoA simulation, grid, LOD, ISM rendering
        │   │   ├── EclipseSwarmNetProxy.h     FastArray snapshot replication
        │   │   └── EclipseSwarmSpawner.h      level-placed pack seeding
        │   ├── Items/
        │   │   ├── EclipseItemTypes.h         FEclipseItemData (= FItemData) + converters
        │   │   ├── EclipseItemDatabase.h      affix / aspect / rune rows → core tables
        │   │   ├── EclipseInventoryComponent.h owner-only FastArray inventory, sockets
        │   │   └── EclipseForgeComponent.h    reforge + Codex RPCs (server authority)
        │   ├── Runes/EclipseRunewordComponent.h  dynamic GAS event ↔ effect binding
        │   ├── Classes/EclipseClassDefinition.h  per-class data asset
        │   ├── Characters/                    Hero, Boss, PlayerState
        │   ├── Abilities/
        │   │   ├── EclipseGameplayAbility.h   base: cost models, runeword bypass, helpers
        │   │   ├── ArchitectOfFleshAbilities.h
        │   │   ├── ChronoSovereignAbilities.h
        │   │   ├── SpireWardenAbilities.h
        │   │   └── EclipsePuppeteerAbilities.h
        │   └── World/
        │       ├── EclipseGroundZone.h        obelisks, scorch zones
        │       ├── EclipseFieldAffinitySubsystem.h
        │       └── EclipseCreationDomain.h    "Creation" affinity override
        └── Private/ (mirrors Public) + Tests/EclipseCoreAutomationTests.cpp
```

### Why two modules?

All gameplay **rules** live in `EclipseCore` as pure functions of explicit state and an explicit RNG. `EclipseGame` only adapts them to Unreal. That split has three benefits:

* **Testability.** The reforge, runeword and stagger rules run in a 0.1 s test binary, with no editor boot.
* **Authority by construction.** The UE layer calls these rules only on the server. Clients only ever receive results.
* **Determinism.** The same seed and inputs produce the same output, so loot and forge exploits can be audited from logs.

UE↔core enum mirrors are guarded by `static_assert`s. If someone reorders an enum on one side, the build fails.

---

## 2. High-density mob & swarm system

```
UEclipseSwarmSubsystem (UTickableWorldSubsystem, server)
  SoA columns: Position[] Velocity[] Impulse[] Health[] Statuses[] ArchetypeIndex[] Serial[] ...
  ┌────────────────────────────── frame ───────────────────────────────┐
  │ GatherPlayers → RebuildGrid → UpdateAuras(4 Hz) → SimulateStatuses │
  │ → SimulateMovement (ParallelFor, distance-LOD) → ResolveAttacks    │
  │   (1 batched GE per player) → UpdateLinkGroups → ProcessDeaths     │
  │ → PushNetSnapshot (10 Hz) → UpdateRendering (ISM, non-dedicated)   │
  └─────────────────────────────────────────────────────────────────────┘
        │ FastArray (changed items only)
        ▼
AEclipseSwarmNetProxy ──► clients interpolate ──► same ISM render path
```

| Concern | Approach |
|---|---|
| CPU | Mobs are SoA rows, not actors. Movement runs in `ParallelFor`; each iteration writes only its own row and reads neighbours from the grid's private copy, so there are no data races. Mobs far from players are time-sliced: every frame within 2.5k, at 30 Hz within 6k, and at 10 Hz beyond that. |
| Queries | `Eclipse::SpatialHashGrid2D` is rebuilt each frame in O(N) by counting sort. Collisions are deduped, and an exact distance test filters results. A test checks it against brute force. |
| Memory | Columns are reserved to `SwarmSimHardCap` at BeginPlay, so the steady state makes zero allocations. Slots are recycled through a free list with **generational handles**, so stale handles fail safely. Freed slots are reused only after the next grid rebuild. Archetypes are flyweights. |
| Rendering | One `UInstancedStaticMeshComponent` per archetype, with VAT materials for animation. A grow-only instance pool hides unused instances by setting their scale to zero, which avoids GPU buffer reshuffles. |
| Network | The server pushes quantized snapshots (about 14 B per changed mob) at 10 Hz, and clients interpolate. The header documents an upgrade path to per-cell relevancy and an Iris bit-packer. |

**Mob tiers** (`EEclipseMobTier`, defined per `UEclipseMobArchetype`):

| Tier | CC duration | Special |
|---|---|---|
| Trash Swarmer | 100 % | Always executable |
| Captain / Elite | 75 % | `bProjectsAura`: damage bonus, damage reduction and speed to nearby trash. The strongest aura wins; auras don't stack, and captains don't buff each other. |
| Primordial Brute | 50 % | Immune to knockback. Executable below half the normal threshold. |
| Boss (actor + GAS) | — | **CC never disables a boss.** It fills the Stagger Meter instead. |

**Stagger Meter** (`ApplyCrowdControlToBossStagger`). Each CC effect fills the meter at a per-type rate per second of intended duration: Freeze 22, Stun 25, Slow 6, and Knockback a flat 12. An Unstoppable boss fills at 50 %. When the meter is full, the boss **breaks**: it is forced into Stun and gets +30 % Vulnerable for `StaggeredDuration` (N seconds). The maximum then grows by 25 % per break. The meter decays after a 2.5 s grace period. Boss scripts cannot purge an active break by going Unstoppable.

**Statuses**:

| Status | Category | Behaviour |
|---|---|---|
| Vulnerable | Debuff | At least +20 % damage taken (clamped) |
| Unstoppable | Buff | Purges CC on apply and blocks new CC |
| Bleed | DoT | Intensity stacks, up to 10 |
| Burn | DoT | Refreshes; the strongest application wins |
| Freeze, Stun | CC | Full disable |
| Slow | CC | Reduces movement speed |
| Knockback | CC | Impulse |

Every application, for heroes, mobs and bosses, goes through **one** pipeline: `Eclipse::ApplyStatusEffect`.

---

## 3. Items, dual-affix reforging & aspects

`FEclipseItemData` (aliased as `FItemData`) is the replicated snapshot. `Eclipse::ItemInstance` is the type the rules operate on. Rarity tiers are Common, Magic, Rare, Legendary and Ancestral Eldritch. Ancestral items have a 0.5 quality floor and at least one guaranteed greater affix at ×1.5.

```
Client UI            Server (UEclipseForgeComponent)                     Core
─────────            ───────────────────────────────                     ────
Select {i,j} ──RPC──► validate ─► SelectReforgeAffixes  (write-once mask, ≤2)
Reroll i     ──RPC──► rate-limit ─► ReforgeAffixes(Item,i,Wallet,Blood,Pool,
                       Pcg32(MixSeed(Seed^ServerSalt, ReforgeCount)))
                       ─► charge wallet, ++ReforgeCount, store session (server-only)
             ◄─RPC─── Client_ReforgeOffer(3 candidates)
Pick k|-1    ──RPC──► CommitReforgeChoice(Item, Session, k)   (tamper-checked)
```

Invariants are enforced in `Reforge.cpp` and tested in `Tests/CoreTests.cpp`:

* Non-selected affixes are bit-identical before and after any reroll.
* The three candidates are distinct stats, and none duplicates another affix on the item.
* Materials are charged once, only after every validation passes. An exhausted pool charges nothing.
* A pending session is invalidated if the item changes underneath it. An invalid index does not forfeit the paid session.
* Primordial Blood catalyst raises the candidates' quality floor by 10 % per unit, up to 3 units.

**Codex of Power.** `ExtractAspect` consumes a Legendary or Ancestral item and keeps the best value per aspect. Values are clamped to the data range, which defends against tampered saves. `ImbueAspect` upgrades **Rare → Legendary**. An **Ancestral** item keeps its rarity and gains or replaces an aspect. Imbuing checks slot and class restrictions.

---

## 4. Runewords

* Each socket **pair** holds an `ERuneType::Ritual` (condition) and an `ERuneType::Effect` (action). Weapons and armor get one pair; two-handers get two. `InsertRune` routes a rune to the correct half based on its type.
* `RunewordEvaluator` is event-driven. It rebuilds only when equipment or sockets change, then dispatches each event in O(bindings for that condition).
* Rituals generate **Offering**. An effect fires when its pair's bank covers its cost and its cooldown is ready. The bank is capped at twice the cost.
* **Dynamic binding.** `UEclipseRunewordComponent` subscribes `ASC->GenericGameplayEventCallbacks[Event.Ritual.X]` only for conditions that currently have a binding, and unsubscribes when the last one goes. It grants a dedicated spec for each effect ability, with `SourceObject` set to the component; that spec skips cost and cooldown. It then triggers the spec with `EventMagnitude = Param`, for example `TriggerTimeStop(Duration)`.

| Ritual condition | Raised by |
|---|---|
| `OnUltimateUsed` | `UEclipseGameplayAbility` (`bIsUltimate`) |
| `OnCrowdControlApplied` | `EclipseCombat::Hit*` (one event per batch) |
| `OnHeavyDamageTaken` | `UEclipseAttributeSet` (hit ≥ 20 % max HP) |
| `OnEnemyKilled` | `UEclipseSwarmSubsystem::ProcessDeaths` |
| `OnResourceSpent` | `UEclipseGameplayAbility::ApplyCost` |

| Effect action | Ability |
|---|---|
| `TriggerTimeStop` | `UGA_TimeStop` |
| `SpawnFleshColossus` | `UGA_SpawnFleshColossus` |
| `CastRealitySlash` | `UGA_RealitySlash` |
| `GainUnstoppable` | `UGA_GainUnstoppable` |

Map these in `UEclipseItemDatabase::RuneEffectAbilities`. A re-entrancy guard stops a runeword from triggering itself within the same call stack, and offering costs plus cooldowns bound chains that span frames.

---

## 5. Classes

| Class | Resource (model) | Abilities |
|---|---|---|
| Architect of Flesh | Biomass (Builder) | `UGA_GraspingHeart`: cone-target execute-or-stun; a kill grants Biomass. **Ult** `UGA_IaMshubNiggurath`: culls trash in radius, heavily damages everything else, spawns 3 Dark Young. |
| Chrono-Sovereign | Temporal Strain (Strain: casting *adds* strain) | `UGA_TimeStop`: AoE Freeze, clamped to 3–5 s; bosses get stagger instead. `UGA_RealitySlash`: line AoE with 50 % armor penetration. **Ult** `UGA_GoalOfAllLifeIsDeath`: marks the area (Vulnerable), then after 12 s a fatal wave kills all non-boss mobs and deals 20 % max HP to bosses. Cancelled if the caster dies. |
| Spire Warden | Seismic Pressure (Builder) | `UGA_GrandCatastrophe`: slam, knockback through the status pipeline, and a ring of magmatic obelisks that pulse Burn. **Ult** `UGA_FallenDown`: telegraphed orbital beam that leaves a scorch zone. |
| Eclipse Puppeteer | Lunar Essence (Spender) | `UGA_PuppetMastersThread`: links up to 5 enemies so damage is mirrored at 70 % to each other member; mirrored damage is never re-shared. **Ult** `UGA_Creation`: replicated domain that overrides field affinity (resonant damage ×1.5, no opposition). |

Each class is a `UEclipseClassDefinition` data asset: abilities, input tags, resource model, max resource, passive regen/decay effects and base stats. The asset is set on `AEclipsePlayerState` and granted in `AEclipseHeroCharacter::PossessedBy`.

---

## 6. Server authority & network safety summary

| System | Client can… | Server does… | Replicated as |
|---|---|---|---|
| Swarm | nothing (render only) | simulate, damage, kill, loot | FastArray snapshot, 10 Hz |
| Statuses | nothing | pipeline, DoTs, stagger | 16-bit mask + speed scale → loose tags |
| Stagger | nothing | meter math | current, max, staggered (3 values) |
| Abilities | predict activation, montages, cues | all damage/status/summons (`IsServer()` gates) | GAS |
| Inventory | send ids | everything | owner-only FastArray; `Seed` is `NotReplicated` |
| Reforge | send ids, indices, choice index | RNG with secret per-player salt, session storage | owner-only item + `Client_ReforgeOffer` |
| Runewords | nothing | bind/evaluate/trigger | owner-only UI view |

---

## 7. Build & test

```bash
# Engine-free rule tests (seconds):
cd Tests && make test

# In-engine automation mirror:
UnrealEditor-Cmd EclipseOfThePrimordials.uproject -ExecCmds="Automation RunTests Eclipse.Core; Quit" -unattended -nullrhi
```

### Assets designers must create

* **`GE_Damage`.** An instant effect whose execution is `UEclipseDamageExecution`. Assign it to `UEclipseGameSettings::MobDamageEffect`, to each ability's `DamageEffect`, and to `StatusComponent.DotDamageEffect`.
* **`GE_ResourceDelta`.** Instant, with an Add modifier on `ClassResource` set by caller with `Data.ResourceDelta`.
* **`DA_ItemDatabase`.** Affix, aspect and rune rows, plus the `RuneEffectAbilities` map.
* **Class assets.** `DA_Class_*` (four assets), `DA_Mob_*` archetypes, and BP subclasses of the ground zones (obelisk, scorch) and summons.
