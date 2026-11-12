#pragma once
#include <bit>
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>

// Static O(1) range queries for an idempotent monoid. The two query blocks can overlap.
template <typename Monoid>
class SparseTable {
	using T = typename Monoid::value_type;
	int n = 0;
	std::vector<std::vector<T>> levels;

       public:
	SparseTable() = default;
	SparseTable(const std::vector<T> &values) { initialize(values); }

	// Builds in O(n log n) from values[1..n].
	void initialize(const std::vector<T> &values) {
		assert(!values.empty());
		const std::size_t size = values.size() - 1;
		std::vector<std::vector<T>> next;
		if (size != 0) {
			next.emplace_back(values.begin() + 1, values.end());
			for (std::size_t width = 2; width <= size; width *= 2) {
				const auto &previous = next.back();
				std::vector<T> row;
				row.reserve(size - width + 1);
				for (std::size_t left = 0; left + width <= size; ++left) {
					row.push_back(Monoid::combine(previous[left], previous[left + width / 2]));
				}
				next.push_back(std::move(row));
			}
		}
		levels = std::move(next);
		n = int(size);
	}

	int size() const { return n; }
	bool empty() const { return n == 0; }

	// The position interval [left, right] is inclusive.
	T query(int left, int right) const {
		if (left > right) return Monoid::identity();
		assert(1 <= left && right <= n);
		const int level = std::bit_width(static_cast<unsigned>(right - left + 1)) - 1;
		const int width = int(1U << level);
		return Monoid::combine(levels[level][left - 1], levels[level][right - width]);
	}

	T product(int left, int right) const { return query(left, right); }
	T get(int position) const { return query(position, position); }
	T all_product() const { return query(1, n); }
};
