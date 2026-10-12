// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// ECLIPSE PUPPETEER  -  Resource: Puppet Strings / Lunar Essence (Spender)
//   Puppet Master's Thread   damage-links up to 5 enemies; damage to one is mirrored
//                            at 70% onto every other linked enemy.
//   Creation                 SUPER-TIER ULTIMATE: spawns a domain that overrides the
//                            field affinity (UEclipseFieldAffinitySubsystem).
//   Gain Unstoppable         rune-effect ability (GainUnstoppable).

#pragma once

#include "Abilities/EclipseGameplayAbility.h"
#include "CoreMinimal.h"

#include "EclipsePuppeteerAbilities.generated.h"

class AEclipseCreationDomain;

UCLASS()
class ECLIPSEGAME_API UGA_PuppetMastersThread : public UEclipseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_PuppetMastersThread();

protected:
	virtual void OnAbilityCommitted() override;

	UPROPERTY(EditDefaultsOnly, Category = "Puppet Thread") float CastRange = 1100.f;
	/** Links spread from the primary target to its neighbours within this radius. */
	UPROPERTY(EditDefaultsOnly, Category = "Puppet Thread") float LinkRadius = 700.f;
	UPROPERTY(EditDefaultsOnly, Category = "Puppet Thread", meta = (ClampMin = "2")) int32 MaxLinks = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Puppet Thread", meta = (ClampMin = "0", ClampMax = "1")) float ShareFraction = 0.7f;
	UPROPERTY(EditDefaultsOnly, Category = "Puppet Thread") float Duration = 8.f;
	UPROPERTY(EditDefaultsOnly, Category = "Puppet Thread") float InitialDamage = 150.f;
};

UCLASS()
class ECLIPSEGAME_API UGA_Creation : public UEclipseGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Creation();

protected:
	virtual void OnAbilityCommitted() override;

	UPROPERTY(EditDefaultsOnly, Category = "Creation") TSubclassOf<AEclipseCreationDomain> DomainClass;
	UPROPERTY(EditDefaultsOnly, Category = "Creation") EEclipseAffinity OverrideAffinity = EEclipseAffinity::Lunar;
	UPROPERTY(EditDefaultsOnly, Category = "Creation") float Radius = 2500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Creation") float Duration = 15.f;
	UPROPERTY(EditDefaultsOnly, Category = "Creation") float ResonanceScalar = 1.5f;
};

UCLASS()
class ECLIPSEGAME_API UGA_GainUnstoppable : public UEclipseGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void OnAbilityCommitted() override;

	UPROPERTY(EditDefaultsOnly, Category = "Unstoppable") float DefaultDuration = 3.f;
};
