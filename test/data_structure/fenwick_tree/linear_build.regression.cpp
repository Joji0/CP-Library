#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <vector>
#include "data_structure/fenwick_tree/fenwick_tree.hpp"
#include "data_structure/fenwick_tree/range_fenwick_tree.hpp"

int main() {
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
		int position = 1;
		long long prefix = size == 0 ? 0 : values[1];
		const long long total = std::accumulate(values.begin() + 1, values.end(), 0LL);
		for (long long target = 0; target <= total + 1; ++target) {
			while (position <= size && prefix < target) {
				++position;
				if (position <= size) prefix += values[position];
			}
			assert(point_tree.lower_bound(target) == position);
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
