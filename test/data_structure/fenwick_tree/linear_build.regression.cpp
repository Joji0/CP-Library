#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <random>
#include <vector>
#include "data_structure/fenwick_tree/fenwick_tree.hpp"
#include "data_structure/fenwick_tree/range_fenwick_tree.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

namespace {
void extreme_values() {
	constexpr long long low = std::numeric_limits<long long>::lowest();
	constexpr long long high = std::numeric_limits<long long>::max();
	FenwickTree<long long> point(std::vector<long long>{999, high, high, low});
	assert(point.get(3) == low && point.range_sum(2, 3) == -1);
	assert(point.prefix_sum(1) == high && point.all_sum() == high - 1);
	point.set(2, low);
	assert(point.get(2) == low && point.sum(1, 2) == -1);
	point.set(2, high);
	assert(point.get(2) == high);

	FenwickTree<long long> thresholds(std::vector<long long>{999, 0, high, 0, high});
	assert(thresholds.lower_bound(low) == 1 && thresholds.upper_bound(low) == 1);
	assert(thresholds.lower_bound(0) == 1 && thresholds.upper_bound(0) == 2);
	assert(thresholds.lower_bound(high) == 2 && thresholds.upper_bound(high) == 4);
	FenwickTree<long long> one(std::vector<long long>{999, high});
	assert(one.upper_bound(high) == one.size() + 1);

	RangeFenwickTree<long long> single(1);
	single.range_add(1, 1, low);
	assert(single.get(1) == low);
	single.set(1, high);
	assert(single.all_sum() == high);

	RangeFenwickTree<long long> range(std::vector<long long>{999, high, high, low});
	assert(range.get(3) == low && range.range_sum(2, 3) == -1);
	range.set(2, low);
	assert(range.get(2) == low && range.sum(1, 2) == -1);
	range.set(2, high);
	assert(range.get(2) == high && range.all_sum() == high - 1);
	range.initialize(4);
	range.range_add(3, 3, low);
	assert(range.get(2) == 0 && range.get(3) == low && range.get(4) == 0);
	range.add(3, high);
	assert(range.point_query(3) == -1 && range.all_sum() == -1);
}

void other_value_types() {
	using Modular = ModularInteger998244353;
	FenwickTree<Modular> point(std::vector<Modular>{0, 1, 2, 3});
	RangeFenwickTree<Modular> range(std::vector<Modular>{0, 1, 2, 3});
	point.set(2, Modular(-4));
	range.range_add(2, 3, Modular(-5));
	range.set(3, 3);
	assert(point.all_sum() == 0 && range.all_sum() == 1);
	constexpr auto highest = std::numeric_limits<unsigned long long>::max();
	FenwickTree<unsigned long long> unsigned_point(std::vector<unsigned long long>{0, highest, 2});
	RangeFenwickTree<unsigned long long> unsigned_range(std::vector<unsigned long long>{0, highest, 2});
	assert(unsigned_point.all_sum() == 1 && unsigned_range.all_sum() == 1);
	unsigned_point.set(1, 1);
	unsigned_range.set(1, 1);
	assert(unsigned_point.all_sum() == 3 && unsigned_range.all_sum() == 3);
}
}  // namespace

int main() {
	extreme_values();
	other_value_types();
	std::mt19937 random(7603);
	for (int size = 0; size <= 70; ++size) {
		std::vector<long long> values(size + 1, -999);
		for (int i = 1; i <= size; ++i) values[i] = random() % 20;
		FenwickTree<long long> point_tree(values);
		RangeFenwickTree<long long> range_tree(values);
		assert(point_tree.size() == size && range_tree.size() == size);
		assert(point_tree.empty() == (size == 0) && range_tree.empty() == (size == 0));
		assert(point_tree.sum_prefix(0) == 0 && range_tree.prefix_sum(0) == 0);
		assert(point_tree.sum(1, 0) == 0 && range_tree.range_sum(1, 0) == 0);
		std::vector<long long> prefixes(size + 1, 0);
		std::partial_sum(values.begin() + 1, values.end(), prefixes.begin() + 1);
		int position = 1;
		long long prefix = size == 0 ? 0 : values[1];
		const long long total = std::accumulate(values.begin() + 1, values.end(), 0LL);
		for (long long target = 0; target <= total + 1; ++target) {
			while (position <= size && prefix < target) {
				++position;
				if (position <= size) prefix += values[position];
			}
			assert(point_tree.lower_bound(target) == position);
			assert(point_tree.upper_bound(target) ==
			       std::upper_bound(prefixes.begin() + 1, prefixes.end(), target) - prefixes.begin());
		}
		if (size == 0) continue;

		for (int i = 1; i <= size; ++i) assert(point_tree.get(i) == values[i] && range_tree.get(i) == values[i]);
		point_tree.set(1, -8);
		range_tree.set(1, -8);
		values[1] = -8;
		range_tree.range_add(1, size, -3);
		for (int i = 1; i <= size; ++i) {
			values[i] -= 3;
			point_tree.add(i, -3);
		}
		for (int left = 1; left <= size; ++left) {
			for (int right = left; right <= size; ++right) {
				const auto expected = std::accumulate(values.begin() + left, values.begin() + right + 1, 0LL);
				assert(point_tree.sum(left, right) == expected && range_tree.range_sum(left, right) == expected);
			}
		}
		point_tree.initialize(0);
		range_tree.initialize(std::vector<long long>{123});
		assert(point_tree.empty() && range_tree.empty());
	}
}
