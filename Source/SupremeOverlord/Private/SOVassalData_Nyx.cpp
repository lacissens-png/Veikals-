#include "SOVassalData_Nyx.h"

#include "SOVassalActor.h"

USOVassalData_Nyx::USOVassalData_Nyx()
{
	VassalName  = FText::FromString(TEXT("Nyx, the Silent Fang"));
	Title       = FText::FromString(TEXT("Blade in the Dark"));
	Description = FText::FromString(TEXT(
		"A swift assassin whose presence quickens every strike. Summon Nyx to "
		"burst down a priority target quickly rather than to hold a line."));

	ActorClass    = ASOVassalActor::StaticClass();
	RequiredLevel = 1;

	MaxHealth      = 220.0f;
	AttackDamage   = 35.0f;
	AttackRange    = 130.0f;
	AttackCooldown = 0.7f;

	BuffType      = ESOVassalBuffType::AttackSpeed;
	BuffMagnitude = 0.20f;
}
