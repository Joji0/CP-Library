#define PROBLEM "https://judge.yosupo.jp/problem/multipoint_evaluation"
#include <iostream>
#include <vector>
#include "mathematics/polynomial/polynomial_multipoint_evaluation.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	using Series = FormalPowerSeries<Modular>;
	int n, m;
	std::cin >> n >> m;
	Series coefficients(n);
	std::vector<Modular> points(m + 1);
	for (auto& x : coefficients) std::cin >> x;
	for (int i = 1; i <= m; ++i) std::cin >> points[i];
	auto result = polynomial_multipoint_evaluation(coefficients, points);
	for (int i = 1; i < int(result.size()); ++i) {
		if (i > 1) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
