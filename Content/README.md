# Content

This folder is where the Unreal Editor stores all `.uasset`/`.umap` binary
content: Blueprint subclasses (`BP_SOCharacter`, `BP_Trap`, `BP_CursedGround`,
`BP_Minion`, `BP_TreasureGoblin`, ...), data assets (`USOWeaponData`,
`USOArmorData`, `USOItemSetData`, `USOTalentNode`, ...), maps, materials, and
any other editor-authored assets referenced throughout `SETUP.md`.

It starts empty on purpose — every gameplay system in this project lives in
`Source/SupremeOverlord/` as plain C++ with `UPROPERTY`/`UFUNCTION` hooks, so
none of it depends on pre-existing content. Open the project in Unreal Editor
5.8 and start placing Blueprints/assets here following the setup steps in
`SETUP.md`.

Binary content in this folder is intentionally excluded from `.gitignore`
(only the engine's generated `Saved/`, `Intermediate/`, and `Binaries/`
folders are ignored) — once you add `.uasset`/`.umap` files here, commit them
normally.
