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
