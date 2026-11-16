#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>
#include "mathematics/combinatorics.hpp"

template <typename T>
class LagrangeInterpolation {
       public:
	static T evaluate(const std::vector<T>& y, long long x) {
		assert(!y.empty());
		int n = int(y.size()) - 1;
		if (n == 0) return T(0);
		if (x >= 0 && x < n) return y[x + 1];

		std::vector<T> pref(n), suff(n);
		pref[0] = T(1);
		for (int i = 0; i < n - 1; ++i) {
			pref[i + 1] = pref[i] * (T(x) - T(i));
		}
		suff[n - 1] = T(1);
		for (int i = n - 1; i >= 1; --i) {
			suff[i - 1] = suff[i] * (T(x) - T(i));
		}

		Combinatorics<T> combinations(n - 1);

		T ans = 0;
		for (int i = 0; i < n; ++i) {
			T num = pref[i] * suff[i];
			T den = combinations.inverse_factorial(i) * combinations.inverse_factorial(n - 1 - i);
			T term = y[i + 1] * num * den;
			if ((n - 1 - i) & 1) {
				ans -= term;
			} else {
				ans += term;
			}
		}
		return ans;
	}
	static T evaluate_at_arbitrary_points(const std::vector<T>& xs, const std::vector<T>& ys, T x) {
		assert(!xs.empty() && xs.size() == ys.size());
		int n = int(xs.size()) - 1;
		for (int i = 1; i <= n; ++i) {
			if (xs[i] == x) return ys[i];
		}
		T ans = 0;
		for (int i = 1; i <= n; ++i) {
			T num = ys[i];
			T den = 1;
			for (int j = 1; j <= n; ++j) {
				if (i == j) continue;
				num *= (x - xs[j]);
				den *= (xs[i] - xs[j]);
			}
			ans += num / den;
		}
		return ans;
	}
};
