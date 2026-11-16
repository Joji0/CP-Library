#pragma once
#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>
#include "mathematics/formal_power_series/formal_power_series.hpp"

// Bostan-Mori algorithm for computing the N-th term of a linear recurrence in O(K log K log N).
template <typename Mint = StaticModularInteger<998244353>>
class BostanMori {
       public:
	static Mint coefficient_of_rational(FormalPowerSeries<Mint> P, FormalPowerSeries<Mint> Q, uint64_t N) {
		assert(!Q.empty() && Q[0] != Mint(0));

		while (N > 0) {
			FormalPowerSeries<Mint> Q_neg = Q;
			for (int i = 1; i < (int)Q_neg.size(); i += 2) {
				Q_neg[i] = -Q_neg[i];
			}

			FormalPowerSeries<Mint> P_prod = P * Q_neg;
			FormalPowerSeries<Mint> Q_prod = Q * Q_neg;

			FormalPowerSeries<Mint> next_P;
			FormalPowerSeries<Mint> next_Q;
			next_P.reserve((P_prod.size() + 1) / 2);
			next_Q.reserve((Q_prod.size() + 1) / 2);

			for (int i = int(N & 1); i < (int)P_prod.size(); i += 2) {
				next_P.push_back(P_prod[i]);
			}
			for (int i = 0; i < (int)Q_prod.size(); i += 2) {
				next_Q.push_back(Q_prod[i]);
			}
			P = std::move(next_P);
			Q = std::move(next_Q);
			N >>= 1;
		}
		return P.empty() ? Mint(0) : P[0] / Q[0];
	}

	// Computes a_N given initial terms a[0..d-1] and recurrence relation:
	// a[n] = c[0]*a[n-1] + c[1]*a[n-2] + ... + c[d-1]*a[n-d]
	static Mint solve(const std::vector<Mint> &initial, const std::vector<Mint> &c, uint64_t N) {
		int d = (int)c.size();
		assert(int(initial.size()) == d);
		if (N < (uint64_t)d) return initial[N];

		FormalPowerSeries<Mint> Q(d + 1, Mint(0));
		Q[0] = Mint(1);
		for (int i = 0; i < d; ++i) Q[i + 1] = -c[i];

		FormalPowerSeries<Mint> P = (FormalPowerSeries<Mint>(initial) * Q).prefix(d);
		return coefficient_of_rational(std::move(P), std::move(Q), N);
	}
};
