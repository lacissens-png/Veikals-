// Copyright Eclipse of the Primordials. All Rights Reserved.

#pragma once

#include "Combat/EclipseCombatTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EclipseCreationDomain.generated.h"

/**
 * Eclipse Puppeteer ultimate "Creation": a replicated, timed sphere that
 * overrides the field affinity inside it. Registration happens on every
 * machine in BeginPlay, so clients can render the domain and preview modifiers.
 */
UCLASS()
class ECLIPSEGAME_API AEclipseCreationDomain : public AActor
{
	GENERATED_BODY()

public:
	AEclipseCreationDomain();

	/** Server: call between SpawnActorDeferred and FinishSpawning. */
	void InitDomain(EEclipseAffinity InAffinity, float InRadius, float InDuration, float InResonance);

	bool Contains(const FVector& Location) const { return FVector::DistSquared2D(Location, GetActorLocation()) <= Radius * Radius; }
	EEclipseAffinity GetOverrideAffinity() const { return OverrideAffinity; }
	float GetResonanceScalar() const { return ResonanceScalar; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Creation")
	EEclipseAffinity OverrideAffinity = EEclipseAffinity::Lunar;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Creation")
	float Radius = 2500.f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Creation")
	float ResonanceScalar = 1.5f;
};
