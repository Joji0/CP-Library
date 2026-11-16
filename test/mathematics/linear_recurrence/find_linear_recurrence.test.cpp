#define PROBLEM "https://judge.yosupo.jp/problem/find_linear_recurrence"
#include <iostream>
#include <vector>
#include "mathematics/linear_recurrence/berlekamp_massey.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int n;
	std::cin >> n;
	std::vector<Modular> sequence(n);
	for (auto& x : sequence) std::cin >> x;
	auto coefficients = berlekamp_massey(sequence);
	std::cout << coefficients.size() << '\n';
	for (auto x : coefficients) std::cout << x << ' ';
	std::cout << '\n';
}
