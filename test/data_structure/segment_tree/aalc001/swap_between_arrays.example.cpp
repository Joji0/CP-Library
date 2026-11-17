// https://atcoder.jp/contests/aalc001/tasks/aalc001_l
#include <bits/stdc++.h>
#include "acted_monoid/array_permutation.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "monoid/maximum_subarray_sum.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Aggregate = MaximumSubarraySum<long long>;
	using Acted = ArrayPermutation<Aggregate, 3>;
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<Acted::Value> initial(n + 1, Acted::identity());
	for (int array = 1; array <= 3; ++array) {
		for (int i = 1; i <= n; ++i) {
			long long value;
			std::cin >> value;
			initial[i][array] = Aggregate::from_val(value);
		}
	}
	LazySegmentTree<Acted> tree(initial);
	while (query_count--) {
		int type, left;
		std::cin >> type >> left;
		if (type == 4) {
			std::cout << tree.max_right(left, [](const Acted::Value &value) {
				return value[1].empty || value[1].pref <= 0;
			}) + 1 << '\n';
		} else {
			int right;
			std::cin >> right;
			tree.update(left, right, Acted::make_swap(type, type % 3 + 1));
		}
	}
}
