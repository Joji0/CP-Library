// https://atcoder.jp/contests/aalc001/tasks/aalc001_w
#include <bits/stdc++.h>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/dyadic_affine_composition.hpp"
#include "monoid/reverse_monoid.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Monoid = ReverseMonoid<DyadicAffineComposition<-1000000000, 2000000000>>;
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<Monoid::value_type> values(n + 1);
	for (int i = 1; i <= n; ++i) {
		long long value;
		std::cin >> value;
		values[i] = DyadicAffineComposition<-1000000000, 2000000000>::make_half(3 * value, true);
	}
	SegmentTree<Monoid> tree(values);
	while (query_count--) {
		int type, left;
		long long value;
		std::cin >> type >> left >> value;
		if (type == 1)
			tree.set(left, DyadicAffineComposition<-1000000000, 2000000000>::make_half(3 * value, true));
		else {
			auto aggregate = tree.query(left, static_cast<int>(value));
			std::cout << (DyadicAffineComposition<-1000000000, 2000000000>::fixed_point(aggregate) ? "Yes" : "No") << '\n';
		}
	}
}
