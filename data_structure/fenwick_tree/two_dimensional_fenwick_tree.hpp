#pragma once
#include <cstdint>
#include <algorithm>
#include <cassert>
#include <vector>

// Point add and rectangle sum in O(log H * log W).
template <typename T = long long>
class TwoDimensionalFenwickTree {
	int _h, _w;
	std::vector<std::vector<T>> _data;

       public:
	TwoDimensionalFenwickTree() : _h(0), _w(0) {}
	TwoDimensionalFenwickTree(int h, int w) : _h(h), _w(w), _data(h + 1, std::vector<T>(w + 1, T(0))) {}

	// Builds in O(H * W) from a grid with a dummy row and column.
	explicit TwoDimensionalFenwickTree(const std::vector<std::vector<T>> &values)
	    : TwoDimensionalFenwickTree(int(values.size()) - 1, values.size() <= 1 ? 0 : int(values[1].size()) - 1) {
		assert(!values.empty());
		for (int row = 1; row <= _h; ++row) {
			assert(int(values[row].size()) == _w + 1);
			for (int column = 1; column <= _w; ++column) {
				_data[row][column] += values[row][column];
				const int parent = column + (column & -column);
				if (parent <= _w) _data[row][parent] += _data[row][column];
			}
		}
		for (int row = 1; row <= _h; ++row) {
			const int parent = row + (row & -row);
			if (parent > _h) continue;
			for (int column = 1; column <= _w; ++column) {
				_data[parent][column] += _data[row][column];
			}
		}
	}

	bool empty() const { return _h == 0 || _w == 0; }
	int height() const { return _h; }
	int width() const { return _w; }

	// Adds val to (r, c) (1-based)
	void add(int r, int c, T val) {
		assert(1 <= r && r <= _h && 1 <= c && c <= _w);
		for (int i = r; i <= _h; i += (i & -i)) {
			for (int j = c; j <= _w; j += (j & -j)) {
				_data[i][j] += val;
			}
		}
	}

	// Prefix sum on [1, r] x [1, c]
	T sum(int r, int c) const {
		assert(0 <= r && r <= _h && 0 <= c && c <= _w);
		T res = T(0);
		for (int i = r; i > 0; i -= (i & -i)) {
			for (int j = c; j > 0; j -= (j & -j)) {
				res += _data[i][j];
			}
		}
		return res;
	}

	// 2D Range sum on [r1, r2] x [c1, c2]
	T sum(int r1, int c1, int r2, int c2) const {
		if (r1 > r2 || c1 > c2) return T(0);
		assert(1 <= r1 && r2 <= _h);
		assert(1 <= c1 && c2 <= _w);
		return sum(r2, c2) - sum(r1 - 1, c2) - sum(r2, c1 - 1) + sum(r1 - 1, c1 - 1);
	}

	T get(int row, int column) const { return sum(row, column, row, column); }
	void set(int row, int column, T value) { add(row, column, value - get(row, column)); }

	// 2D Range sum on [r1, r2] x [c1, c2] (inclusive)
	T query(int r1, int c1, int r2, int c2) const {
		return sum(r1, c1, r2, c2);
	}
};
