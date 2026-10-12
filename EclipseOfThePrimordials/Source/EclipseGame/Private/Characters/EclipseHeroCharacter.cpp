// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "Characters/EclipseHeroCharacter.h"

#include "Abilities/EclipseGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Characters/EclipsePlayerState.h"
#include "Combat/EclipseAttributeSet.h"
#include "Combat/EclipseStatusComponent.h"
#include "Runes/EclipseRunewordComponent.h"

AEclipseHeroCharacter::AEclipseHeroCharacter()
{
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	Attributes = CreateDefaultSubobject<UEclipseAttributeSet>(TEXT("Attributes"));
	Status = CreateDefaultSubobject<UEclipseStatusComponent>(TEXT("Status"));
	Runewords = CreateDefaultSubobject<UEclipseRunewordComponent>(TEXT("Runewords"));

	// Isometric camera: no controller rotation; movement orients the mesh.
	bUseControllerRotationYaw = false;
}

UAbilitySystemComponent* AEclipseHeroCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

UEclipseClassDefinition* AEclipseHeroCharacter::GetClassDefinition() const
{
	const AEclipsePlayerState* PS = GetPlayerState<AEclipsePlayerState>();
	return PS ? PS->GetClassDefinition() : nullptr;
}

EEclipseResourceModel AEclipseHeroCharacter::GetResourceModel() const
{
	const UEclipseClassDefinition* Def = GetClassDefinition();
	return Def ? Def->ResourceModel : EEclipseResourceModel::Spender;
}

void AEclipseHeroCharacter::InitAbilityActorInfo()
{
	AbilitySystem->InitAbilityActorInfo(this, this);
}

void AEclipseHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); // Server only.
	InitAbilityActorInfo();

	if (AEclipsePlayerState* PS = GetPlayerState<AEclipsePlayerState>())
	{
		GrantClass(PS->GetClassDefinition());
		Runewords->BindToInventory(PS->GetInventory());
	}
}

void AEclipseHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); // Clients: actor info must be valid for prediction.
	InitAbilityActorInfo();
}

void AEclipseHeroCharacter::GrantClass(UEclipseClassDefinition* ClassDef)
{
	if (!HasAuthority() || !ClassDef || bClassGranted)
	{
		return;
	}
	bClassGranted = true;

	// Base attributes (server; replicate down via the AttributeSet).
	AbilitySystem->SetNumericAttributeBase(UEclipseAttributeSet::GetMaxHealthAttribute(), ClassDef->BaseMaxHealth);
	AbilitySystem->SetNumericAttributeBase(UEclipseAttributeSet::GetHealthAttribute(), ClassDef->BaseMaxHealth);
	AbilitySystem->SetNumericAttributeBase(UEclipseAttributeSet::GetArmorAttribute(), ClassDef->BaseArmor);
	AbilitySystem->SetNumericAttributeBase(UEclipseAttributeSet::GetMaxClassResourceAttribute(), ClassDef->MaxResource);
	AbilitySystem->SetNumericAttributeBase(UEclipseAttributeSet::GetClassResourceAttribute(),
		ClassDef->ResourceModel == EEclipseResourceModel::Spender ? ClassDef->MaxResource : ClassDef->StartingResource);

	auto Grant = [this](const FEclipseAbilityGrant& G)
	{
		if (G.Ability)
		{
			FGameplayAbilitySpec Spec(G.Ability, 1, INDEX_NONE, this);
			Spec.DynamicAbilityTags.AddTag(G.InputTag); // Input routing by tag.
			AbilitySystem->GiveAbility(Spec);
		}
	};
	for (const FEclipseAbilityGrant& G : ClassDef->Abilities)
	{
		Grant(G);
	}
	Grant(ClassDef->Ultimate);

	for (const TSubclassOf<UGameplayEffect>& Effect : ClassDef->PassiveEffects)
	{
		if (Effect)
		{
			AbilitySystem->ApplyGameplayEffectToSelf(Effect.GetDefaultObject(), 1.f, AbilitySystem->MakeEffectContext());
		}
	}
}
