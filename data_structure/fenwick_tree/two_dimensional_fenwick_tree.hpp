#pragma once
#include <cassert>
#include <cstddef>
#include <vector>
#include "data_structure/fenwick_tree/detail/arithmetic.hpp"

// Point add and rectangle sum in O(log H * log W).
template <typename T = long long>
class TwoDimensionalFenwickTree {
	using Storage = fenwick_tree_detail::Storage<T>;
	int h = 0, w = 0;
	std::vector<std::vector<Storage>> data;

	void add_value(int row, int column, const Storage &delta) {
		assert(1 <= row && row <= h && 1 <= column && column <= w);
		for (std::size_t i = row; i < data.size(); i += i & -i) {
			for (std::size_t j = column; j < data[i].size(); j += j & -j) data[i][j] += delta;
		}
	}

	Storage prefix_value(int row, int column) const {
		assert(0 <= row && row <= h && 0 <= column && column <= w);
		Storage result(0);
		for (int i = row; i > 0; i -= i & -i) {
			for (int j = column; j > 0; j -= j & -j) result += data[i][j];
		}
		return result;
	}

	Storage rectangle_value(int top, int left, int bottom, int right) const {
		if (top > bottom || left > right) return Storage(0);
		assert(1 <= top && bottom <= h && 1 <= left && right <= w);
		return prefix_value(bottom, right) - prefix_value(top - 1, right) -
		       prefix_value(bottom, left - 1) + prefix_value(top - 1, left - 1);
	}

       public:
	TwoDimensionalFenwickTree() = default;
	TwoDimensionalFenwickTree(int height, int width) : h(height), w(width) {
		assert(0 <= h && 0 <= w);
		data.assign(std::size_t(h) + 1, std::vector<Storage>(std::size_t(w) + 1, Storage(0)));
	}

	// Builds in O(H * W) from a grid with a dummy row and column.
	explicit TwoDimensionalFenwickTree(const std::vector<std::vector<T>> &values)
	    : TwoDimensionalFenwickTree(values.empty() ? 0 : int(values.size() - 1),
					values.size() <= 1 || values[1].empty() ? 0 : int(values[1].size() - 1)) {
		assert(!values.empty());
		const std::size_t columns = std::size_t(w) + 1;
		for (std::size_t row = 1; row < data.size(); ++row) {
			assert(values[row].size() == columns);
			for (std::size_t column = 1; column < columns; ++column) {
				data[row][column] += Storage(values[row][column]);
				const auto parent = column + (column & -column);
				if (parent < columns) data[row][parent] += data[row][column];
			}
		}
		for (std::size_t row = 1; row < data.size(); ++row) {
			const auto parent = row + (row & -row);
			if (parent >= data.size()) continue;
			for (std::size_t column = 1; column < columns; ++column) data[parent][column] += data[row][column];
		}
	}

	bool empty() const { return h == 0 || w == 0; }
	int height() const { return h; }
	int width() const { return w; }

	void add(int row, int column, T value) { add_value(row, column, Storage(value)); }
	void set(int row, int column, T value) {
		add_value(row, column, Storage(value) - rectangle_value(row, column, row, column));
	}
	T get(int row, int column) const { return sum(row, column, row, column); }

	T prefix_sum(int row, int column) const { return fenwick_tree_detail::to_value<T>(prefix_value(row, column)); }
	T rectangle_sum(int top, int left, int bottom, int right) const {
		return fenwick_tree_detail::to_value<T>(rectangle_value(top, left, bottom, right));
	}
	T sum(int row, int column) const { return prefix_sum(row, column); }
	T sum(int top, int left, int bottom, int right) const { return rectangle_sum(top, left, bottom, right); }
	T query(int top, int left, int bottom, int right) const { return rectangle_sum(top, left, bottom, right); }
	T all_sum() const { return prefix_sum(h, w); }
};
