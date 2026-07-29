#include "SOVassalData_Grimjaw.h"

#include "SOVassalActor.h"

USOVassalData_Grimjaw::USOVassalData_Grimjaw()
{
	VassalName  = FText::FromString(TEXT("Grimjaw, Iron Warden"));
	Title       = FText::FromString(TEXT("The Unbreakable"));
	Description = FText::FromString(TEXT(
		"Raw muscle with no aura to speak of. Grimjaw is recruited for the "
		"fight itself, not for what he grants while standing nearby."));

	ActorClass    = ASOVassalActor::StaticClass();
	RequiredLevel = 1;

	MaxHealth      = 600.0f;
	AttackDamage   = 45.0f;
	AttackRange    = 160.0f;
	AttackCooldown = 1.3f;

	BuffType      = ESOVassalBuffType::None;
	BuffMagnitude = 0.0f;
}
