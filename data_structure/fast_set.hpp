#pragma once
#include <algorithm>
#include <vector>
#include <cstdint>
#include <string>
#include <cassert>
#include <bit>
#include <cstddef>

class FastSet {
	int n;
	std::vector<std::vector<uint64_t>> seg;

       public:
	explicit FastSet(int size) : n(size) {
		assert(size >= 0);
		std::size_t blocks = (std::size_t(size) + 63) / 64;
		while (true) {
			seg.emplace_back(blocks, 0);
			if (blocks <= 1) break;
			blocks = (blocks + 63) / 64;
		}
	}

	void build(const std::string& s) {
		assert(s.size() == std::size_t(n) + 1);
		clear();
		for (int i = 0; i < n; ++i) {
			if (s[i + 1] == '1') {
				seg[0][i >> 6] |= (1ULL << (i & 63));
			}
		}
		for (int i = 0; i < (int)seg.size() - 1; ++i) {
			for (int j = 0; j < (int)seg[i].size(); ++j) {
				if (seg[i][j]) {
					seg[i + 1][j >> 6] |= (1ULL << (j & 63));
				}
			}
		}
	}
	bool contains(int i) const {
		if (i < 1 || i > n) return false;
		--i;
		return (seg[0][i >> 6] >> (i & 63)) & 1;
	}
	void insert(int i) {
		assert(1 <= i && i <= n);
		--i;
		for (int h = 0; h < (int)seg.size(); ++h) {
			seg[h][i >> 6] |= (1ULL << (i & 63));
			i >>= 6;
		}
	}
	void erase(int i) {
		assert(1 <= i && i <= n);
		--i;
		for (int h = 0; h < (int)seg.size(); ++h) {
			seg[h][i >> 6] &= ~(1ULL << (i & 63));
			if (seg[h][i >> 6]) break;
			i >>= 6;
		}
	}
	// Inclusive successor and predecessor return -1 when no element exists.
	int next(int i) const {
		if (i > n) return -1;
		i = std::max(i, 1) - 1;
		for (int h = 0; h < (int)seg.size(); ++h) {
			if (i >> 6 >= (int)seg[h].size()) break;
			uint64_t d = seg[h][i >> 6];
			d &= (~0ULL) << (i & 63);
			if (!d) {
				i = (i >> 6) + 1;
				continue;
			}
			i = (i >> 6 << 6) + std::countr_zero(d);
			for (int g = h - 1; g >= 0; --g) {
				i = (i << 6) + std::countr_zero(seg[g][i]);
			}
			return i < n ? i + 1 : -1;
		}
		return -1;
	}
	int prev(int i) const {
		if (i < 1 || n == 0) return -1;
		i = std::min(i, n) - 1;
		for (int h = 0; h < (int)seg.size(); ++h) {
			if (i >> 6 >= (int)seg[h].size()) break;
			uint64_t d = seg[h][i >> 6];
			d &= (~0ULL) >> (63 - (i & 63));
			if (!d) {
				i = (i >> 6) - 1;
				if (i < 0) return -1;
				continue;
			}
			i = (i >> 6 << 6) + (63 - std::countl_zero(d));
			for (int g = h - 1; g >= 0; --g) {
				i = (i << 6) + (63 - std::countl_zero(seg[g][i]));
			}
			return i + 1;
		}
		return -1;
	}
	bool empty() const {
		return seg.back().empty() || seg.back()[0] == 0;
	}
	int min() const {
		return next(1);
	}
	int max() const {
		return prev(n);
	}
	bool any(int l, int r) const {
		int x = next(l);
		return x != -1 && x <= r;
	}
	template <typename F>
	void enumerate(int l, int r, F f) const {
		for (int x = next(l); x != -1 && x <= r; x = next(x + 1)) {
			f(x);
			if (x == n || x == r) break;
		}
	}
	void clear() {
		for (auto& v : seg) {
			std::fill(v.begin(), v.end(), 0ULL);
		}
	}
};
