# Supreme Overlord — Core Framework Setup

This branch scaffolds the initial C++ framework for an isometric ARPG in Unreal Engine 5.4:

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
