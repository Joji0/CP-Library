#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Integer Partition Function p(0), p(1), ..., p(N) via Euler's Pentagonal Number Theorem in O(N log N).
template <typename Mint = ModularInteger998244353>
class PartitionNumber {
       public:
	static std::vector<Mint> table(int n) {
		assert(n >= 0);
		FormalPowerSeries<Mint> den(n + 1, Mint(0));
		den[0] = Mint(1);
		for (long long k = 1;; ++k) {
			long long p1 = k * (3 * k - 1) / 2;
			long long p2 = k * (3 * k + 1) / 2;
			if (p1 > n) break;
			Mint s = (k & 1) ? Mint(-1) : Mint(1);
			den[int(p1)] += s;
			if (p2 <= n) den[int(p2)] += s;
		}
		return den.inverse(n + 1);
	}
};
