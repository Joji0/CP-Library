#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_quotients"
#include <iostream>
#include <vector>
#include "mathematics/number_theory/quotient_blocks.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	uint64_t n;
	std::cin >> n;
	auto blocks = quotient_blocks(n);
	std::cout << blocks.size() << '\n';
	for (auto it = blocks.rbegin(); it != blocks.rend(); ++it) std::cout << it->quotient << ' ';
	std::cout << '\n';
}
