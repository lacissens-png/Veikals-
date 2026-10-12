// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// Random.h
// -----------------------------------------------------------------------------
// Deterministic PCG32 generator used for ALL server-authoritative rolls
// (item generation, affix rerolls, aspect values).
//
// We deliberately do not use FMath::Rand / std::rand:
//   - Global RNG state is shared by gameplay code all over the engine, so a
//     roll could never be replayed from a log.
//   - PCG32 is tiny (16 bytes), fast, and statistically far better than LCGs.
//
// Seeding policy (enforced by the UE forge layer, not here):
//   Seed = MixSeed(Item.Seed, Item.ReforgeCount) ^ ServerSecretSalt
//   The salt never leaves the server, so clients cannot pre-compute rerolls.
// -----------------------------------------------------------------------------
#pragma once

#include "EclipseCore/EclipseCoreTypes.h"

namespace Eclipse
{
	class Pcg32
	{
	public:
		explicit Pcg32(u64 Seed, u64 Stream = 0xda3e39cb94b95bdbULL)
		{
			State = 0u;
			Inc = (Stream << 1u) | 1u;
			Next();
			State += Seed;
			Next();
		}

		/** Uniform 32-bit value. */
		u32 Next()
		{
			const u64 Old = State;
			State = Old * 6364136223846793005ULL + Inc;
			const u32 XorShifted = static_cast<u32>(((Old >> 18u) ^ Old) >> 27u);
			const u32 Rot = static_cast<u32>(Old >> 59u);
			return (XorShifted >> Rot) | (XorShifted << ((32u - Rot) & 31u));
		}

		/** Uniform in [0, 1). Uses the top 24 bits so every value is exactly representable. */
		float NextFloat01()
		{
			return static_cast<float>(Next() >> 8) * (1.0f / 16777216.0f);
		}

		/** Uniform integer in [0, Bound) without modulo bias. Returns 0 when Bound == 0. */
		u32 NextBounded(u32 Bound)
		{
			if (Bound == 0u)
			{
				return 0u;
			}
			const u32 Threshold = (0u - Bound) % Bound;
			for (;;)
			{
				const u32 R = Next();
				if (R >= Threshold)
				{
					return R % Bound;
				}
			}
		}

		/** Uniform float in [Lo, Hi). */
		float Range(float Lo, float Hi)
		{
			return Lo + (Hi - Lo) * NextFloat01();
		}

		/** Bernoulli trial. */
		bool Chance(float Probability01)
		{
			return NextFloat01() < Probability01;
		}

	private:
		u64 State;
		u64 Inc;
	};

	/** SplitMix64 finaliser: combines two 64-bit values into a well-distributed seed. */
	constexpr u64 MixSeed(u64 A, u64 B)
	{
		u64 Z = A + 0x9e3779b97f4a7c15ULL + (B << 6) + (B >> 2);
		Z = (Z ^ (Z >> 30)) * 0xbf58476d1ce4e5b9ULL;
		Z = (Z ^ (Z >> 27)) * 0x94d049bb133111ebULL;
		return Z ^ (Z >> 31);
	}
}
