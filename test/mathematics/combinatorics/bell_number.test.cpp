#define PROBLEM "https://judge.yosupo.jp/problem/bell_number"
#include <iostream>
#include <vector>
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "mathematics/combinatorics/bell_number.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int n;
	std::cin >> n;
	auto result = bell_numbers<Modular>(n);
	for (int i = 0; i < int(result.size()); ++i) {
		if (i > 0) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
