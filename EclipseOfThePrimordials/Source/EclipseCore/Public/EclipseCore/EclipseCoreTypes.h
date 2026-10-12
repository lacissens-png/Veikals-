// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// EclipseCoreTypes.h
// -----------------------------------------------------------------------------
// Foundation types for the engine-agnostic "rules core".
//
// WHY A SEPARATE CORE?
//   Everything in EclipseCore is plain C++17 with no UObject / UHT dependency.
//   That buys us three things:
//     1. Unit testability: the authoritative rules (reforging, runewords,
//        stagger, status stacking) compile and run in a standalone test binary
//        (see Tests/) in milliseconds, without booting the editor.
//     2. Server authority by construction: every function is a pure
//        transformation of explicit state + an explicit RNG. The UE layer only
//        ever calls these on the server; clients receive replicated results.
//     3. Determinism: given the same seed and inputs, results are bit-for-bit
//        reproducible, which makes loot/forge exploits auditable from logs.
//
// When compiled inside Unreal, ECLIPSECORE_API is defined by UBT as the
// module's DLL export macro. In the standalone test build it expands to nothing.
// -----------------------------------------------------------------------------
#pragma once

#include <cstddef>
#include <cstdint>

#ifndef ECLIPSECORE_API
#define ECLIPSECORE_API
#endif

namespace Eclipse
{
	using u8  = std::uint8_t;
	using u16 = std::uint16_t;
	using u32 = std::uint32_t;
	using u64 = std::uint64_t;
	using i32 = std::int32_t;
	using i64 = std::int64_t;

	/** Reserved id meaning "no definition / empty socket". All data ids start at 1. */
	constexpr u32 InvalidId = 0;

	template <typename T>
	constexpr T Clamp(T Value, T Lo, T Hi)
	{
		return Value < Lo ? Lo : (Value > Hi ? Hi : Value);
	}

	template <typename T>
	constexpr T Max(T A, T B) { return A > B ? A : B; }

	template <typename T>
	constexpr T Min(T A, T B) { return A < B ? A : B; }

	/** Enum -> underlying index helper used for fixed-size lookup tables. */
	template <typename E>
	constexpr std::size_t Idx(E Value) { return static_cast<std::size_t>(Value); }
}
