#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_interpolation"
#include <iostream>
#include <vector>
#include "mathematics/polynomial/polynomial_interpolation.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int n;
	std::cin >> n;
	std::vector<Modular> points(n + 1), values(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> points[i];
	for (int i = 1; i <= n; ++i) std::cin >> values[i];
	auto result = polynomial_interpolation(points, values);
	for (int i = 0; i < int(result.size()); ++i) {
		if (i > 0) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
