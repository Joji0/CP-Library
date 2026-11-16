#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_floor_of_linear"
#include <iostream>
#include <vector>
#include "mathematics/number_theory/number_theory.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int count;
	std::cin >> count;
	while (count--) {
		long long n, modulus, slope, offset;
		std::cin >> n >> modulus >> slope >> offset;
		std::cout << NumberTheory::floor_sum(n, modulus, slope, offset) << '\n';
	}
}
