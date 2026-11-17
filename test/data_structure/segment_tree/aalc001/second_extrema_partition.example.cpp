// https://atcoder.jp/contests/aalc001/tasks/aalc001_u
#include <bits/stdc++.h>
#include "acted_monoid/maximum_add.hpp"
#include "data_structure/fast_set.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int test_count;
	std::cin >> test_count;
	while (test_count--) {
		int n;
		std::cin >> n;
		std::vector<long long> values(n + 1);
		std::vector<int> order(n);
		for (int i = 1; i <= n; ++i) {
			std::cin >> values[i];
			order[i - 1] = i;
		}
		std::sort(order.begin(), order.end(), [&](int a, int b) { return std::pair(values[a], a) < std::pair(values[b], b); });
		struct Event {
			int left, right;
			long long delta;
		};
		std::vector<std::vector<Event>> events(n + 2);
		auto rectangle = [&](int left, int right, int first_end, int last_end, long long delta) {
			if (left > right || first_end > last_end) return;
			events[first_end].push_back({left, right, delta});
			events[last_end + 1].push_back({left, right, -delta});
		};
		auto add_contributions = [&](int sign) {
			FastSet seen(n);
			for (int i : order) {
				int left = seen.prev(i - 1), right = seen.next(i + 1);
				int leftmost = left == -1 ? -1 : seen.prev(left - 1);
				int rightmost = right == -1 ? -1 : seen.next(right + 1);
				if (leftmost == -1) leftmost = 0;
				if (rightmost == -1) rightmost = n + 1;
				if (left != -1) rectangle(leftmost + 1, left, i, (right == -1 ? n + 1 : right) - 1, sign * values[i]);
				if (right != -1) rectangle((left == -1 ? 0 : left) + 1, i, right, rightmost - 1, sign * values[i]);
				seen.insert(i);
			}
		};
		add_contributions(-1);
		std::reverse(order.begin(), order.end());
		add_contributions(1);

		using Acted = MaximumAdd<>;
		LazySegmentTree<Acted> tree(n, Acted::identity());
		tree.set_value(1, 0LL);
		Acted::Value answer;
		for (int right = 1; right <= n; ++right) {
			for (auto [left, end, delta] : events[right]) tree.update(left, end, delta);
			answer = tree.query(1, right - 3);
			if (right < n) tree.set(right + 1, answer);
		}
		std::cout << *answer << '\n';
	}
}
