#define PROBLEM "https://judge.yosupo.jp/problem/subset_convolution"
#include <iostream>
#include <vector>
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "mathematics/set_function/subset_convolution.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int n;
	std::cin >> n;
	std::vector<Modular> left(1 << n);
	for (auto& x : left) std::cin >> x;
	std::vector<Modular> right(1 << n);
	for (auto& x : right) std::cin >> x;
	auto result = subset_convolution(left, right);
	for (int i = 0; i < int(result.size()); ++i) {
		if (i > 0) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
