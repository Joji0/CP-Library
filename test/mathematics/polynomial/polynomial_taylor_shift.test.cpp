#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_taylor_shift"
#include <iostream>
#include <vector>
#include "mathematics/polynomial/polynomial_taylor_shift.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	using Series = FormalPowerSeries<Modular>;
	int n;
	Modular offset;
	std::cin >> n >> offset;
	Series coefficients(n);
	for (auto& x : coefficients) std::cin >> x;
	auto result = polynomial_taylor_shift(coefficients, offset);
	for (int i = 0; i < int(result.size()); ++i) {
		if (i > 0) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
