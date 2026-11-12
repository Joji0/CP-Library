#pragma once
#include <cassert>
#include <bit>
#include <utility>
#include <vector>

template <typename T = long long>
class FenwickTree {
	int n;
	std::vector<T> bit;

       public:
	FenwickTree(int n = 0) { initialize(n); }
	explicit FenwickTree(std::vector<T> values) { initialize(std::move(values)); }

	void initialize(int n) {
		assert(n >= 0);
		this->n = n;
		bit.assign(n + 1, T(0));
	}
	// Builds in O(n) from values[1..n].
	void initialize(std::vector<T> values) {
		assert(!values.empty());
		n = int(values.size()) - 1;
		bit = std::move(values);
		bit[0] = T(0);
		for (int i = 1; i <= n; ++i) {
			const int step = i & -i;
			if (step <= n - i) bit[i + step] += bit[i];
		}
	}

	int size() const { return n; }
	bool empty() const { return n == 0; }

	void add(int pos, T delta) {
		assert(1 <= pos && pos <= n);
		for (int i = pos; i <= n; i += i & -i) bit[i] += delta;
	}
	T sum_prefix(int r) const {
		assert(0 <= r && r <= n);
		T res = 0;
		for (int i = r; i > 0; i -= i & -i) res += bit[i];
		return res;
	}
	T sum(int l, int r) const {
		if (l > r) return T(0);
		assert(1 <= l && r <= n);
		return sum_prefix(r) - sum_prefix(l - 1);
	}
	T get(int pos) const { return sum(pos, pos); }
	void set(int pos, T value) { add(pos, value - get(pos)); }

	// Requires nonnegative point values. Returns size() + 1 if the target is not reached.
	int lower_bound(T target) const {
		if (target <= 0) return 1;
		int pos = 0;
		const int pw = int(std::bit_floor(static_cast<unsigned>(n)));
		for (int k = pw; k > 0; k >>= 1) {
			if (k <= n - pos && bit[pos + k] < target) {
				pos += k;
				target -= bit[pos];
			}
		}
		return pos + 1;
	}
};
