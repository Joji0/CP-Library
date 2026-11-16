#define PROBLEM "https://judge.yosupo.jp/problem/factorize"
#include <iostream>
#include <vector>
#include "mathematics/number_theory/number_theory.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int count;
	std::cin >> count;
	while (count--) {
		uint64_t n;
		std::cin >> n;
		auto factors = NumberTheory::factorize(n);
		std::cout << factors.size();
		for (auto factor : factors) std::cout << ' ' << factor;
		std::cout << '\n';
	}
}
