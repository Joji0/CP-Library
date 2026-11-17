// https://atcoder.jp/contests/aalc001/tasks/aalc001_t
#include <bits/stdc++.h>
#include "data_structure/fenwick_tree/fenwick_tree.hpp"
#include "data_structure/segment_tree/dual_segment_tree.hpp"
#include "monoid/minimum_plus_matrix.hpp"
#include "monoid/reverse_monoid.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Matrix = MinimumPlusMatrix<long long, 2>;
	int n, query_count, distance;
	std::cin >> n >> query_count >> distance;
	std::vector<int> values(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	std::vector<std::vector<std::pair<int, int>>> queries(n + 1);
	std::vector<long long> answers(query_count);
	for (int i = 0; i < query_count; ++i) {
		int length, inserted;
		std::cin >> length >> inserted;
		queries[length].emplace_back(inserted, i);
	}
	DualSegmentTree<ReverseMonoid<Matrix>> actions(n);
	FenwickTree<long long> frequency(n);
	auto increase_best = Matrix::identity(), increase_at_end = Matrix::identity(), take_minimum = Matrix::identity();
	increase_best[2][2] = 1;
	increase_at_end[1][1] = 1;
	take_minimum[1][2] = 0;
	long long inversions = 0;
	for (int i = 1; i <= n; ++i) {
		const int value = values[i];
		inversions += frequency.sum(value + 1, std::min(n, value + distance));
		frequency.add(value, 1);
		actions.apply(value + 1, std::min(n, value + distance), increase_best);
		actions.apply(std::max(1, value - distance), value - 1, increase_at_end);
		actions.apply(1, n, take_minimum);
		for (auto [inserted, ordinal] : queries[i]) {
			auto state = actions.get(inserted);
			answers[ordinal] = inversions + *state[2][2];
			if (state[1][2]) answers[ordinal] = std::min(answers[ordinal], inversions + *state[1][2]);
		}
	}
	for (auto answer : answers) std::cout << answer << '\n';
}
