#define PROBLEM "https://judge.yosupo.jp/problem/prefix_sum_of_binomial_coefficient"

#include <iostream>
#include <utility>
#include <vector>
#include "mathematics/combinatorics/binomial_coefficient_prefix_sum.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int count;
	std::cin >> count;
	std::vector<std::pair<int, int>> queries(count + 1);
	for (int i = 1; i <= count; ++i) std::cin >> queries[i].first >> queries[i].second;
	auto result = binomial_coefficient_prefix_sums<ModularInteger998244353>(queries);
	for (int i = 1; i <= count; ++i) std::cout << result[i] << '\n';
}
