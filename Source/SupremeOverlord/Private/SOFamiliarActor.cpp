#include "SOFamiliarActor.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SOCharacter.h"
#include "SODamageType.h"
#include "SOEnemyCharacter.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

ASOFamiliarActor::ASOFamiliarActor()
{
	PrimaryActorTick.bCanEverTick = true;

	FamiliarRoot = CreateDefaultSubobject<USceneComponent>(TEXT("FamiliarRoot"));
	SetRootComponent(FamiliarRoot);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(FamiliarRoot);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetWorldScale3D(FVector(0.3f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BasicSphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (BasicSphere.Succeeded())
	{
		BodyMesh->SetStaticMesh(BasicSphere.Object);
	}
}

void ASOFamiliarActor::BeginPlay()
{
	Super::BeginPlay();

	if (ZapInterval > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ZapTimerHandle, this, &ASOFamiliarActor::PerformZap, ZapInterval, true);
	}
}

void ASOFamiliarActor::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ZapTimerHandle);
	}
	Super::EndPlay(Reason);
}

void ASOFamiliarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeAlive += DeltaTime;

	ASOCharacter* OwnerChar = OwnerCharacter.Get();
	if (!OwnerChar)
	{
		return;
	}

	const FVector Forward = OwnerChar->GetActorForwardVector();
	const FVector Right   = OwnerChar->GetActorRightVector();

	FVector TargetLocation = OwnerChar->GetActorLocation()
		+ Forward * FollowOffset.X
		+ Right   * FollowOffset.Y
		+ FVector(0.0f, 0.0f, FollowOffset.Z);

	if (BobAmplitude > 0.0f)
	{
		TargetLocation.Z += FMath::Sin(TimeAlive * BobRateHz * 2.0f * PI) * BobAmplitude;
	}

	const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, FollowInterpSpeed);
	SetActorLocation(NewLocation);
}

void ASOFamiliarActor::PerformZap()
{
	ASOCharacter* OwnerChar = OwnerCharacter.Get();
	if (!OwnerChar || !OwnerChar->IsAlive())
	{
		return;
	}

	TArray<AActor*> All;
	UGameplayStatics::GetAllActorsOfClass(this, ASOEnemyCharacter::StaticClass(), All);

	ASOEnemyCharacter* Target = nullptr;
	float BestDistSq = ZapRange * ZapRange;

	for (AActor* A : All)
	{
		ASOEnemyCharacter* Enemy = Cast<ASOEnemyCharacter>(A);
		if (!Enemy || !Enemy->IsAlive())
		{
			continue;
		}
		const float DistSq = FVector::DistSquared(GetActorLocation(), Enemy->GetActorLocation());
		if (DistSq <= BestDistSq)
		{
			BestDistSq = DistSq;
			Target     = Enemy;
		}
	}

	if (!Target)
	{
		return;
	}

	TSubclassOf<UDamageType> DTClass = ZapDamageType
		? ZapDamageType
		: TSubclassOf<UDamageType>(USODamageType::StaticClass());

	UGameplayStatics::ApplyDamage(Target, ZapDamage, OwnerChar->GetController(), this, DTClass);

	if (ZapSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ZapSFX, GetActorLocation());
	}

	OnZapPerformed(Target);
}
