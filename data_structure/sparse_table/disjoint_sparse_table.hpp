#pragma once
#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>

// Static O(1) inclusive range queries for any monoid, preserving left-to-right order.
template <typename Monoid>
class DisjointSparseTable {
	using T = typename Monoid::value_type;
	int n = 0;
	std::vector<std::vector<T>> levels;

       public:
	DisjointSparseTable() = default;
	explicit DisjointSparseTable(const std::vector<T> &values) { initialize(values); }

	// Builds in O(n log n) from values[1..n].
	void initialize(const std::vector<T> &values) {
		assert(!values.empty());
		const std::size_t size = values.size() - 1;
		std::vector<std::vector<T>> next;
		if (size != 0) {
			next.emplace_back(values.begin() + 1, values.end());
			for (std::size_t half = 2; half < size; half *= 2) {
				auto row = next.front();
				for (std::size_t start = 0; start < size; start += 2 * half) {
					const auto middle = std::min(start + half, size);
					const auto finish = std::min(start + 2 * half, size);
					for (std::size_t i = middle - 1; i > start; --i) {
						row[i - 1] = Monoid::combine(row[i - 1], row[i]);
					}
					for (std::size_t i = middle + 1; i < finish; ++i) {
						row[i] = Monoid::combine(row[i - 1], row[i]);
					}
				}
				next.push_back(std::move(row));
			}
		}
		levels = std::move(next);
		n = int(size);
	}

	int size() const { return n; }
	bool empty() const { return n == 0; }

	T product(int left, int right) const {
		if (left > right) return Monoid::identity();
		assert(1 <= left && right <= n);
		--left;
		--right;
		if (left == right) return levels[0][left];
		const int level = std::bit_width(static_cast<unsigned>(left ^ right)) - 1;
		return Monoid::combine(levels[level][left], levels[level][right]);
	}

	T query(int left, int right) const { return product(left, right); }
	T get(int position) const { return product(position, position); }
	T all_product() const { return product(1, n); }
};
