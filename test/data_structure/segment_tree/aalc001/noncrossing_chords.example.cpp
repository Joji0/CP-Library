// https://atcoder.jp/contests/aalc001/tasks/aalc001_p
#include <bits/stdc++.h>
#include "data_structure/segment_tree/dual_segment_tree.hpp"
#include "monoid/exclusive_or.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<std::pair<int, int>> chords(n + 1);
	std::vector<unsigned long long> hash(n + 1);
	std::mt19937_64 random(std::chrono::steady_clock::now().time_since_epoch().count());
	for (int i = 1; i <= n; ++i) {
		std::cin >> chords[i].first >> chords[i].second;
		hash[i] = random();
	}
	DualSegmentTree<ExclusiveOr<unsigned long long>> cover(2 * n);
	std::vector<int> last(n + 1);
	int right = 0;
	for (int left = 1; left <= n; ++left) {
		while (right < n) {
			auto [first, second] = chords[right + 1];
			if (cover.get(first) != cover.get(second - 1)) break;
			++right;
			cover.apply(first, second - 1, hash[right]);
		}
		last[left] = right;
		auto [first, second] = chords[left];
		cover.apply(first, second - 1, hash[left]);
	}
	while (query_count--) {
		int left, right;
		std::cin >> left >> right;
		std::cout << (right <= last[left] ? "Yes" : "No") << '\n';
	}
}
