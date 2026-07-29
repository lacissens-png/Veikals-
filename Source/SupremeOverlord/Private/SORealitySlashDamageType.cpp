#include "SORealitySlashDamageType.h"

USORealitySlashDamageType::USORealitySlashDamageType()
{
	Category            = ESODamageCategory::TrueDamage;
	bIgnoresResistances = true;
	HitReactStrength    = 1.0f;
}
