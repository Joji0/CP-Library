#define PROBLEM "https://judge.yosupo.jp/problem/division_of_polynomials"
#include <iostream>
#include <vector>
#include "mathematics/polynomial/polynomial_division.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	using Series = FormalPowerSeries<Modular>;
	int n, m;
	std::cin >> n >> m;
	Series dividend(n), divisor(m);
	for (auto& x : dividend) std::cin >> x;
	for (auto& x : divisor) std::cin >> x;
	auto [quotient, remainder] = polynomial_division(dividend, divisor);
	std::cout << quotient.size() << ' ' << remainder.size() << '\n';
	for (const auto& coefficients : {quotient, remainder}) {
		for (const auto& coefficient : coefficients) std::cout << coefficient << ' ';
		std::cout << '\n';
	}
}
