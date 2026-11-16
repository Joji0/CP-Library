#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_set_power_series"
#include <iostream>
#include <vector>
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "mathematics/set_function/set_power_series_exponential.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int n;
	std::cin >> n;
	std::vector<Modular> left(1 << n);
	for (auto& x : left) std::cin >> x;
	auto result = set_power_series_exponential(left);
	for (int i = 0; i < int(result.size()); ++i) {
		if (i > 0) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
