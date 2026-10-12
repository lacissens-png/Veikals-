// Copyright Eclipse of the Primordials. All Rights Reserved.
//
// SpatialHash.h
// -----------------------------------------------------------------------------
// Rebuild-every-frame 2D spatial hash for the swarm (top-down game => XY only).
//
// DESIGN
//   * Counting-sort build into ONE flat array (Entries) + bucket offsets.
//     No per-cell containers, no allocations after warm-up: O(N) build,
//     ~2 passes over memory. Rebuilding 500 mobs costs a few microseconds.
//   * Infinite world: cell coords are hashed into a power-of-two bucket table.
//     Collisions are harmless (exact distance test filters them) and the query
//     dedupes buckets so a bucket hit by two cells is not visited twice.
//   * Positions are copied into Entries, so the query loop touches only the
//     grid's own contiguous memory (cache friendly; no gather into SoA arrays).
//
// THREADING
//   Build() on one thread, then any number of concurrent const queries
//   (the swarm's ParallelFor steering pass relies on this).
// -----------------------------------------------------------------------------
#pragma once

#include "EclipseCore/EclipseCoreTypes.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace Eclipse
{
	class SpatialHashGrid2D
	{
	public:
		struct Entry
		{
			float X;
			float Y;
			u32 Index; // Caller's index (swarm slot).
		};

		explicit SpatialHashGrid2D(float InCellSize = 400.f, u32 BucketCountPow2 = 4096)
			: CellSize(InCellSize)
			, InvCellSize(1.f / InCellSize)
			, BucketMask(BucketCountPow2 - 1u)
		{
			BucketStart.assign(BucketCountPow2 + 1u, 0u);
		}

		float GetCellSize() const { return CellSize; }
		std::size_t Num() const { return Entries.size(); }

		/**
		 * @param Count   number of candidate items
		 * @param GetPos  callable(u32 i, float& X, float& Y) -> bool; return false to skip (dead slot)
		 */
		template <typename PosFn>
		void Build(u32 Count, PosFn&& GetPos)
		{
			const u32 BucketCount = BucketMask + 1u;
			std::fill(BucketStart.begin(), BucketStart.end(), 0u);
			Scratch.clear();
			Scratch.reserve(Count);

			// Pass 1: collect live items + count per bucket.
			for (u32 I = 0; I < Count; ++I)
			{
				float X, Y;
				if (!GetPos(I, X, Y))
				{
					continue;
				}
				const u32 B = BucketOf(CellCoord(X), CellCoord(Y));
				Scratch.push_back({ X, Y, I, B });
				++BucketStart[B + 1u];
			}

			// Prefix sum -> bucket start offsets.
			for (u32 B = 0; B < BucketCount; ++B)
			{
				BucketStart[B + 1u] += BucketStart[B];
			}

			// Pass 2: scatter.
			Entries.resize(Scratch.size());
			Cursor.assign(BucketStart.begin(), BucketStart.end() - 1);
			for (const Pending& P : Scratch)
			{
				Entries[Cursor[P.Bucket]++] = { P.X, P.Y, P.Index };
			}
		}

		/** Visits every item within Radius of (X, Y). Visitor(const Entry&, float DistSq). */
		template <typename Fn>
		void QueryRadius(float X, float Y, float Radius, Fn&& Visitor) const
		{
			if (Entries.empty() || Radius < 0.f)
			{
				return;
			}
			const float RadiusSq = Radius * Radius;
			const i32 MinCX = CellCoord(X - Radius), MaxCX = CellCoord(X + Radius);
			const i32 MinCY = CellCoord(Y - Radius), MaxCY = CellCoord(Y + Radius);

			// Small fixed buffer for bucket de-duplication; queries larger than
			// 16x16 cells fall back to a heap buffer (rare: huge ultimates only).
			constexpr int InlineCap = 256;
			u32 Inline[InlineCap];
			std::vector<u32> Heap;
			const i64 CellCount = static_cast<i64>(MaxCX - MinCX + 1) * static_cast<i64>(MaxCY - MinCY + 1);
			u32* Visited = Inline;
			if (CellCount > InlineCap)
			{
				Heap.resize(static_cast<std::size_t>(Min<i64>(CellCount, static_cast<i64>(BucketMask) + 1)));
				Visited = Heap.data();
			}
			std::size_t NumVisited = 0;
			const std::size_t VisitedCap = (CellCount > InlineCap) ? Heap.size() : static_cast<std::size_t>(InlineCap);

			for (i32 CY = MinCY; CY <= MaxCY; ++CY)
			{
				for (i32 CX = MinCX; CX <= MaxCX; ++CX)
				{
					const u32 B = BucketOf(CX, CY);
					bool bSeen = false;
					for (std::size_t K = 0; K < NumVisited; ++K)
					{
						if (Visited[K] == B) { bSeen = true; break; }
					}
					if (bSeen)
					{
						continue;
					}
					if (NumVisited < VisitedCap)
					{
						Visited[NumVisited++] = B;
					}

					for (u32 E = BucketStart[B]; E < BucketStart[B + 1u]; ++E)
					{
						const Entry& En = Entries[E];
						const float DX = En.X - X, DY = En.Y - Y;
						const float D2 = DX * DX + DY * DY;
						if (D2 <= RadiusSq)
						{
							Visitor(En, D2);
						}
					}
				}
			}
		}

	private:
		struct Pending
		{
			float X, Y;
			u32 Index;
			u32 Bucket;
		};

		i32 CellCoord(float V) const { return static_cast<i32>(std::floor(V * InvCellSize)); }

		u32 BucketOf(i32 CX, i32 CY) const
		{
			// Large-prime hash (Teschner et al.).
			const u32 H = (static_cast<u32>(CX) * 73856093u) ^ (static_cast<u32>(CY) * 19349663u);
			return H & BucketMask;
		}

		float CellSize;
		float InvCellSize;
		u32 BucketMask;
		std::vector<u32> BucketStart;
		std::vector<u32> Cursor;
		std::vector<Entry> Entries;
		std::vector<Pending> Scratch;
	};
}
