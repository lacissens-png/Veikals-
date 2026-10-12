// Copyright Eclipse of the Primordials. All Rights Reserved.

#include "World/EclipseFieldAffinitySubsystem.h"

#include "World/EclipseCreationDomain.h"

namespace
{
	constexpr float ResonanceScalar = 1.25f;
	constexpr float OppositionScalar = 0.8f;
}

void UEclipseFieldAffinitySubsystem::RegisterDomain(AEclipseCreationDomain* Domain)
{
	Domains.AddUnique(Domain);
}

void UEclipseFieldAffinitySubsystem::UnregisterDomain(AEclipseCreationDomain* Domain)
{
	Domains.RemoveAll([Domain](const TWeakObjectPtr<AEclipseCreationDomain>& D) { return !D.IsValid() || D.Get() == Domain; });
}

bool UEclipseFieldAffinitySubsystem::AreOpposed(EEclipseAffinity A, EEclipseAffinity B)
{
	auto Pair = [A, B](EEclipseAffinity X, EEclipseAffinity Y) { return (A == X && B == Y) || (A == Y && B == X); };
	return Pair(EEclipseAffinity::Blood, EEclipseAffinity::Lunar)
		|| Pair(EEclipseAffinity::Temporal, EEclipseAffinity::Seismic)
		|| Pair(EEclipseAffinity::Void, EEclipseAffinity::Blood);
}

const AEclipseCreationDomain* UEclipseFieldAffinitySubsystem::FindDomainAt(const FVector& Location) const
{
	// Iterate newest-first so overlapping domains resolve to the latest cast.
	for (int32 K = Domains.Num() - 1; K >= 0; --K)
	{
		const AEclipseCreationDomain* D = Domains[K].Get();
		if (D && D->Contains(Location))
		{
			return D;
		}
	}
	return nullptr;
}

EEclipseAffinity UEclipseFieldAffinitySubsystem::ResolveAffinityAt(const FVector& Location) const
{
	const AEclipseCreationDomain* D = FindDomainAt(Location);
	return D ? D->GetOverrideAffinity() : BaseAffinity;
}

float UEclipseFieldAffinitySubsystem::GetDamageScalar(const FVector& Location, EEclipseAffinity DamageAffinity) const
{
	if (DamageAffinity == EEclipseAffinity::Neutral)
	{
		return 1.f;
	}

	if (const AEclipseCreationDomain* D = FindDomainAt(Location))
	{
		// Inside Creation the caster rewrites the rules: matching damage gets the domain's
		// (larger) resonance, and nothing is ever opposed.
		return D->GetOverrideAffinity() == DamageAffinity ? D->GetResonanceScalar() : 1.f;
	}

	if (BaseAffinity == DamageAffinity)
	{
		return ResonanceScalar;
	}
	return AreOpposed(BaseAffinity, DamageAffinity) ? OppositionScalar : 1.f;
}
