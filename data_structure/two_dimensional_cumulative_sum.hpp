#pragma once
#include <cassert>
#include <cstdint>
#include <vector>

template <typename T = long long>
class TwoDimensionalCumulativeSum {
	int H, W;
	std::vector<std::vector<T>> pref;

       public:
	TwoDimensionalCumulativeSum() : TwoDimensionalCumulativeSum(0, 0) {}
	TwoDimensionalCumulativeSum(int h, int w) : H(h), W(w), pref(h + 1, std::vector<T>(w + 1, T(0))) {}
	TwoDimensionalCumulativeSum(const std::vector<std::vector<T>> &a) { build(a); }

	void add(int r, int c, T val) {
		assert(1 <= r && r <= H && 1 <= c && c <= W);
		pref[r][c] += val;
	}

	void build() {
		for (int i = 1; i <= H; ++i) {
			for (int j = 1; j <= W; ++j) {
				pref[i][j] += pref[i - 1][j] + pref[i][j - 1] - pref[i - 1][j - 1];
			}
		}
	}

	void build(const std::vector<std::vector<T>> &a) {
		assert(!a.empty());
		H = int(a.size()) - 1;
		W = H == 0 ? 0 : int(a[1].size()) - 1;
		assert(W >= 0);
		pref.assign(H + 1, std::vector<T>(W + 1, T(0)));
		for (int i = 1; i <= H; ++i) {
			assert(int(a[i].size()) == W + 1);
			for (int j = 1; j <= W; ++j) {
				pref[i][j] = a[i][j] + pref[i - 1][j] + pref[i][j - 1] - pref[i - 1][j - 1];
			}
		}
	}

	// Query sum on subgrid [r1, r2] x [c1, c2] (1-based, inclusive)
	T query(int r1, int c1, int r2, int c2) const {
		if (r1 > r2 || c1 > c2) return T(0);
		assert(1 <= r1 && r2 <= H && 1 <= c1 && c2 <= W);
		return pref[r2][c2] - pref[r1 - 1][c2] - pref[r2][c1 - 1] + pref[r1 - 1][c1 - 1];
	}
};
