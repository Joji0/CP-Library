#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"
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
		std::cout << (NumberTheory::is_prime(n) ? "Yes" : "No") << '\n';
	}
}
