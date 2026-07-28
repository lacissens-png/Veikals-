#include "SOHazardZone.h"

#include "Components/BoxComponent.h"
#include "SOStatusEffectComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

ASOHazardZone::ASOHazardZone()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(200.0f, 200.0f, 50.0f));
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAll"));
	TriggerBox->SetGenerateOverlapEvents(true);
	RootComponent = TriggerBox;

	GroundDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("GroundDecal"));
	GroundDecal->SetupAttachment(RootComponent);
	GroundDecal->DecalSize = FVector(50.0f, 200.0f, 200.0f);
	GroundDecal->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	AmbientFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AmbientFX"));
	AmbientFX->SetupAttachment(RootComponent);
	AmbientFX->bAutoActivate = true;
}

void ASOHazardZone::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASOHazardZone::OnBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this,   &ASOHazardZone::OnEndOverlap);

	// Apply tint to decal material instance
	if (DecalMaterial)
	{
		GroundDecal->SetDecalMaterial(DecalMaterial);
	}

	if (UMaterialInstanceDynamic* DynMat = GroundDecal->CreateDynamicMaterialInstance())
	{
		DynMat->SetVectorParameterValue(TEXT("Color"), GetHazardColor());
	}

	if (AmbientParticles)
	{
		AmbientFX->SetTemplate(AmbientParticles);
	}

	// Start damage ticking
	GetWorldTimerManager().SetTimer(DamageTickHandle, this, &ASOHazardZone::DamageTick,
	                                TickInterval, /*bLoop=*/true, TickInterval);

	// Optional lifetime
	if (Lifetime > 0.0f)
	{
		GetWorldTimerManager().SetTimer(LifetimeHandle, [this]()
		{
			OnZoneExpired();
			Destroy();
		}, Lifetime, /*bLoop=*/false);
	}
}

void ASOHazardZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(DamageTickHandle);
	GetWorldTimerManager().ClearTimer(LifetimeHandle);
	Super::EndPlay(EndPlayReason);
}

void ASOHazardZone::OnBeginOverlap(UPrimitiveComponent*, AActor* OtherActor,
                                    UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (OtherActor && OtherActor != this)
	{
		ActorsInside.AddUnique(OtherActor);
	}
}

void ASOHazardZone::OnEndOverlap(UPrimitiveComponent*, AActor* OtherActor,
                                  UPrimitiveComponent*, int32)
{
	ActorsInside.Remove(OtherActor);

	// Apply a lingering debuff to actors that leave the zone.
	if (bApplyLingeringStatus && IsValid(OtherActor))
	{
		if (USOStatusEffectComponent* SFX = OtherActor->FindComponentByClass<USOStatusEffectComponent>())
		{
			SFX->ApplyEffect(LingeringStatusType, LingeringDuration,
			                 LingeringDamagePerTick, LingeringTickInterval);
		}
	}
}

void ASOHazardZone::DamageTick()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TSubclassOf<UDamageType> DMGClass = GetDamageTypeClass();

	// Iterate a copy in case damage triggers BeginOverlap/EndOverlap
	TArray<TObjectPtr<AActor>> Snapshot = ActorsInside;
	for (AActor* Actor : Snapshot)
	{
		if (!IsValid(Actor))
		{
			continue;
		}

		UGameplayStatics::ApplyDamage(Actor, DamagePerTick, nullptr, this, DMGClass);

		if (HitFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, HitFX, Actor->GetActorLocation(),
			                                         FRotator::ZeroRotator, true);
		}
	}

	// Remove stale entries
	ActorsInside.RemoveAll([](const TObjectPtr<AActor>& A) { return !IsValid(A); });
}

TSubclassOf<UDamageType> ASOHazardZone::GetDamageTypeClass() const
{
	// All hazard types route through SODamageType; category is baked into the class.
	// BP can subclass SODamageType per hazard for more granular resist checks.
	return UDamageType::StaticClass();
}

FLinearColor ASOHazardZone::GetHazardColor() const
{
	switch (HazardType)
	{
	case ESOHazardType::Fire:      return FLinearColor(1.0f, 0.3f, 0.0f, 1.0f);
	case ESOHazardType::Poison:    return FLinearColor(0.2f, 0.8f, 0.1f, 1.0f);
	case ESOHazardType::Frost:     return FLinearColor(0.4f, 0.8f, 1.0f, 1.0f);
	case ESOHazardType::Lightning: return FLinearColor(1.0f, 0.95f, 0.2f, 1.0f);
	case ESOHazardType::Necrotic:  return FLinearColor(0.5f, 0.0f, 0.8f, 1.0f);
	default:                       return FLinearColor::White;
	}
}
