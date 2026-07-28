#include "SORealitySlashDamageType.h"

USORealitySlashDamageType::USORealitySlashDamageType()
{
	Category            = ESODamageCategory::True;
	bIgnoresResistances = true;
	HitReactStrength    = 1.0f;
}
