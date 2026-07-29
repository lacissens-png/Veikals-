# Supreme Overlord — Core Framework Setup

This branch scaffolds the initial C++ framework for an isometric ARPG in Unreal Engine 5.8:

- `ASOGameMode` — top-down ARPG game mode wiring the pawn and controller.
- `ASOCharacter` — pawn with a fixed isometric spring-arm/camera and exposed tuning knobs.
- `ASOPlayerController` — click-to-move controller using the navigation system, with a decal indicator.

## 1. Generate the project files

The repo is set up as a full Unreal project. From the repository root:

**Windows**

```
Right-click SupremeOverlord.uproject > "Generate Visual Studio project files"
```

**macOS / Linux**

```
"/path/to/UnrealEngine/Engine/Build/BatchFiles/{Mac|Linux}/GenerateProjectFiles.sh" \
    -project="$PWD/SupremeOverlord.uproject" -game -engine
```

If you do not yet have a `Content/` folder, launch `SupremeOverlord.uproject`
once through the launcher — the editor will create the required empty
directories and prompt to build the missing modules.

## 2. Compile

- Open the generated solution/workspace.
- Set configuration to **Development Editor** and platform to your host.
- Build the `SupremeOverlord` target (or just double-click the `.uproject`; the
  editor will build on first launch and prompt if compilation fails).

## 3. Create a test map with a NavMesh

1. `File > New Level` — pick **Basic** (gives you a floor + light).
2. Save as `Content/Maps/L_TestArena.umap`.
3. In the **Place Actors** panel, search for **NavMeshBoundsVolume** and drop
   one into the scene.
4. Scale the NavMeshBoundsVolume (`R`) so it fully covers the floor plane
   you want the character to move on (typical starting scale: `X=40 Y=40 Z=4`).
5. Press **`P`** to visualize the navmesh — you should see a green overlay
   covering the walkable area. If it is missing, verify `bAutoCreateNavigationData=True`
   in `Config/DefaultEngine.ini` and that `RecastNavMesh` was auto-spawned in the World Outliner.
6. Under **World Settings**, leave *GameMode Override* empty — the project
   default (`SOGameMode`) will be used because it is set in `DefaultEngine.ini`.
7. Set this map as the editor default: `Project Settings > Maps & Modes >
   Editor Startup Map` and `Game Default Map`.

## 4. (Optional) Create a click decal material

The controller spawns a decal at the click location if `ClickDecalMaterial` is
assigned:

1. Create a material `M_ClickIndicator` in `Content/UI/`.
2. Set **Material Domain** to **Deferred Decal** and **Blend Mode** to
   **Translucent**.
3. Plug a bright emissive color + a radial gradient opacity mask into it.
4. Create a Blueprint subclass of `ASOPlayerController`
   (`Content/Blueprints/BP_SOPlayerController`) and set `ClickDecalMaterial =
   M_ClickIndicator`, then point `SOGameMode` at that BP subclass (see step 5).

Skipping this step is fine — the click-to-move will still work, you just
won't see the ground indicator.

## 5. Wire the classes to the GameMode

`DefaultEngine.ini` already sets:

```
[/Script/EngineSettings.GameMapsSettings]
GlobalDefaultGameMode=/Script/SupremeOverlord.SOGameMode
```

If you create Blueprint subclasses (recommended for tweaking), point the game
mode at them:

- `BP_SOGameMode` (Parent: `SOGameMode`)
  - Default Pawn Class: `BP_SOCharacter`
  - Player Controller Class: `BP_SOPlayerController`
- Then either:
  - Set the map's **World Settings > GameMode Override** to `BP_SOGameMode`, or
  - Change `GlobalDefaultGameMode` in `DefaultEngine.ini` to your BP path.

## 6. Input

`Config/DefaultInput.ini` binds a `MoveTo` action to both the **Left** and
**Right** mouse buttons. Change or extend those bindings under
`Project Settings > Input > Bindings > Action Mappings > MoveTo` if you want
different keys or gamepad support.

Because the controller uses the legacy input action binding
(`InputComponent->BindAction("MoveTo", ...)`), it works out of the box without
requiring an Enhanced Input asset. You can migrate to Enhanced Input later
without touching the movement logic — only `SetupInputComponent` needs to change.

## 7. Play

Hit **Play** in the editor. Click (or hold) anywhere on the NavMesh; the
character should path to the click location and the decal (if assigned) should
flash on the ground.

## Tweakables

All key values are `UPROPERTY(EditAnywhere, BlueprintReadWrite)`, so you can
tune them per-instance in a Blueprint subclass or per-actor in the editor:

**`ASOCharacter`**
- `CameraDistance` — spring arm length (default 1200).
- `CameraPitch` — downward tilt in degrees (default 55).
- `CameraYaw` — locked world yaw (default -45).
- `MovementSpeed` — max walk speed (default 600).
- `RotationRateYaw` — how fast the character faces its move direction.

**`ASOPlayerController`**
- `ClickDecalMaterial` — decal drawn at the click location.
- `ClickParticleFX` — optional particle spawned alongside the decal.
- `ClickDecalSize` — footprint of the decal (X thickness, Y/Z ground size).
- `ClickDecalLifetime` — how long the decal stays before fading.
- `ClickTraceChannel` — collision channel used for the cursor trace.
- `DebugDamageAmount` — HP subtracted from the pawn every time `K` is pressed.

## 8. Health, damage, and death

The character now carries a `USOHealthComponent` (created in
`ASOCharacter`'s constructor). Anything that calls
`UGameplayStatics::ApplyDamage` on the character will funnel through it.

- **Test it in play:** press **`K`** to apply `DebugDamageAmount` damage to
  yourself. When HP hits 0 the character stops accepting move orders,
  disables its capsule collision, and freezes movement. `OnDeath` fires so
  Blueprint / UI can react (e.g. show a "You Died" screen later).
- **Tune HP:** on `BP_SOCharacter`, expand *SupremeOverlord | Health* and
  edit `Max Health`, `Starting Health`, `Incoming Damage Multiplier`, or
  `Invulnerable`.
- **Custom damage types:** subclass `USODamageType` in Blueprint
  (`BP_Damage_Shadow`, `BP_Damage_Fire`, …) and set the `Category` enum.
  Later systems (resistances, floating combat text, hit VFX) key off this.
- **Revive for testing:** call `HealthComponent->Revive()` from Blueprint or
  the console to restore HP. Note that this alone does not re-enable the
  capsule — for full respawn you'll want a dedicated Respawn flow.

Bindable component events (all `BlueprintAssignable`):
- `OnHealthChanged(OwningComponent, Old, New, Delta, Instigator, Causer)`
- `OnDeath(OwningComponent, Instigator, Causer)`

## 9. Enemies and melee AI

The project ships with a self-contained melee grunt so you can test combat
without any BehaviorTree assets:

- `ASOEnemyCharacter` — a `ACharacter` that owns a `USOHealthComponent` and
  exposes combat knobs (`SightRadius`, `LoseSightRadius`, `AttackRange`,
  `AttackDamage`, `AttackCooldown`, `AttackDamageType`, `CorpseLifetime`).
- `ASOEnemyAIController` — a native-C++ `AAIController` that runs an
  Idle → Chasing → Attacking state machine on a `ThinkInterval` timer
  (default 0.15s). It uses `MoveToActor` for pathing and calls the enemy's
  `PerformAttack` to apply damage. Enable `bDrawDebug` on the AI controller
  CDO or instance to visualize sight/attack radii and the target line.

### Drop one into the map

1. Compile the project.
2. In the Content Browser, right-click and create
   **Blueprint Class → SOEnemyCharacter**, name it `BP_MeleeGrunt`, and drop
   any skeletal or static mesh onto its Mesh component just so you can see it.
   (For a first test even the placeholder capsule is enough.)
3. Set tuning defaults on the Blueprint (recommended starting values):
   - `Movement Speed`: 400
   - `Sight Radius`: 1500 / `Lose Sight Radius`: 2200
   - `Attack Range`: 180
   - `Attack Damage`: 12 / `Attack Cooldown`: 1.4
   - `Corpse Lifetime`: 5.0
4. Drag `BP_MeleeGrunt` into your `L_TestArena` level, **inside the
   NavMeshBoundsVolume** so it can pathfind.
5. Play. The enemy will chase you as soon as you enter its sight radius,
   swing when in melee range, and die when your health component fires
   `OnDeath` (killing it back is not yet wired — that comes with the next
   task: player abilities).

### Damage flow

`ASOEnemyCharacter::PerformAttack` calls `UGameplayStatics::ApplyDamage`,
which fires the target actor's `OnTakeAnyDamage` — the same delegate the
player's `USOHealthComponent` is bound to. So the entire loop
(spawn enemy → chase → hit → apply damage → HP drops → OnDeath →
character death handler → controller stops accepting moves) already works
end-to-end.

If you want a stronger elite variant, create another BP subclass of
`SOEnemyCharacter` and bump `AttackDamage`, `MaxHealth`, and swap
`AttackDamageType` for a `BP_Damage_Shadow` subclass of `USODamageType`.

## 10. Player primary attack

The player has a melee-style primary attack driven from `ASOCharacter`.
It is a sphere overlap in front of the character, keyed to two inputs by
default:

| Input | Action        |
|-------|---------------|
| LMB   | MoveTo        |
| RMB   | PrimaryAttack |
| Q     | PrimaryAttack |
| K     | Debug damage  |

When PrimaryAttack fires:
1. The controller resolves the cursor to a world location (falls back to
   the character's forward vector if the cursor is over nothing).
2. `ASOCharacter::PerformPrimaryAttack(FVector)` snaps the character's
   yaw to face the target (toggleable via `bFaceAttackDirection`).
3. A sphere overlap of radius `PrimaryAttackRadius` is centered
   `PrimaryAttackRange` cm in front of the character on the trace channel
   `PrimaryAttackChannel` (default `ECC_Pawn`).
4. Every unique actor with a live `USOHealthComponent` gets
   `PrimaryAttackDamage` applied via `UGameplayStatics::ApplyDamage`
   using `PrimaryAttackDamageType` (falls back to `USODamageType`).
5. `OnPrimaryAttackPerformed(AttackCenter, ImpactActors)` fires — a
   `BlueprintImplementableEvent` for wiring up animation montages, hit
   VFX, camera shake, or SFX later.
6. A cooldown timer starts (`PrimaryAttackCooldown` seconds) blocking
   further swings until it elapses.

**Tuning on `BP_SOCharacter` (SupremeOverlord | Combat | Primary):**
- `PrimaryAttackDamage` (default 15)
- `PrimaryAttackRange` (default 220 cm — how far in front the hit sphere sits)
- `PrimaryAttackRadius` (default 140 cm — width of the swing)
- `PrimaryAttackCooldown` (default 0.5 s)
- `PrimaryAttackDamageType` (subclass of `USODamageType`)
- `PrimaryAttackChannel` (default `ECC_Pawn`)
- `bFaceAttackDirection` (default true)
- `bDrawPrimaryAttackDebug` — turn on to see the swing sphere and reach
  line every time you attack.

### Full end-to-end combat test

1. Drop a `BP_MeleeGrunt` onto the NavMesh (see section 9).
2. Enable `bDrawPrimaryAttackDebug` on `BP_SOCharacter` so you can see
   where the swing lands.
3. Play. Walk into the enemy's sight radius — it'll chase you and start
   hitting. Press **RMB** (or Q) to swing back; the sphere overlap
   applies damage every 0.5 s. Kill it and it'll drop, unpossess, and
   despawn after `CorpseLifetime`.

## 11. Shadow Bolt (ranged spell)

`ASOShadowBoltProjectile` is a self-contained ranged projectile:
`USphereComponent` + `UProjectileMovementComponent` + optional mesh. On
overlap with a valid actor it calls `UGameplayStatics::ApplyDamage`,
fires `OnBoltImpact` (BP hook for VFX/SFX), and destroys itself.

Defaults: **40 damage**, **2200 cm/s**, **3 s** lifetime. The bolt
auto-tries `/Engine/BasicShapes/Sphere` as its mesh so it's visible
without any asset setup.

### Wiring it in

1. Compile.
2. In the Content Browser create **Blueprint Class →
   SOShadowBoltProjectile**, name it `BP_ShadowBolt`. (Optional: give
   it a dark material and a translucent trail particle.)
3. Open `BP_SOCharacter`. In *SupremeOverlord | Combat | ShadowBolt* set
   **Shadow Bolt Class = BP_ShadowBolt**.
4. Optional tuning on the character (all `EditAnywhere,
   BlueprintReadWrite`):
   - `ShadowBoltCooldown` (default 1.5 s)
   - `ShadowBoltMuzzleForward` / `ShadowBoltMuzzleHeight` — where the
     bolt spawns relative to the character root.
   - `bFaceCastDirection` — snap yaw toward the cursor on cast.
5. Play. Press **E** to cast toward the cursor. The bolt travels
   straight, ignores the caster, and detonates on first hit.

### Blueprint hooks

- `OnShadowBoltCast(MuzzleLocation, AimDirection, SpawnedBolt)` fires on
  `ASOCharacter` — hook cast VFX / SFX / camera shake.
- `OnBoltImpact(Hit, HitActor)` fires on the projectile — hook impact
  VFX / decals.

## 12. HUD

`ASOHUD` (registered via `SOGameMode::HUDClass`) draws directly with the
Canvas so it works without any UMG widget:

- Red HP bar in the bottom-left with a numeric `HP  current / max`
  overlay.
- One-line control hints across the top (toggleable).
- Big red **YOU DIED** overlay when the health component reports dead.

All colors, sizes, and text strings are `EditAnywhere,
BlueprintReadWrite` UPROPERTYs under `SupremeOverlord|HUD|*`, so you
can create a `BP_SOHUD` subclass and tweak the look.

To replace the whole thing with a UMG widget later:
1. Subclass `AHUD` (or start from `ASOHUD` and override `DrawHUD` to no-op).
2. Create a `UUserWidget` bound to `USOHealthComponent::OnHealthChanged`.
3. Point `SOGameMode::HUDClass` at your new HUD class.

### Updated input table

| Input | Action        |
|-------|---------------|
| LMB   | MoveTo        |
| RMB   | PrimaryAttack |
| Q     | PrimaryAttack |
| E     | ShadowBolt    |
| K     | Debug damage  |

## 13. Mana

`USOManaComponent` is a lightweight regenerating pool:

- `MaxMana` / `StartingMana` / `RegenPerSecond` (default 100 / full / 10 s⁻¹).
- `RegenDelayAfterConsume` (default 1.0 s) briefly pauses regen after a
  successful `Consume(Cost)`, so casting a spell has bite.
- `Consume(Cost)` returns true if the pool had enough.
- `Restore(Amount)` / `RefillToMax()` add mana back.
- `OnManaChanged(OwningComp, Old, New, Delta)` is `BlueprintAssignable`.

The player character owns one automatically. `CastShadowBolt` refuses to
fire (and `CanCastShadowBolt()` returns false) when the pool has less
than `ShadowBoltManaCost` (default 25). The HUD draws a blue mana bar
above the HP bar.

**Tuning on `BP_SOCharacter` (Mana):** `MaxMana`, `StartingMana`,
`RegenPerSecond`, `RegenDelayAfterConsume`.
**Tuning on the character (ShadowBolt):** `ShadowBoltManaCost`.

## 14. Loot drops

Enemies roll a table on death and spawn `ASOPickupOrb` actors around
their corpse. Orbs bob and spin, magnetize toward the player once close,
and apply their payload on overlap.

### Orb setup

`ASOPickupOrb` has an `OrbType` enum:

| OrbType | Effect on pickup                                    |
|---------|-----------------------------------------------------|
| Health  | Calls `HealthComponent->Heal(Amount)`               |
| Mana    | Calls `ManaComponent->Restore(Amount)`              |
| Gold    | Calls `ASOCharacter::AddGold(RoundToInt(Amount))`   |

Create three BP subclasses so the loot table has something to reference:
- `BP_Orb_Health` — OrbType = Health, Amount = 25, red material on the mesh.
- `BP_Orb_Mana`   — OrbType = Mana,   Amount = 30, blue material on the mesh.
- `BP_Orb_Gold`   — OrbType = Gold,   Amount = 10, yellow material on the mesh.

Tunable per orb: `MagnetRadius`, `MagnetInterpSpeed`, `SpinRateDegPerSec`,
`BobAmplitude`, `BobRateHz`, `DespawnAfter` (30 s default). `OnPickedUp`
is a BP hook for pickup VFX/SFX.

### Enemy loot table

`ASOEnemyCharacter` now has a `LootTable` (array of `FSOLootDrop`). Each
row has:

- `OrbClass` — a `BP_Orb_*` subclass.
- `DropChance` — probability 0..1 that this row fires.
- `MinCount` / `MaxCount` — quantity spawned on a successful roll.

Additional knobs: `LootSpreadRadius` (disk around the corpse, default
80 cm) and `LootSpawnHeight` (default 50 cm so orbs float above the ground).

Recommended starting table on `BP_MeleeGrunt`:

| Row | OrbClass       | DropChance | Min | Max |
|-----|----------------|-----------:|----:|----:|
| 0   | BP_Orb_Gold    |       1.00 |   1 |   3 |
| 1   | BP_Orb_Health  |       0.35 |   1 |   1 |
| 2   | BP_Orb_Mana    |       0.35 |   1 |   1 |

### Gold + HUD

`ASOCharacter::AddGold(int32)` and `GetGold()` manage the currency
counter. The HUD draws a yellow "Gold: N" readout in the top-right;
tune position/color/scale under `SupremeOverlord|HUD|Gold`. Blueprint
subscribers can bind to `OnGoldChanged(Old, New, Delta)` for UI
animations.

### End-to-end loot test

1. Create `BP_Orb_Health`, `BP_Orb_Mana`, `BP_Orb_Gold` (BP subclasses
   of `SOPickupOrb`) and set their `OrbType` / `Amount`.
2. Open `BP_MeleeGrunt` → `Loot Table` → add rows per the table above.
3. Play. Kill the grunt with **RMB** or **E**. Orbs will spawn around
   the corpse, float toward you when close, and top up HP / Mana / Gold.

## 15. Equipment (weapons)

`USOItemData` is a `UPrimaryDataAsset` — the base for every item.
`USOWeaponData` extends it with `PrimaryDamageBonus`,
`ShadowBoltDamageBonus`, and `PrimaryAttackCooldownMultiplier`.

`ASOCharacter` has a single weapon slot. On BeginPlay it auto-equips
`StartingWeapon`. Attack code now goes through effective-stat
getters, so weapon stats stack on top of the character base
automatically:

- `GetEffectivePrimaryAttackDamage() = PrimaryAttackDamage + Weapon->PrimaryDamageBonus`
- `GetEffectivePrimaryAttackCooldown() = PrimaryAttackCooldown * Weapon->PrimaryAttackCooldownMultiplier`
- `GetEffectiveShadowBoltDamage()   = ShadowBoltBaseDamage + Weapon->ShadowBoltDamageBonus`

### Create a weapon

1. Right-click in Content → **Miscellaneous → Data Asset** → pick
   `SOWeaponData` → name `DA_Wpn_RustedSword`.
2. Fill in `DisplayName`, `Rarity` (Common..Legendary), and stat
   bonuses (e.g. `PrimaryDamageBonus = 8`).
3. On `BP_SOCharacter`, set *SupremeOverlord | Equipment | Starting
   Weapon = DA_Wpn_RustedSword*.
4. Play. The bottom-right of the HUD shows the weapon name colored by
   rarity (tan for Common, blue for Magic, gold for Rare, orange for
   Legendary). Damage numbers scale accordingly.

Blueprints can call `EquipWeapon(WeaponData)` at runtime — great for
picking up a weapon later. Subscribe to `OnWeaponChanged(Old, New)`
to react in UMG.

## 16. Ranged caster enemy

`ASOCasterEnemyCharacter` inherits from `ASOEnemyCharacter` and
overrides `PerformAttack` to spawn a `ASOShadowBoltProjectile` at the
target. Reuses the existing `ASOEnemyAIController` unchanged — the
larger `AttackRange` default (1100 cm) means the AI stops just inside
casting range instead of pressing into melee.

### Setup

1. Create `BP_ShadowBolt` (BP subclass of `SOShadowBoltProjectile`) if
   you don't already have one — same asset the player uses.
2. Create `BP_CultistCaster` (BP subclass of `SOCasterEnemyCharacter`).
   Set *SupremeOverlord | Caster | Bolt Class = BP_ShadowBolt*.
3. Optional: tweak `BoltDamage`, `MuzzleForward`, `MuzzleHeight`.
4. Drop one into the arena, inside the NavMesh. Aggro range is 1800 cm
   by default — it'll spot you from further away than a melee grunt.

## 17. XP and leveling

`USOExperienceComponent` (spawned on `ASOCharacter`) uses the curve
`XPForNextLevel(L) = BaseXPForLevel2 * L^LevelCurveExponent`.
Defaults: 100 XP to hit L2, ~283 to L3, ~520 to L4.

- `ASOEnemyCharacter::XPReward` (default 25) grants XP to the killer.
- On level-up, `ASOCharacter::HandleLevelUp` bumps
  `MaxHealth`, `MaxMana`, `PrimaryAttackDamage`, and
  `ShadowBoltBaseDamage` by the per-level scalars, then tops the pools
  off. `OnLevelUpReached(NewLevel)` is a BP hook for VFX/SFX.
- HUD draws a gold XP bar and "Lv N" number at the bottom-center of
  the screen.

Blueprints can subscribe to `OnLevelUp(Comp, NewLevel, PrevLevel)` and
`OnExperienceChanged(Comp, NewXP, XPForNext, Level)` for animations.

## 18. Life Drain — second ability (R)

Vampiric AoE spell centered on the caster. All live enemies inside
`LifeDrainRadius` (default 420 cm) take `LifeDrainDamage`, and the
caster heals for `LifeDrainHealFraction` (default 0.5) of the total
damage dealt.

- Cost: `LifeDrainManaCost` (default 40).
- Cooldown: `LifeDrainCooldown` (default 6 s).
- `bDrawLifeDrainDebug` visualizes the AoE sphere on cast.
- `OnLifeDrainCast(Hits, TotalDamage, Healed)` is a BP hook for
  cast VFX / SFX / camera shake.

Bound to **R** by default.

### Full input table

| Input | Action              |
|-------|---------------------|
| LMB   | MoveTo              |
| RMB   | PrimaryAttack       |
| Q     | PrimaryAttack       |
| E     | ShadowBolt          |
| R     | LifeDrain           |
| F1    | Allocate Strength   |
| F2    | Allocate Intellect  |
| F3    | Allocate Vitality   |
| ESC   | Toggle Pause        |
| F10   | Quit (when paused)  |
| K     | Debug damage        |

## 19. Item drops

`ASOItemPickup` is a floating cube-shaped pickup that carries any
`USOItemData` (weapons for now). On overlap with the player it calls
`EquipWeapon` and self-destroys. The mesh is tinted with a Dynamic
Material Instance to the item's rarity color so drops are readable at
a glance.

Enemies now also have an **item drop pool** on top of the orb loot
table:

- `ItemPickupClass` — a `BP_ItemPickup` subclass of `ASOItemPickup`.
- `ItemDropChance` — overall chance any item drops (default 0.15).
- `ItemDropPool` — weighted rows of `USOItemData` assets. Higher
  Weight = more common. `RollItemDrop` weighted-picks one row and
  spawns `ItemPickupClass` carrying it.

**Recipe for weapon drops:**
1. Create data assets `DA_Wpn_RustedSword` (Common, +8),
   `DA_Wpn_MagicBlade` (Magic, +14), `DA_Wpn_LegendaryStaff`
   (Legendary, `PrimaryDamageBonus=6`, `ShadowBoltDamageBonus=24`).
2. Create `BP_ItemPickup` (BP subclass of `SOItemPickup`).
3. On `BP_MeleeGrunt`: set `ItemPickupClass = BP_ItemPickup`,
   `ItemDropChance = 0.20`, and populate `ItemDropPool` with the three
   weapons at weights 6 / 3 / 1.
4. Kill enough grunts and a colored cube will drop. Walk over it to
   auto-equip.

## 20. Skill panel HUD

The HUD now renders a centered row of three skill tiles (Strike /
Shadow / Drain) above the XP bar. Each tile shows:

- Key label (top-left)
- Skill name (bottom)
- Top-down cooldown overlay with seconds remaining
- Mana cost line below (turns red and desaturates the tile when the
  player can't afford it)

Tile size, gap, border, and cooldown-overlay tint are exposed under
`SupremeOverlord|HUD|Skills`.

## 21. Pause menu

`ESC` toggles pause via `UGameplayStatics::SetGamePaused`. The HUD
dims the screen and shows a **PAUSED** title plus a hint line
(`ESC Resume | F10 Quit`). `F10` only quits while paused, so a stray
press during combat doesn't kill the session. Both actions are bound
`bExecuteWhenPaused = true` so they fire under the paused clock.

## 22. Passive attributes (STR / INT / VIT)

`USOAttributesComponent` grants `PointsPerLevel` unspent points at
every level-up. Spend them with **F1 (STR)**, **F2 (INT)**, **F3 (VIT)**.
Each point applies immediately to the owning character's pools:

- **STR** → +1 `PrimaryAttackDamage` per point
- **INT** → +1.5 `ShadowBoltBaseDamage`, +3 `MaxMana` per point (and refills the delta)
- **VIT** → +5 `MaxHealth` per point (and heals the delta)

Multipliers live on the component as `EditAnywhere` UPROPERTYs, so you
can tune them without recompiling.

HUD panel in the top-left shows the three attributes + F-key hints and
a golden "Unspent points: N" line when you have points to spend.

## 23. Wave spawner

`ASOEnemySpawner` runs waves of enemies picked from a weighted pool
inside a disc around the spawner:

- `EnemyPool` — array of `{EnemyClass, Weight}` rows.
- `EnemiesPerWave` (default 3), `EnemiesPerWaveGrowth` (+1 per wave),
  `MaxEnemiesPerWave` (cap).
- `SpawnRadius` (default 800 cm — visualized by the sphere gizmo).
- `SpawnStagger` (delay between individual spawns inside a wave).
- `InterWaveDelay` (rest between waves).
- `bLimitTotalWaves` + `TotalWaves` (endless by default).

Delegates: `OnWaveStarted(WaveIndex, EnemyCount)`,
`OnWaveCleared(WaveIndex)`, `OnAllWavesCleared`.

The HUD auto-detects any `ASOEnemySpawner` in the level and draws a
"Wave N — X / Y enemies" counter across the top, transitioning to
"All waves cleared" once every spawner finishes.

### Setup

1. Drop `SOEnemySpawner` into the level, inside your NavMesh.
2. Populate `EnemyPool` with `BP_MeleeGrunt` (weight 3) and
   `BP_CultistCaster` (weight 1).
3. Play. Wave 1 spawns 3 enemies; kill them → 5s rest → wave 2 spawns
   4. Repeats indefinitely (or set `bLimitTotalWaves` + `TotalWaves =
   5` for a fixed 5-wave arena).

## 24. Quick save / quick load (F5 / F9)

`USOSaveGame` snapshots the persistent parts of the player state:

- Level, XP-in-level, gold, STR/INT/VIT + unspent points, current +
  max HP/Mana, PrimaryAttackDamage / ShadowBoltBaseDamage.
- **Full equipment loadout** (all 9 slots, not just the weapon) —
  each equipped item's source template, rarity, item level, rolled
  affixes, and socketed gems round-trip via
  `USOLootRoller::ReconstructItemInstance`, which replays the exact
  same affix rolls rather than re-randomizing them.
- **Crafting materials** — every material and its stack count.
- **Talents** — every unlocked node plus banked talent points, via
  `USOTalentComponent::RestoreFromSave` (respecs any current unlocks
  first, then reapplies the saved set from scratch).
- **Quests** — active quest objective progress and the completed
  quest list, via `USOQuestComponent::RestoreQuestState`.
- **Bestiary** — kill counts per enemy species.
- **Achievements** — every unlocked achievement ID (restored silently,
  no toast replay on load).
- **Potion charges**.
- **Waypoints** — every discovered waypoint plus the last-traveled-to
  one. Waypoints are matched back up by `ASOWaypoint::GetStableID()`
  (an explicit `WaypointID`, or the actor's own internal name if
  unset) rather than by position, so this survives minor level edits.
- **Difficulty tier** (New Game+) and the **corruption meter**.

Item instances (drops, crafted results) carry a new
`USOItemData::SourceTemplate` soft pointer back to the template asset
they were rolled from — that's what makes an item with random affixes
+ sockets fully reconstructable from a save file instead of only
round-tripping hand-authored template assets.

- **F5** = QuickSave to `DefaultSaveSlot` (default
  `"SupremeOverlordSlot"`).
- **F9** = QuickLoad from that same slot.
- Both work while paused, so you can bail out via ESC → F9.
- `OnSaveGameCompleted(bSaved, bSuccess, Slot)` is a BP hook for
  save/load toasts.

Note: level state (dropped orbs, wave/spawner progress, corpses,
other actors' positions) is intentionally *not* persisted — this is a
walk-out-of-town save, not a full world snapshot.

## 25. Boss encounter (`ASOBossCharacter`)

`SOBossCharacter` inherits from the base enemy and adds:

- HP-driven phase transitions at **66% (Phase 2)** and **33% (Phase 3)**.
  Each phase rescales `AttackDamage` and `MovementSpeed` from cached
  base values, so re-entering a phase can't compound.
- A telegraphed AoE that fires every `(AoECadence + 1)` swings —
  draws a red debug circle at the target for `TelegraphWindup`
  seconds, then damages every live health-carrying non-enemy in
  `TelegraphedAoERadius`.
- Boss defaults: 500 HP, 250 XP reward, 8 s corpse.

BP hooks:
- `OnBossPhaseChanged(OldPhase, NewPhase)` — delegate.
- `OnPhaseEnteredBP(NewPhase)` — for phase-transition VFX/dialogue.
- `OnTelegraphAoEStart(Center, Radius, Windup)` — decal spawn hook.
- `OnTelegraphAoEResolve(Center, Radius, EnemiesHit)` — impact FX.

The HUD auto-detects the closest live boss and draws a red HP bar
with nameplate across the top (`BossDisplayName`, tunable per BP).

**Setup**
1. Create `BP_ArenaLord` (BP subclass of `SOBossCharacter`) — set
   `BossDisplayName`, tweak `MaxHealth` on the health component, and
   optionally assign `AoEDamageType` to a `BP_Damage_Shadow` subclass.
2. Drop one into the arena. The AI (inherited from
   `ASOEnemyAIController`) chases the player and starts swinging.

## 26. Elite modifiers (`USOEliteComponent`)

Attach `USOEliteComponent` to any enemy Blueprint (or add it at
runtime) to grant one or more affixes:

| Affix        | Effect                                    |
|--------------|-------------------------------------------|
| Fortified    | ×2.5 MaxHealth                            |
| Vicious      | ×1.5 AttackDamage                         |
| Swift        | ×1.4 MovementSpeed                        |
| Enriched     | ×2.0 loot counts, ItemDropChance, XP      |
| Far-Sighted  | ×1.5 SightRadius + LoseSightRadius        |

The enum is a bitmask — pick one or several via the checkbox row in
the editor. `bRandomizeOnBeginPlay` + `RollCount` picks unique
random affixes at spawn (drop this component on
`BP_MeleeGrunt_Elite`, set RollCount = 2 → every spawned instance
gets two random affixes).

`ApplyAffixesToOwner` runs in `OnRegister` so the mutated max HP /
mana values seed the health/mana components on their first tick. All
multipliers are `EditAnywhere` UPROPERTYs.

BP hook: `OnAffixesApplied(AffixMask)` — for aura VFX / SFX.

## 27. Sound hooks

Every gameplay actor now exposes `USoundBase*` slots you can assign
in a Blueprint subclass. When left null the event is silent; when
set, it fires via `UGameplayStatics::PlaySoundAtLocation`.

**`ASOCharacter`:**
- `PrimaryAttackSFX`, `ShadowBoltCastSFX`, `LifeDrainCastSFX`,
  `LevelUpSFX`, `DeathSFX`.

**`ASOEnemyCharacter`:** `AttackSFX`, `DeathSFX`.

**`ASOPickupOrb`, `ASOItemPickup`:** `PickupSFX`.

**`ASOShadowBoltProjectile`:** `ImpactSFX`.

Standard workflow: create a `BP_SOCharacter`, expand
`SupremeOverlord|Audio`, and assign the engine's default cues (or
your own imports) to hear everything without touching code.

### Final input table

| Input | Action              |
|-------|---------------------|
| LMB   | MoveTo              |
| RMB   | PrimaryAttack       |
| Q     | PrimaryAttack       |
| E     | ShadowBolt          |
| R     | LifeDrain           |
| F     | Interact (Vendor buy) |
| G     | Sell (main-hand)    |
| F1    | Allocate Strength   |
| F2    | Allocate Intellect  |
| F3    | Allocate Vitality   |
| F5    | QuickSave           |
| F9    | QuickLoad           |
| ESC   | Toggle Pause        |
| F10   | Quit (when paused)  |
| K     | Debug damage        |

## 28. Talent tree

`USOTalentComponent` banks `TalentPointsPerLevel` (default 1) at every
level-up. Nodes are `USOTalentNode` data assets with:

- `PointCost`, `Prerequisites` (hard pointers to other nodes)
- `Effects[]` — enum + magnitude:
  `FlatMaxHealth`, `FlatMaxMana`, `FlatPrimaryDamage`,
  `FlatShadowBoltDamage`, `FlatLifeDrainHealFrac`, `FlatManaRegen`,
  `MultMovementSpeed`.

`UnlockNode(Node)` enforces cost + prereqs and applies effects to the
owning character; `CanUnlock(Node, OutReason)` returns a human-readable
"why not" for a future tooltip UI. BP delegates
`OnTalentPointsChanged`, `OnTalentUnlocked`.

### Setup

1. Create data assets `DA_Talent_Toughness` (+30 MaxHealth, cost 1),
   `DA_Talent_Bloodbind` (+0.2 LifeDrainHealFrac, cost 2, prereq
   Toughness), `DA_Talent_Overlord` (+10 ShadowBoltDamage,
   +10 PrimaryDamage, cost 3, prereq Bloodbind).
2. Play — level up and call `TalentComponent->UnlockNode(DA_...)`
   from BP (a full UMG tree UI comes later).

## 29. Inventory (multi-slot equipment)

`USOItemData::EquipSlot` declares where an item goes:
MainHand / OffHand / Head / Chest / Legs / Boots / Amulet / Ring1 /
Ring2. `USOArmorData` extends the base item with `MaxHealthBonus`,
`MaxManaBonus`, `DamageReductionPct`, `MovementSpeedMultiplier`.

`USOEquipmentComponent` owns a `TMap<ESOEquipSlot, USOItemData*>`.
`Equip(Item)` stores it in the item's declared slot and calls
`RecomputeAggregateStats`, which sums bonuses from scratch and diffs
against last-applied so nothing is ever double-counted. MainHand
changes route through `ASOCharacter::EquipWeapon` so existing damage
code keeps working.

Damage reduction rides on the health component's
`IncomingDamageMultiplier`, so all damage sources honor it
automatically. `OnSlotChanged(Slot, OldItem, NewItem)` for BP.

### Setup

1. Create `DA_Armor_LeatherChest` (Rarity Common, EquipSlot Chest,
   MaxHealthBonus 30).
2. Create `DA_Armor_RingOfPower` (Rarity Rare, EquipSlot Ring1,
   MovementSpeedMultiplier 1.1).
3. Extend `BP_MeleeGrunt`'s `ItemDropPool` with these DataAssets so
   drops feel varied.

## 30. Crafting

`USOMaterialData` (marker + `MaxStack`) + `USORecipeData`
(Ingredients array + Result item + GoldCost).

`USOInventoryComponent` counts materials in a
`TMap<USOMaterialData*, int32>` respecting per-material `MaxStack`.
`Craft(Recipe)` verifies via `CanCraft`, consumes ingredients + gold,
and auto-equips the result via the equipment component when the
result has an `EquipSlot`. `OnRecipeCrafted(Recipe, Result)` BP
delegate for craft-bench animations.

### Recipe recipe

1. `DA_Mat_ShadowIron` (MaxStack 99).
2. `DA_Recipe_ShadowBlade` — ingredients: `DA_Mat_ShadowIron × 3`,
   gold 100, result: `DA_Wpn_ShadowBlade` (any weapon data asset).
3. Hand your player some materials (BP call
   `InventoryComponent->AddMaterial(DA_Mat_ShadowIron, 3)`), then
   `InventoryComponent->Craft(DA_Recipe_ShadowBlade)` — the sword
   equips itself immediately.

## 31. Vendor NPC

`ASOVendorNPC` is a cylinder actor with an interaction sphere. When
the player enters `InteractionRadius` (default 220 cm), the HUD shows
a yellow prompt `Vendor   [F] Buy Next   [G] Sell Weapon`.

- **F** (Interact) — calls `TryBuyNext` on the nearest in-range
  vendor; cycles through `Stock` from an internal cursor and buys the
  next affordable row the player doesn't already own.
- **G** (Sell) — calls `TrySellCurrentWeapon`; unequips the main-hand
  weapon and pays out `SellbackFraction × ListedPrice`
  (`SellbackFlatFallback` when the weapon isn't in the stock table).

BP delegates: `OnPlayerEnteredRange`, `OnPlayerLeftRange`,
`OnPurchase`, `OnSellback`.

### Setup

1. Drop `SOVendorNPC` in the arena.
2. Fill `Stock` with `{Item = DA_Wpn_RustedSword, Price = 150}`,
   `{Item = DA_Armor_LeatherChest, Price = 250}`,
   `{Item = DA_Armor_RingOfPower, Price = 900}`.
3. Play. Walk into range — the prompt appears. Press F to cycle
   through purchases; G to dump your current weapon for coin.

## 32. Minimap (radar)

`ASOHUD` renders a top-right square minimap tracking everything of
interest around the player:

- Yellow center pip = player.
- Red = enemies (bosses tinted orange).
- Blue = vendors.
- Green = orb pickups. Gold = item pickups.

`MinimapWorldRange` (default 3500 cm) is the half-edge world width.
All colors + sizes exposed under `SupremeOverlord|HUD|Minimap`.

Dots outside the map rect are clipped so nothing smears across the
screen. Actor queries use `UGameplayStatics::GetAllActorsOfClass` on
render — for very large levels, replace with a spatially-partitioned
lookup later.

## 33. Environmental Hazard Zones

`ASOHazardZone` is a placeable actor with a `UBoxComponent` trigger.
Any pawn that enters the box takes `DamagePerTick` every
`TickInterval` seconds until they leave. A timer fires `Lifetime`
seconds after spawn — if set non-zero the zone self-destructs.

**Types** (`ESOHazardType`): Fire, Poison, Frost, Lightning, Necrotic.
Each type drives a helper color returned by `GetHazardColor()` so the
ground decal can be tinted dynamically.

Slots: `DecalMaterial` for the ground projection, `AmbientParticles`
for looping VFX, `HitFX` played per-victim per-tick.

### Setup

1. Place a `ASOHazardZone` actor in your arena.
2. Set `HazardType = Fire`, `DamagePerTick = 8`, `TickInterval = 0.5`
   (16 DPS), `Lifetime = 10` (pool burns for 10 s then goes away).
3. Assign a decal material and particle in the slots.
4. To spawn from a boss: `GetWorld()->SpawnActor<ASOHazardZone>(...)` 
   inside `OnPhaseEnteredBP`; set `Lifetime` to match the phase window.

## 34. Quest System

`USOQuestData` is a `UPrimaryDataAsset` holding:
- `QuestTitle` / `QuestDescription`
- `Objectives` (`TArray<FSOQuestObjective>`) — each objective has a
  `Type`, optional `TargetClass` or `TargetMaterial`, `RequiredCount`,
  and (for ReachLocation) a `TargetLocation + AcceptanceRadius`.
- `Reward` (XP, gold, optional item).
- `PrerequisiteQuest` (gate behind another quest completion).

**Objective types**: KillEnemies, CollectMaterial, ClearWaves,
DefeatBoss, ReachLocation.

`USOQuestComponent` lives on `ASOCharacter` and tracks
`ActiveQuests[]` + `CompletedQuests[]`.

**Wire-up**:
- Kill events flow from `ASOEnemyCharacter::HandleDeath` → player's
  `QuestComponent->NotifyEnemyKilled(enemy)` automatically.
- Wave clears propagate via `ASOEnemySpawner::OnWaveCleared` delegate
  bound in `BeginPlay`.
- Inventory material counts update via
  `InventoryComponent::OnMaterialCountChanged`.
- ReachLocation objectives are polled at 4 Hz on Tick.

**HUD**: Active quests are listed in the left panel below the
attribute readouts. Up to `MaxQuestsInTracker` quests shown; each
objective line shows `[ ] Desc 0/3` or `[x] Desc 3/3` in green when
done.

**BP delegates**: `OnQuestAccepted`, `OnObjectiveUpdated`,
`OnQuestCompleted`, `OnQuestFailed` — bind in the HUD Blueprint for
animated popups.

### Setup

1. Create `DA_Quest_Survive` (title "Survive the Assault"):
   - Objective 0: Type=ClearWaves, RequiredCount=3, Description="Clear 3 waves".
   - Objective 1: Type=KillEnemies, TargetClass=BP_MeleGrunt, RequiredCount=10, Description="Kill 10 Grunts".
   - Reward: XP=500, Gold=200.
2. In the level BP (or a trigger volume), call
   `Character->QuestComponent->AcceptQuest(DA_Quest_Survive)`.
3. Play — the quest tracker appears in the HUD left panel; it
   auto-advances as enemies die and waves clear.


---

## 35. Status Effect System

**Files**: `SOStatusEffectComponent.h/.cpp`

`USOStatusEffectComponent` is a `UActorComponent` that manages ticking
debuffs and buffs on any character — both the player and enemies carry
it by default.

### Effect types

| Type     | Behaviour |
|----------|-----------|
| Burning  | Fire DOT per tick |
| Poisoned | Nature DOT, can stack (e.g. max 5 stacks) |
| Frozen   | Walk speed → 0 immediately |
| Slowed   | Walk speed reduced by `SlowFraction` (default 50 %) |
| Shocked  | Incoming damage × `ShockedDamageMultiplier` (1.25) |
| Blessed  | Negative `DamagePerTick` = heal over time |
| Cursed   | Outgoing damage × `CursedDamageMultiplier` (0.75) |

### API

```cpp
StatusEffectComponent->ApplyEffect(ESOStatusEffectType::Burning,
    /*Duration*/ 5.0f,
    /*DamagePerTick*/ 8.0f,
    /*TickInterval*/ 1.0f,
    /*MaxStacks*/ 1);
StatusEffectComponent->RemoveEffect(ESOStatusEffectType::Burning);
bool bBurning = StatusEffectComponent->HasEffect(ESOStatusEffectType::Burning);
float mult = StatusEffectComponent->GetIncomingDamageMultiplier();
```

Applying while an effect is already active refreshes the duration (and
adds a stack if `CurrentStacks < MaxStacks`).

### Hazard zone integration

`ASOHazardZone` can optionally apply a lingering status when an actor
**leaves** the zone: set `bApplyLingeringStatus = true` and configure
`LingeringStatusType`, `LingeringDuration`, `LingeringDamagePerTick`.

### HUD

The HUD renders a row of coloured tiles **above the mana bar**.
Each tile shows:
- A 3-letter abbreviation (BRN / PSN / FRZ / SLW / SHK / BLS / CRS).
- Remaining seconds at the bottom.
- A yellow stack-count badge in the top-right corner when stacked.

Tile size and gap are tunable via `StatusIconSize` / `StatusIconGap`
on the HUD asset.

---

## 36. Dialogue System

**Files**: `SODialogueNode.h/.cpp`, `SODialogueComponent.h/.cpp`,
`SODialogueNPC.h/.cpp`

Builds conversation trees from `USODialogueNode` data assets.
Each node contains a speaker name, body text, and up to four
`FSODialogueChoice` entries that point to the next node.

### Asset workflow

1. Create one `SODialogueNode` data asset per "page" of dialogue in
   the Content Browser.
2. Fill in `SpeakerName`, `BodyText`, and any `Choices` (ChoiceText +
   soft-ref to the next `SODialogueNode`).  Leave Choices empty to
   make the node a plain advance-or-end screen.
3. Chain nodes by setting `NextNode` on each choice.
4. Place an `ASODialogueNPC` actor in the level and set `EntryNode` on
   its `DialogueComponent` to the root data asset.

### Interaction

- Press **F** near an NPC to start the dialogue.
- Press **F** again (or **1-4**) to advance through nodes or pick
  choices.
- Conversation ends automatically when a NextNode pointer is null.

### HUD

The HUD shows a full-width panel at the **bottom-center** of the
screen while a dialogue is active (suppressed during pause):

```
┌──────────────────────────────────────────────────────────────────┐
│ Elder Moros                                                      │
│ "You have proven yourself worthy of the dark blessing..."        │
│  [1]  Accept the blessing.                                       │
│  [2]  Refuse and walk away.                                      │
└──────────────────────────────────────────────────────────────────┘
```

Panel look is tunable via `DialoguePanelColor`, `DialoguePanelBorderColor`,
`DialogueSpeakerColor`, `DialogueBodyColor`, `DialogueChoiceColor`, and
`DialogueTextScale` on the HUD asset.

### Delegates (bind in BP for animations)

- `OnDialogueStarted(FirstNode)`
- `OnDialogueNodeChanged(NewNode)`
- `OnDialogueEnded()`

### Key bindings

| Key | Action |
|-----|--------|
| F   | Start dialogue / advance no-choice node |
| 1-4 | Select choice 1-4 |

---

## 37. Minion Summoning System

**Files**: `SOMinion.h/.cpp`, `SOSummonComponent.h/.cpp`

The player commands an army of undead/dark minions — the core "Supreme
Overlord" power fantasy.

### Components

**`USOSummonComponent`** (on `ASOCharacter`):
- `MaxMinions` (default 3) — cap on simultaneous live minions.
- `ManaCostPerSummon` (default 30) — mana drained per summon.
- `SummonCooldown` (default 1.5 s) — minimum time between summons.
- `MinionClass` — assign `BP_Minion_Skeleton` (or any `ASOMinion`
  subclass) in the editor.
- `SummonMinion(Location, Caster)` — enforces all guards, snaps the
  spawn point to the nav mesh via `GetRandomPointInNavigableRadius`,
  and broadcasts `OnMinionSummoned`.
- `DismissAll()` — destroys every active minion immediately.
- `GetActiveCount()` — prunes stale weak-object pointers and returns
  the live count.

**`ASOMinion`** (spawned actor):
- Carries `USOHealthComponent` (HP) and `USOStatusEffectComponent`
  (can be Burning/Slowed/etc. by hazard zones or enemy spells).
- Ticks at 10 Hz: finds the nearest `ASOEnemyCharacter` within
  `AggroRange` (2000 cm default), chases via
  `UAIBlueprintHelperLibrary::SimpleMoveToActor`, attacks when within
  `AttackRange` on an `AttackCooldown` timer.
- Optional `LifetimeDuration` (0 = permanent) — minion auto-dissipates
  after the set number of seconds.
- On death: broadcasts back to the owning `SummonComponent` via
  `OnMinionDied`, disables collision, un-possesses the AI controller,
  and sets a 3-second lifespan for corpse visibility.

### Key bindings

| Key | Action |
|-----|--------|
| T   | Summon one minion at cursor position |
| Y   | Dismiss all active minions |

### HUD changes

The skill panel expanded from 3 → **4 tiles**. The fourth tile (blue)
shows:
- **T** key label and "Summon" name.
- Cooldown overlay and remaining time.
- Mana cost badge.
- **`X / Y`** active/max minion count below the tile in light blue.

### Setup

1. Create a `BP_Minion_Skeleton` Blueprint subclass of `ASOMinion`.
   - Set `MaxHealth` to 60, `AttackDamage` to 15, `AttackCooldown`
     to 1.2 s.
   - Assign a skeletal mesh (skeleton/zombie/wraith) and an
     `AAIController` (or leave default).
2. On `BP_SOCharacter` (the player Blueprint), set
   `SummonComponent → MinionClass = BP_Minion_Skeleton`.
3. Optionally set `SummonCastSFX` for an audio cue.
4. Play — press **T** near enemies to summon, **Y** to recall the
   army. The HUD fourth tile shows live count `2 / 3`.

## 38. Dark Presence Aura

**Files**: `SOAuraComponent.h/.cpp`

Passive necrotic aura radiating from the player every `PulseInterval`
(default 0.5 s).  All live `ASOEnemyCharacter` actors within `AuraRadius`
(default 600 cm) are:
1. Slowed for `SlowDuration` (default 0.65 s — slightly longer than the
   pulse so slow is continuous while inside the ring).
2. Dealt `DamagePerPulse` necrotic damage (default 0, designer-set).

Toggle with `bAuraActive`.  Delegates: `OnAuraPulse(EnemiesAffected)`
and the BP event `OnAuraPulseBP`.

### HUD

A translucent purple aura ring (16 dots) is drawn on the minimap
centered on the player pip, scaled to `AuraRadius / MinimapWorldRange`.

### Setup

1. `USOAuraComponent` is already added to `ASOCharacter` in the
   constructor.
2. On `BP_SOCharacter` → *Aura* category set `AuraRadius` and optionally
   `DamagePerPulse`.
3. To toggle at runtime: `AuraComponent→bAuraActive = false`.

## 39. Trap System

**Files**: `SOTrap.h/.cpp`

Three selectable trap types placed at the cursor with **C**, cycled with
**V**:

| Key | Trap          | Effect                             |
|-----|---------------|------------------------------------|
| —   | ShadowSnare   | Slows + necrotic damage            |
| —   | ArcaneMine    | Burst damage + Shocked status      |
| —   | NecroticSpore | Poison DoT (5 s, 8 dmg/tick)       |

`ASOTrap` arms after `ArmDelay` (default 0.8 s), then triggers on the
first `ASOEnemyCharacter` to enter its `TriggerRadius` (default 150 cm).
`ASOCharacter::MaxActiveTrapCount` (default 10) caps simultaneous traps;
the oldest is destroyed when the cap is exceeded.

### HUD changes

Skill panel now has 7 tiles (was 4).  Tile 5 (golden, **C** key):
- Shows selected trap type ("Snare" / "Mine" / "Spore") as a label below
  the tile.
- Cooldown overlay: 0.5 s between placements.

### Setup

1. Create `BP_Trap` (BP subclass of `ASOTrap`).  Tweak stats per type.
2. On `BP_SOCharacter` set `TrapClass = BP_Trap` and optionally
   `TrapPlaceSFX`.
3. Press **C** in-game — the trap appears at the cursor, flashes while
   arming, then detonates on enemy contact.

## 40. Corruption / Overlord Mode

**Files**: `SOCorruptionComponent.h/.cpp`

Every enemy killed adds `CorruptionValue` (default 8) to a corruption
meter that decays at `DecayRate` (default 3 / s) when idle.  When the
meter reaches `MaxCorruption` (default 100) the player may press **Z** to
activate Overlord Mode:

- All outgoing damage multiplied by `OverlordDamageMultiplier` (default
  1.5×) for `OverlordModeDuration` (default 10 s).
- On expiry, corruption resets to 0.
- `OnOverlordModeStarted` / `OnOverlordModeEnded` delegates; matching BP
  events for VFX/SFX hooks.

### HUD changes

- **Corruption bar**: thin purple strip centered above the XP bar.
  Color shifts to bright purple when full; overlaps with the Overlord
  Mode tile.
- **Skill tile 6** (purple, **Z** key): dims when not full, glows when
  active.
- **"OVERLORD MODE!"** centered flash text while the mode is running.

### Setup

1. Set `ASOEnemyCharacter::CorruptionValue` per enemy type (higher for
   bosses).
2. Set `OverlordModeActivateSFX` on `BP_SOCharacter`.
3. Fill the corruption bar by killing enemies, then press **Z**.

## 41. Necromantic Resurrection

**Files**: `SOSummonComponent.h/.cpp` (added `ResurrectAtLocation`)

Press **U** to raise the nearest dead `ASOEnemyCharacter` within
`ResurrectRange` (default 500 cm of the cursor) as a temporary minion.
Shares the minion cap with normal summons.

Key config on `SummonComponent`:

| Property                  | Default | Notes                              |
|---------------------------|---------|------------------------------------|
| `ManaCostPerResurrect`    | 20      | Deducted from mana on use          |
| `NecromancyCooldown`      | 6 s     | Separate cooldown from T-summon    |
| `ResurrectedMinionLifetime` | 30 s  | Auto-dissipates; 0 = permanent     |
| `ResurrectRange`          | 500 cm  | Radius around cursor               |

Only enemies with `bCanBeResurrected = true` (default) are eligible.
Elite / boss enemies may set this to `false` in their BP subclass.

### HUD changes

Skill tile 7 (green, **U** key): shows mana cost, necromancy cooldown
overlay, and dims when mana is insufficient.

### Setup

1. `SummonComponent` already exists on `ASOCharacter`.  Set `MinionClass`
   to your minion Blueprint (same as T-summon).
2. Set `NecroResurrectSFX` for audio feedback.
3. Kill an enemy, walk to the corpse, press **U**.

## 42. Corpse Explosion

**Files**: `SOCorpseExplosionComponent.h/.cpp` (new), `SOCharacter.h/.cpp`,
`SOPlayerController.h/.cpp`, `DefaultInput.ini`, `SOHUD.h/.cpp`

Press **X** to consume the nearest dead `ASOEnemyCharacter` within
`SearchRange` (default 400 cm) of the cursor. The corpse detonates,
dealing `MaxHealth * HPFraction` damage to every live enemy inside
`ExplosionRadius` (default 350 cm), then the corpse is destroyed.

Key config on `CorpseExplosionComponent` (added to `ASOCharacter`):

| Property        | Default | Notes                                   |
|-----------------|---------|------------------------------------------|
| `SearchRange`   | 400 cm  | Max distance from cursor to find a corpse |
| `ExplosionRadius` | 350 cm | Blast radius centered on the corpse      |
| `HPFraction`    | 0.40    | Fraction of each victim's MaxHealth dealt |
| `ManaCost`      | 35      | Deducted on a successful cast             |
| `Cooldown`      | 3 s     | Time between casts                        |

`OnCorpseExploded` (C++) / `OnCorpseExplodedBP` (BP) fire with
`(EnemiesHit, TotalDamage)` for VFX/SFX hookup.

### Setup

1. `CorpseExplosionComponent` already exists on `ASOCharacter` — tune
   values in the editor or leave defaults.
2. Set `CorpseExplosionSFX` on `BP_SOCharacter` for audio feedback.
3. Kill an enemy, aim near its corpse, press **X**.

## 43. Shadow Step / Blink

**Files**: `SOBlinkComponent.h/.cpp` (new), `SOCharacter.h/.cpp`,
`SOPlayerController.h/.cpp`, `DefaultInput.ini`, `SOHUD.h/.cpp`

Press **B** to teleport toward the cursor, up to `BlinkRange` (default
650 cm). The destination is snapped to the navmesh when one is present
and validated with `TeleportTo` so the character never lands inside
geometry. On landing, `HealthComponent->bInvulnerable` is set for
`InvulnerabilityDuration` (default 0.18 s), making Blink a viable dodge
as well as a gap-closer.

Key config on `BlinkComponent`:

| Property                  | Default | Notes                                |
|----------------------------|--------|----------------------------------------|
| `BlinkRange`               | 650 cm | Cursor targets beyond this are clamped |
| `ManaCost`                 | 22     | Deducted on a successful blink         |
| `Cooldown`                 | 5 s    | Time between casts                     |
| `InvulnerabilityDuration`  | 0.18 s | I-frame window after landing; 0 disables |
| `NavProjectionExtent`      | 200 cm | Search extent for navmesh snapping     |

`OnBlinked` (C++) / `OnBlinkedBP` (BP) fire with `(FromLocation, ToLocation)`.

### Setup

1. `BlinkComponent` already exists on `ASOCharacter`.
2. Set `BlinkSFX` on `BP_SOCharacter`.
3. Press **B** near a wall or a gap in a monster pack to test the dash.

## 44. Minion Tiers / Evolution

**Files**: `SOMinion.h/.cpp` (added `ESOMinionTier`, `NotifyKill`, `Evolve`)

Every minion tracks its own kill count. After landing `KillsToEvolve`
(default 5) killing blows, a minion evolves from **Base** to **Elite**,
then after the same number of kills again, to **Champion** — the
strongest rank. Each evolution:

- Multiplies `AttackDamage` and `MaxHealth` by `TierStatMultiplier`
  (default 1.35×), and heals to the new max.
- Multiplies `MaxWalkSpeed` by `TierSpeedMultiplier` (default 1.10×).
- Scales the actor by `TierScaleMultiplier` (default 1.15×) so evolved
  minions read as visibly bigger on sight.

Kill detection happens inside `ASOMinion::Tick` — after a melee swing
lands, the minion checks whether its target was alive before the hit
and dead after, crediting the kill without needing extra wiring on
`ASOEnemyCharacter`.

`OnMinionEvolved` (C++) / `OnMinionEvolvedBP` (BP) fire with the new
`ESOMinionTier` for VFX/SFX (recommended: a burst effect + `EvolveSFX`).

### Setup

1. Tune `KillsToEvolve` / `TierStatMultiplier` / `TierSpeedMultiplier` /
   `TierScaleMultiplier` on `BP_SOMinion` (or per minion type).
2. Set `EvolveSFX` for audio feedback.
3. Summon a minion, let it rack up kills, watch it grow.

## 45. Cursed Ground

**Files**: `SOCursedGround.h/.cpp` (new), `SOCharacter.h/.cpp`,
`SOPlayerController.h/.cpp`, `DefaultInput.ini`, `SOHUD.h/.cpp`

Press **H** to place a persistent hazard zone at the cursor. Unlike
`ASOTrap` (which triggers once), `ASOCursedGround` pulses every
`TickInterval` (default 0.75 s) for its full `Duration` (default 8 s),
damaging and debuffing every live `ASOEnemyCharacter` standing inside
`AreaRadius` (default 300 cm) — a zoning tool for denying ground to
packs your minions are wading into.

Key config on `ASOCursedGround`:

| Property        | Default | Notes                                    |
|-----------------|---------|--------------------------------------------|
| `AreaRadius`    | 300 cm  | Hazard zone radius                         |
| `Duration`      | 8 s     | Total lifetime before the zone expires     |
| `TickInterval`  | 0.75 s  | Seconds between damage pulses              |
| `DamagePerTick` | 10      | Damage applied to each enemy per pulse     |
| `bAppliesSlow`  | true    | Refreshes Slowed on every pulse            |
| `bAppliesCurse` | true    | Refreshes Cursed (-25% outgoing dmg) too   |

Key config on `ASOCharacter`:

| Property              | Default | Notes                                  |
|-----------------------|---------|-------------------------------------------|
| `CursedGroundManaCost`| 30      | Deducted on placement                     |
| `MaxCursedGrounds`    | 5       | Oldest zone is destroyed past this cap    |

`OnCursedGroundTick` (C++) / `OnCursedGroundTickBP` (BP) fire with
`(EnemiesAffected, DamageDealt)` on every pulse.

### Setup

1. Assign a `BP_CursedGround` subclass of `ASOCursedGround` to
   `CursedGroundClass` on `BP_SOCharacter`.
2. Set `CursedGroundSFX` for audio feedback.
3. Press **H** ahead of an approaching pack to zone them out.

### HUD changes (Corpse Explosion / Blink / Cursed Ground)

Skill tiles 8-10 (bone-white **X**, shadow-blue **B**, violet **H**)
show mana cost and cooldown overlay using the same tile pattern as the
rest of the skill bar.

## 46. Dodge Roll

**Files**: `SODodgeRollComponent.h/.cpp` (new), `SOCharacter.h/.cpp`,
`SOPlayerController.h/.cpp`, `DefaultInput.ini`, `SOHUD.h/.cpp`

Press **Space** for a free, cooldown-gated evade — the ARPG staple
"get me out of here" button, distinct from Blink (mana-cost teleport
toward the cursor). The roll direction is the character's current
movement direction when moving (so a click-to-move flee reads
naturally), falling back to the direction toward the cursor when
standing still. The roll covers `RollDistance` over `RollDuration` at a
constant speed by directly driving `CharacterMovementComponent::Velocity`
each tick, and the whole duration is invulnerable
(`HealthComponent->bInvulnerable`).

Key config on `DodgeRollComponent`:

| Property       | Default | Notes                                       |
|----------------|---------|-----------------------------------------------|
| `RollDistance` | 450 cm  | Total ground distance covered by one roll      |
| `RollDuration` | 0.28 s  | Time to cover RollDistance (sets roll speed)   |
| `ManaCost`     | 0       | Free by default; exposed in case designers want a cost |
| `Cooldown`     | 1.2 s   | Time between rolls                             |

`OnDodgeRoll` (C++) / `OnDodgeRollBP` (BP) fire with
`(FromLocation, RollDirection)` for VFX/SFX (dust trail, blur, etc.).

### HUD changes

Skill tile 11 (steel-grey, **Space** key) shows the cooldown overlay
using the same tile pattern as the rest of the skill bar.

### Setup

1. `DodgeRollComponent` already exists on `ASOCharacter`.
2. Set `DodgeRollSFX` on `BP_SOCharacter`.
3. Click to move, then tap **Space** mid-flight to dash through danger.

## 47. Item Rarity + Affixes

**Files**: `SOItemAffix.h` (new), `SOItemData.h/.cpp`, `SOLootRoller.h/.cpp`
(new), `SOEnemyCharacter.cpp`

Every enemy loot drop is now rolled into a distinct instance instead of
handing out the shared editor template directly. `USOLootRoller::RollItemInstance`
duplicates the picked `USOItemData` (or `USOWeaponData`/`USOArmorData`
subclass), rolls a rarity tier, and applies a matching number of random
affixes straight onto the instance's existing stat fields
(`PrimaryDamageBonus`, `MaxHealthBonus`, etc.) — nothing downstream
(equipment, inventory, vendor) needed to change since it still just sees
a `USOItemData*` with different numbers.

Rarity odds and affix counts:

| Rarity    | Odds | Affixes |
|-----------|------|---------|
| Common    | 55%  | 0       |
| Magic     | 30%  | 1       |
| Rare      | 12%  | 2       |
| Legendary | 3%   | 4       |

Affix pool is chosen by item type — weapons roll from Primary Damage /
Shadow Bolt Damage / Attack Speed; armor rolls from Max Health / Max
Mana / Movement Speed / Damage Reduction. Magnitude scales with the
template's `ItemLevel`. Each rolled item exposes
`GetAffixDescriptions()` (e.g. `"+18 Primary Damage"`) for tooltip use.

Note: a random roll of Legendary *rarity* just means great stats — it
is distinct from a hand-authored Legendary *Unique* (see below), which
always carries a fixed special effect.

### Setup

1. No changes needed — `ASOEnemyCharacter::RollItemDrop()` already
   calls the roller before spawning the pickup.
2. Tune odds/affix pools in `SOLootRoller.cpp` if you want a different
   drop feel.

## 48. Legendary Uniques

**Files**: `SOItemAffix.h` (new), `SOItemData.h`, `SOCharacter.h/.cpp`,
`SOShadowBoltProjectile.h/.cpp`, `SOCorpseExplosionComponent.h/.cpp`,
`SOSummonComponent.h/.cpp`

Any `USOItemData` (weapon or armor) can be hand-authored as a legendary
unique: set `bIsLegendaryUnique = true`, write a `LegendaryFlavorText`,
and pick a `LegendaryEffect`. `USOLootRoller` detects the flag and
passes such items through unmodified — no rarity reroll, no random
affixes; the designer's numbers are final.

`ASOCharacter::HasLegendaryEffect(Effect)` scans every equipped slot
and returns true if any item carries it. Four effects are wired in:

| Effect                | Hook                                              |
|------------------------|---------------------------------------------------|
| `ShadowBoltChain`      | The bolt chains once to a second nearby enemy for `ChainDamageFraction` (default 50%) of its damage. |
| `CorpseExplosionFree`  | Corpse Explosion costs no mana and its cooldown is halved. |
| `EndlessMinions`       | Summoned/resurrected minions never expire (`SetLifeSpan(0)`). |
| `VampiricStrikes`      | Primary Attack heals the caster for `VampiricStrikesHealFraction` (default 25%) of the swing's total damage. |

### Setup

1. Create a `USOWeaponData`/`USOArmorData` asset, check
   `bIsLegendaryUnique`, and set `LegendaryEffect`.
2. Place it in a loot table or vendor stock like any other item.
3. Equip it — the relevant cast function picks up the effect
   automatically via `HasLegendaryEffect`.

## 49. Waypoint Fast Travel

**Files**: `SOWaypoint.h/.cpp` (new), `SOWaypointComponent.h/.cpp` (new),
`SOCharacter.h/.cpp`, `SOPlayerController.h/.cpp`, `DefaultInput.ini`,
`SOHUD.h/.cpp`

Place `ASOWaypoint` actors around the level. Walking into one's
`DiscoveryRadius` (default 250 cm) registers it with the player's
`USOWaypointComponent`. Press **M** to open the waypoint map — a
Canvas list of every discovered waypoint — then press **5**-**9** to
instantly teleport to the corresponding entry (list is 1-indexed on
screen, 0-indexed internally). Traveling closes the map automatically.

Key config on `ASOWaypoint`:

| Property          | Default | Notes                                          |
|-------------------|---------|---------------------------------------------------|
| `WaypointName`    | —       | Shown in the map list                              |
| `DiscoveryRadius` | 250 cm  | Overlap radius that registers the waypoint         |
| `bStartDiscovered`| false   | Set true for a "home base" waypoint known from the start |
| `TravelOffset`    | (150,0,0) | Landing offset from the marker so travelers don't spawn inside the mesh |

`OnWaypointDiscovered` / `OnWaypointMapToggled` / `OnWaypointTravel`
(C++) and their BP equivalents fire for VFX/SFX/UI hookup.

### HUD changes

A centered panel lists discovered waypoints while the map is open,
with a `"5-9 Travel | M Close"` footer; the control hint bar mentions
**M** for waypoints.

### Setup

1. Drag `ASOWaypoint` (or a BP subclass) into the level at each fast
   travel point; set `WaypointName`.
2. Set `DiscoverSFX` on the waypoint for discovery feedback.
3. Walk near a waypoint, press **M**, then **5** to travel back to it.

### Updated input table

| Key   | Action                          |
|-------|---------------------------------|
| LMB   | MoveTo                          |
| RMB/Q | Primary Attack                  |
| E     | Shadow Bolt                     |
| R     | Life Drain                      |
| T     | Summon Minion (at cursor)       |
| Y     | Dismiss All Minions             |
| C     | Place Trap (selected type)      |
| V     | Cycle Trap Type                 |
| Z     | Activate Overlord Mode          |
| U     | Necromantic Resurrect           |
| X     | Corpse Explosion                |
| B     | Shadow Step / Blink             |
| H     | Place Cursed Ground              |
| Space | Dodge Roll                       |
| M     | Toggle Waypoint Map              |
| 5-9   | Travel to Waypoint 1-5 (map open) |
| F1    | Allocate Strength               |
| F2    | Allocate Intellect              |
| F3    | Allocate Vitality               |
| F5    | Quick Save                      |
| F9    | Quick Load                      |
| F     | Interact / Buy                  |
| G     | Sell Weapon                     |
| ESC   | Toggle Pause                    |
| F10   | Quit (while paused)             |
| K     | Debug Damage Self               |

## 50. Item Sets

**Files**: `SOItemAffix.h` (added `FSOSetBonusTier`), `SOItemSetData.h`
(new), `SOItemData.h` (added `ItemSet`), `SOEquipmentComponent.h/.cpp`,
`SOLootRoller.cpp`, `SOHUD.cpp`

Gives the previously-unused `ESOItemRarity::Set` tier an actual
gameplay effect. A `USOItemSetData` asset defines a name and a ladder
of `FSOSetBonusTier` thresholds (e.g. a 2pc and a 4pc entry, each a
single `FSOItemAffix` stat + value). Any `USOWeaponData`/`USOArmorData`
that references the same `USOItemSetData` via its `ItemSet` field is
part of that set — no ID registry, just a direct asset reference like
`TrapClass` or `MinionClass`.

`USOEquipmentComponent::RecomputeAggregateStats()` (already called on
every equip/unequip) now also counts how many equipped items share
each `ItemSet` and folds in every tier whose `PiecesRequired` is met,
alongside the existing armor-stat aggregation — stacking naturally
with the 2pc bonus staying active once you also meet the 4pc. Only the
four stats armor already supports are valid for set tiers: **Max
Health, Max Mana, Movement Speed, Damage Reduction** (Primary/Shadow
Bolt Damage and Attack Speed aren't wired for sets since those are
per-weapon fields, not character aggregates).

`USOLootRoller` treats any item with a non-null `ItemSet` like a
legendary unique: rarity is forced to `Set` and no random affixes are
rolled — the item's power is its authored base stats plus whatever set
bonus tiers get assembled.

### HUD changes

Active set bonus lines (e.g. `"Bonewalker's Regalia (4pc): +12%
Damage Reduction"`) render in the green Set-rarity tint just below the
attributes panel, sourced from
`EquipmentComponent->GetActiveSetBonusDescriptions()`.

### Setup

1. Create a `USOItemSetData` asset, set `SetName`, and add tiers (e.g.
   `PiecesRequired=2` / `Bonus.Stat=DamageReduction` / `Bonus.Value=0.08`).
2. On each `USOWeaponData`/`USOArmorData` piece meant to belong to the
   set, assign that asset to `ItemSet`.
3. Equip 2+ pieces — the HUD panel and `RecomputeAggregateStats` pick
   it up automatically.

## 51. Treasure Goblin (rare fleeing enemy)

**Files**: `SOEnemyCharacter.h/.cpp` (added flee + loot-burst flags),
`SOEnemyAIController.h/.cpp` (added `Fleeing` state), `SOTreasureGoblinCharacter.h/.cpp`
(new)

Any `ASOEnemyCharacter` can now be told to flee instead of fight —
`bFleeFromPlayer` bypasses the normal Idle/Chasing/Attacking states in
`ASOEnemyAIController::ThinkTick` entirely in favor of a standalone
Idle/Fleeing loop: once the player enters `SightRadius`, it plots a
destination `FleeDistance` cm directly away and re-paths there every
think tick, at `FleeSpeedMultiplier`× movement speed.

`ASOTreasureGoblinCharacter` is a ready-to-place subclass with sensible
defaults baked into the constructor: flees at 1.6× speed, deals no
damage, and dies into a guaranteed, oversized loot burst via two new
general-purpose fields on the base class:

| Property             | Default (Goblin) | Effect                                      |
|----------------------|-------------------|-----------------------------------------------|
| `LootRollCount`      | 6                 | `DropLoot()` rolls the whole `LootTable` this many times |
| `bGuaranteedItemDrop`| true              | `RollItemDrop()` ignores `ItemDropChance`, always attempts a pick from `ItemDropPool` |

Both fields live on the base `ASOEnemyCharacter`, so any enemy can be
turned into an occasional "elite drop" variant without touching code.

### Setup

1. Create a `BP_TreasureGoblin` subclass of `ASOTreasureGoblinCharacter`,
   give it a generous `LootTable`/`ItemDropPool`, and a distinct mesh.
2. Drop it into a wave spawner's rare slot or hand-place it as an
   ambush.
3. Approach it — it flees; landing a kill (Shadow Bolt, Corpse
   Explosion, a well-timed Trap) pays off with a loot pile.

## 52. Difficulty Tiers (New Game+ style)

**Files**: `SODifficultySubsystem.h/.cpp` (new), `SOEnemyCharacter.cpp`,
`SOPlayerController.h/.cpp`, `DefaultInput.ini`, `SOHUD.h/.cpp`

`USODifficultySubsystem` is a `UGameInstanceSubsystem` holding one
global `ESODifficultyTier` (Normal/Hard/Nightmare/Torment). Press **O**
to cycle it. `ASOEnemyCharacter::BeginPlay` reads the active tier's
multipliers *before* calling `Super::BeginPlay()` — critical, since
`USOHealthComponent` snaps `CurrentHealth` to `MaxHealth` inside its
own `BeginPlay`, so the scaling has to land first — and applies them to
`MaxHealth`, `AttackDamage`, `XPReward`, and `ItemDropChance`. Gold
orbs get `GetGoldMultiplier()` applied at spawn in `DropLoot()`.

Only enemies spawned *after* a difficulty change are affected —
already-alive enemies keep their original stats, matching how
"restart on a harder tier" difficulty selection works in Diablo-likes.

| Tier      | Enemy HP | Enemy Damage | XP    | Gold  | Item Drop Chance |
|-----------|----------|--------------|-------|-------|-------------------|
| Normal    | 1.0×     | 1.0×         | 1.0×  | 1.0×  | 1.0×              |
| Hard      | 1.8×     | 1.3×         | 1.25× | 1.15× | 1.1×              |
| Nightmare | 3.0×     | 1.8×         | 1.6×  | 1.35× | 1.25×             |
| Torment   | 5.0×     | 2.5×         | 2.2×  | 1.6×  | 1.5×              |

### HUD changes

A "Difficulty: Nightmare"-style label renders under the gold counter,
colored per tier (grey/gold/orange/red).

### Setup

1. No per-actor setup needed — every `ASOEnemyCharacter` picks up the
   active tier automatically on spawn.
2. Press **O** before starting a run (or mid-run, for subsequently
   spawned enemies) to raise the stakes.
3. Tune the multiplier tables in `SODifficultySubsystem.cpp` to taste.

## 53. Talent Respec

**Files**: `SOTalentComponent.h/.cpp` (added `RespecAll`), `SOCharacter.h/.cpp`,
`SOPlayerController.h/.cpp`, `DefaultInput.ini`

Press **P** to respec: every unlocked talent node is reverted (its
`FSOTalentEffect`s undone via `RevertNodeEffects`, the exact inverse of
`ApplyNodeEffects`) and its `PointCost` refunded, for a flat
`RespecGoldCost` (default 50 gold) charged via `ASOCharacter::RespecTalents`.
No-op if nothing is unlocked or gold is short.

### Setup

1. `RespecGoldCost` is already set on `ASOCharacter` — tune or zero it
   out for a free respec.
2. Set `RespecSFX` on `BP_SOCharacter` for audio feedback.
3. Unlock a few nodes, press **P**, confirm the points return and
   stats revert.

### Updated input table

| Key   | Action                          |
|-------|---------------------------------|
| LMB   | MoveTo                          |
| RMB/Q | Primary Attack                  |
| E     | Shadow Bolt                     |
| R     | Life Drain                      |
| T     | Summon Minion (at cursor)       |
| Y     | Dismiss All Minions             |
| C     | Place Trap (selected type)      |
| V     | Cycle Trap Type                 |
| Z     | Activate Overlord Mode          |
| U     | Necromantic Resurrect           |
| X     | Corpse Explosion                |
| B     | Shadow Step / Blink             |
| H     | Place Cursed Ground              |
| Space | Dodge Roll                       |
| M     | Toggle Waypoint Map              |
| 5-9   | Travel to Waypoint 1-5 (map open) |
| P     | Respec All Talents               |
| O     | Cycle Difficulty Tier             |
| L     | Toggle Bestiary / Kill Codex      |
| F1    | Allocate Strength               |
| F2    | Allocate Intellect              |
| F3    | Allocate Vitality               |
| F5    | Quick Save                      |
| F9    | Quick Load                      |
| F     | Interact / Buy                  |
| G     | Sell Weapon                     |
| ESC   | Toggle Pause                    |
| F10   | Quit (while paused)             |
| K     | Debug Damage Self               |

## 54. Gem Sockets

**Files**: `SOGemData.h` (new), `SOItemData.h` (added `MaxSockets`/`SocketedGems`),
`SOEquipmentComponent.h/.cpp` (added `SocketGem`), `SOLootRoller.h/.cpp`
(`ApplyAffix` made public)

Any weapon/armor can have `MaxSockets` gem slots. A `USOGemData` asset
(Chipped→Perfect quality ladder, purely presentational) defines one
`ESOAffixStat` + `Value`, exactly like a rolled affix.
`USOEquipmentComponent::SocketGem(Slot, Gem)` bakes that bonus into the
equipped item's own stat fields via `USOLootRoller::ApplyAffix` — the
same routine random drop affixes use — then calls
`RecomputeAggregateStats()` so armor-targeted gems (Health/Mana/Speed/
Damage Reduction) take effect immediately; weapon-targeted gems
(Primary/Shadow Bolt Damage/Attack Speed) are already read live via
`GetEffective*Damage()`.

No new key binding — sockets are meant to be filled from an inventory
screen (`Equipment->SocketGem(...)`), not cast like a spell.

### Setup

1. Create `USOGemData` assets per tier (e.g. a "Flawless Ruby":
   `Stat=PrimaryDamage`, `Value=15`).
2. Set `MaxSockets` on the weapon/armor templates that should be
   socketable.
3. Call `EquipmentComponent->SocketGem(ESOEquipSlot::MainHand, Gem)`
   from your inventory UI once it exists.

## 55. Familiar Companion

**Files**: `SOFamiliarActor.h/.cpp` (new), `SOCharacter.h/.cpp`

`ASOFamiliarActor` is a permanent hovering companion — distinct from
`ASOMinion`: it never expires, doesn't count against the minion cap,
and doesn't melee. It follows the owner at a fixed offset (with a
gentle bob) and periodically zaps the nearest enemy in range for light
chip damage. Set `FamiliarClass` on `ASOCharacter` and it spawns once,
automatically, in `BeginPlay` — no key needed, it's a passive
companion rather than a cast ability.

| Property             | Default | Notes                                    |
|-----------------------|---------|---------------------------------------------|
| `FollowOffset`        | (-150,100,120) | Hover position relative to the owner  |
| `FollowInterpSpeed`   | 4.0     | How snappily it catches up               |
| `ZapDamage`           | 8       | Damage per zap                            |
| `ZapRange`            | 900 cm  | Max range to the nearest enemy            |
| `ZapInterval`         | 2 s     | Seconds between zaps                      |

### Setup

1. Create a `BP_Familiar` subclass of `ASOFamiliarActor` with a fitting
   mesh (imp, raven, floating skull, ...).
2. Assign it to `FamiliarClass` on `BP_SOCharacter`.
3. Start a level — the familiar spawns and starts zapping nearby
   enemies automatically.

## 56. Hit-Stop / Camera Shake Juice

**Files**: `SOCharacter.h/.cpp`

Cosmetic "game feel" polish: `ASOCharacter::TriggerHitImpact(ShakeScale)`
plays a `HitCameraShakeClass` camera shake and briefly sets
`UGameplayStatics::SetGlobalTimeDilation` to `HitStopTimeDilation`
(default 0.05) for `HitStopDuration` real-world seconds (default
0.045s) before restoring normal speed. Because timers advance on
*dilated* world time, the restore timer's duration is pre-multiplied by
the dilation factor (`HitStopDuration * HitStopTimeDilation`) — passing
the raw real-seconds value would make the "restore" timer itself run in
slow motion and never recover in time.

Wired into two hooks automatically:
- **Landing a Primary Attack hit** — `ShakeScale = 0.5` (punchy but subtle).
- **Taking any damage** (`HealthComponent->OnHealthChanged`, `Delta < 0`)
  — `ShakeScale = 1.0` (more pronounced, since getting hit should read
  clearly).

Call `TriggerHitImpact()` manually from any other cast function
(Corpse Explosion, a boss nova, ...) for the same feedback elsewhere.

### Setup

1. Create a `UCameraShakeBase` Blueprint (or use a `UMatineeCameraShake`
   subclass) and assign it to `HitCameraShakeClass` on `BP_SOCharacter`.
2. Tune `HitStopTimeDilation`/`HitStopDuration` to taste — smaller
   dilation and longer duration reads as a much harder-hitting freeze.

## 57. Bestiary / Kill Codex

**Files**: `SOBestiaryComponent.h/.cpp` (new), `SOEnemyCharacter.h/.cpp`
(added `BestiaryDisplayName`), `SOCharacter.h/.cpp`,
`SOPlayerController.h/.cpp`, `DefaultInput.ini`, `SOHUD.h/.cpp`

Tracks kills per enemy *class* (not per-instance) for simple meta-
progression. `ASOEnemyCharacter::HandleDeath` calls
`Killer->BestiaryComponent->RecordKill(GetClass())` alongside the
existing XP/quest/corruption grants. Press **L** to open/close a
Canvas codex overlay listing every discovered species by kill count,
descending, using each class's `BestiaryDisplayName` (falls back to
the raw class name if left blank).

### Setup

1. Set `BestiaryDisplayName` on each `BP_Enemy*` subclass (e.g.
   "Skeleton Grunt", "Shadow Wraith") for a clean codex entry.
2. Kill a few enemies, press **L** to check the tally.
3. `BestiaryComponent->GetEntryDescriptions()` / `GetTotalKills()` /
   `GetDiscoveredSpeciesCount()` are all available for a future
   achievements/rewards layer.

## 58. Elemental Resistances (completing a dormant system)

**Files**: `SOHealthComponent.h/.cpp`

`USODamageType` already carried an `ESODamageCategory` (`Physical`,
`Shadow`, `Fire`, `Frost`, `Necrotic`, `Holy`, `True`) and a
`bIgnoresResistances` flag from an earlier pass — but `HandleAnyDamage`
never actually read either one; the `DamageType` parameter was unused
and every hit only ever applied `IncomingDamageMultiplier`. Both fields
are now wired up for real:

- `USOHealthComponent::ElementalResistances` is a
  `TMap<ESODamageCategory, float>` (0.0-0.9 fractional reduction per
  school), read via `GetResistance(Category)` / written via
  `SetResistance(Category, Value)` (handy for temporary resistance
  buffs/potions).
- `HandleAnyDamage` now casts the incoming `DamageType` to
  `USODamageType`, and — unless `bIgnoresResistances` is set or the
  category is `True` — multiplies the (already `IncomingDamageMultiplier`-
  scaled) damage by `(1 - resistance)`.
- `bIgnoresResistances` now also skips `IncomingDamageMultiplier`
  itself, matching what its doc comment always said it should do.

### Setup

1. Tag each elemental `BP_DamageType_*` subclass with the right
   `Category` (this may already be done from the original pass).
2. Set `ElementalResistances` on a character/enemy's `HealthComponent`
   (e.g. `Frost -> 0.3` for a fire-elemental boss that resists cold).
3. Mark any "true damage" source (execute effects, debug damage) with
   a `USODamageType` subclass whose `Category = True` or
   `bIgnoresResistances = true` to bypass all of this.

## 59. Potions / Quick-Use Consumables

**Files**: `SOConsumableComponent.h/.cpp` (new), `SOCharacter.h/.cpp`,
`SOPlayerController.h/.cpp`, `DefaultInput.ini`, `SOHUD.h/.cpp`,
`SOWaypointComponent.cpp`

Press **I** to consume one potion charge, healing/restoring mana by a
fraction of max (`HealFraction`/`ManaFraction`, defaults 35%/25%) on a
short `UseCooldown` (default 3s) so it can't be spammed. Charges are a
fixed pool (`MaxCharges`, default 4) refilled by
`RefillCharges()` — wired automatically into
`USOWaypointComponent::TravelToWaypoint`, so arriving at any waypoint
tops potions back up, Diablo-town-portal style, instead of needing a
shop purchase per use.

### HUD changes

Skill tile 12 (deep red, **I** key) shows the cooldown overlay plus a
`"charges / max"` label underneath, same pattern as the Summon Minion
count badge.

### Setup

1. `ConsumableComponent` already exists on `ASOCharacter` — tune
   `MaxCharges`/`HealFraction`/`ManaFraction`/`UseCooldown` to taste.
2. Set `PotionUseSFX` on `BP_SOCharacter`.
3. Drink potions in a fight, then travel to any waypoint to top back up.

## 60. Achievements

**Files**: `SOAchievementComponent.h/.cpp` (new), `SOCharacter.h/.cpp`,
`SOHUD.h/.cpp`

`USOAchievementComponent` is a generic unlock-by-`FName`-ID tracker
with a HUD toast — it doesn't own any milestone *definitions* itself;
anything can call `AchievementComponent->UnlockAchievement(ID,
DisplayName)` and it dedupes, remembers, and shows a 3.5s "ACHIEVEMENT
UNLOCKED: ..." banner across the top of the screen.

Six concrete milestones are wired as a worked example, checked from
existing delegate handlers already on `ASOCharacter`:

| ID           | Trigger                                    |
|--------------|---------------------------------------------|
| `first_blood`| First kill ever (via `BestiaryComponent`)     |
| `centurion`  | 100 total kills                              |
| `level_10`   | Reach character level 10                     |
| `level_25`   | Reach character level 25                     |
| `rich`       | Accumulate 1000 gold                         |
| `explorer`   | Discover 5 waypoints                         |

### Setup

1. No setup needed for the six wired-in examples — play normally and
   watch the toasts.
2. Add more by calling `UnlockAchievement` from any other event hook
   (quest completion, boss defeat, crafting a Legendary, ...).
3. Set `UnlockSFX` on `BP_SOCharacter`'s `AchievementComponent` for
   audio feedback.

## 61. Hit-React Strength (completing another dormant field) + Boss Enrage

**Files**: `SOHealthComponent.h/.cpp`, `SOCharacter.cpp`,
`SOBossCharacter.h/.cpp`, `SOHUD.cpp`

Same story as the elemental-resistance fix (§58): `USODamageType` has
carried `HitReactStrength` (doc'd as "hit-react animation/VFX
intensity") since an early pass, but nothing ever read it.
`USOHealthComponent` now caches `LastHitReactStrength` from whichever
`USODamageType` last actually landed damage, and
`ASOCharacter::HandleHealthChanged` scales its hit-stop camera shake by
it — a heavy boss slam now visibly shakes the screen harder than a
weak poison tick, using the exact same field a future hit-react
animation system would also read.

**Boss Enrage** is a new, standard anti-turtling mechanic: after
`EnrageTime` seconds (default 180s) the boss permanently gets
`EnrageDamageMult`/`EnrageSpeedMult` (default 2.0x/1.5x) stronger and
is forced into Phase 3 if it wasn't already there — `EnterPhase`
always rescales from the cached base stats, so forcing Phase 3 first
and multiplying after keeps the two effects additive instead of one
clobbering the other. `EnrageTime = 0` disables it entirely.

### HUD changes

The boss bar now shows a countdown ("Enrage in 42s") beneath the
nameplate, replaced by a bright red "ENRAGED!" tag once it triggers.

### Setup

1. No setup needed for hit-react — it's automatic wherever
   `USODamageType::HitReactStrength` is already authored.
2. Tune `EnrageTime`/`EnrageDamageMult`/`EnrageSpeedMult` per boss, or
   set `EnrageTime = 0` for fights that shouldn't enrage.
3. Set `EnrageSFX` for a roar/audio cue the moment it triggers.

## 62. Death / Respawn (closing a long-standing gap)

**Files**: `SOCharacter.h/.cpp`, `SOWaypointComponent.h/.cpp`, `SOHUD.cpp`

Until now, dying had no way out: `OnCharacterDied` froze movement and
showed the "YOU DIED" overlay, and its own comment said "actual
respawn / game-over UI is intentionally left to a later system" — that
system never arrived, so death was a dead end (pun intended) short of
manually pressing Quick Load.

`HandleDeath` now schedules `Respawn()` automatically after
`RespawnDelay` seconds (default 3s, `bAutoRespawn` toggles this off
for a hard "game over" feel instead). `Respawn()`:

- Charges `RespawnGoldPenaltyFraction` (default 10%) of current gold —
  a Diablo-style death penalty with actual teeth once gold matters.
- Revives to full HP, refills mana and potion charges.
- Restores capsule collision and movement mode.
- Teleports to `WaypointComponent->GetLastWaypoint()` — the most
  recently *discovered or traveled-to* waypoint, tracked by
  `USOWaypointComponent` for exactly this purpose. Stays in place if no
  waypoint has been found yet.

### HUD changes

While dead, a "Respawning in Ns..." countdown appears beneath the
"YOU DIED" text (only shown when `bAutoRespawn` is on).

### Setup

1. Place at least one `ASOWaypoint` (ideally with `bStartDiscovered =
   true`) near the start of a level so an early death has somewhere to
   respawn to.
2. Tune `RespawnDelay`/`RespawnGoldPenaltyFraction` on `BP_SOCharacter`,
   or set `bAutoRespawn = false` for a stricter "no continue" mode.
3. `OnCharacterRespawned` is a BP event for respawn VFX/SFX/UI.

## 63. Reality Slash (Overlord-flavored instant kill)

**Files**: `SORealitySlashDamageType.h/.cpp` (new),
`SORealitySlashComponent.h/.cpp` (new), `SOCharacter.h/.cpp`,
`SOPlayerController.h/.cpp`, `DefaultInput.ini`, `SOHUD.h/.cpp`

Straight out of the source material: Ainz's signature spells
("Grasp Heart", "Reality Slash") instantly kill ordinary monsters but
explicitly fail against other "Players" — bosses. Press **J** to
slash the nearest enemy to the cursor:

- **Ordinary enemy** (not a boss, no active `USOEliteComponent`
  affixes): dies outright via `USOHealthComponent::Kill()`. Because
  `Kill()` drives the exact same `OnDeath` flow as a normal kill,
  XP/loot/corruption/bestiary-codex/quest-credit all fire automatically
  — no special-casing needed anywhere else.
- **Boss or elite** (resists the instant kill): takes a `BossFallbackDamage`
  (default 400) burst of `USORealitySlashDamageType` — a damage type
  that "cuts through reality itself" (`Category = True`,
  `bIgnoresResistances = true`), so it bypasses every elemental
  resistance from §58. Doubles as a template for authoring further
  elemental `USODamageType` subclasses.

Mana cost 60, cooldown 8s, range 700cm — tuned as a powerful but
deliberate cooldown-gated tool rather than a spammable execute.

### HUD changes

Skill tile 13 (void-black, **J** key) shows mana cost and cooldown
overlay using the same tile pattern as the rest of the skill bar.

### Setup

1. `RealitySlashComponent` already exists on `ASOCharacter` — tune
   `Range`/`ManaCost`/`Cooldown`/`BossFallbackDamage` to taste.
2. Optionally author further `USODamageType` subclasses (elemental
   schools) following `SORealitySlashDamageType.cpp` as a template.
3. Set `RealitySlashSFX` on `BP_SOCharacter` for audio feedback.
4. Slash a regular enemy (dies instantly), then a boss (takes heavy
   True damage instead) to feel the difference.

### Updated input table

| Key   | Action                          |
|-------|---------------------------------|
| LMB   | MoveTo                          |
| RMB/Q | Primary Attack                  |
| E     | Shadow Bolt                     |
| R     | Life Drain                      |
| T     | Summon Minion (at cursor)       |
| Y     | Dismiss All Minions             |
| C     | Place Trap (selected type)      |
| V     | Cycle Trap Type                 |
| Z     | Activate Overlord Mode          |
| U     | Necromantic Resurrect           |
| X     | Corpse Explosion                |
| B     | Shadow Step / Blink             |
| H     | Place Cursed Ground              |
| Space | Dodge Roll                       |
| M     | Toggle Waypoint Map              |
| 5-9   | Travel to Waypoint 1-5 (map open) |
| P     | Respec All Talents               |
| O     | Cycle Difficulty Tier             |
| L     | Toggle Bestiary / Kill Codex      |
| I     | Use Potion                        |
| J     | Reality Slash                     |
| F1    | Allocate Strength               |
| F2    | Allocate Intellect              |
| F3    | Allocate Vitality               |
| F5    | Quick Save                      |
| F9    | Quick Load                      |
| F     | Interact / Buy                  |
| G     | Sell Weapon                     |
| ESC   | Toggle Pause                    |
| F10   | Quit (while paused)             |
| K     | Debug Damage Self               |
