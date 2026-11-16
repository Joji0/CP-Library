#define PROBLEM "https://judge.yosupo.jp/problem/sharp_p_subset_sum"
#include <iostream>
#include <vector>
#include "mathematics/combinatorics/subset_sum_count.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int n, maximum;
	std::cin >> n >> maximum;
	std::vector<int> values(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	auto result = subset_sum_counts<Modular>(values, maximum);
	for (int i = 1; i < int(result.size()); ++i) {
		if (i > 1) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
