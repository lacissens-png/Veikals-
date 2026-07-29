#include "SOVassalData_Aurelion.h"

#include "SOVassalActor.h"

USOVassalData_Aurelion::USOVassalData_Aurelion()
{
	VassalName  = FText::FromString(TEXT("Aurelion, Ember Sentinel"));
	Title       = FText::FromString(TEXT("Guardian of the Overlord"));
	Description = FText::FromString(TEXT(
		"A stoic guardian who shields the Overlord from harm. Summon Aurelion "
		"when the fight calls for survivability over burst damage."));

	ActorClass    = ASOVassalActor::StaticClass();
	RequiredLevel = 1;

	MaxHealth      = 450.0f;
	AttackDamage   = 25.0f;
	AttackRange    = 150.0f;
	AttackCooldown = 1.1f;

	BuffType      = ESOVassalBuffType::DamageReduction;
	BuffMagnitude = 0.20f;
}
