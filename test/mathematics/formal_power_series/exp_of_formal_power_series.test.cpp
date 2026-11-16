#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_formal_power_series"
#include <iostream>
#include <vector>
#include "mathematics/formal_power_series/formal_power_series.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	using Series = FormalPowerSeries<Modular>;
	int n;
	std::cin >> n;
	Series coefficients(n);
	for (auto& coefficient : coefficients) std::cin >> coefficient;
	auto result = coefficients.exponential(n);
	for (int i = 0; i < int(result.size()); ++i) {
		if (i > 0) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
