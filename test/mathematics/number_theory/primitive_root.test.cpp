#define PROBLEM "https://judge.yosupo.jp/problem/primitive_root"
#include <iostream>
#include <vector>
#include "mathematics/number_theory/primitive_root.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int count;
	std::cin >> count;
	while (count--) {
		uint64_t n;
		std::cin >> n;
		std::cout << PrimitiveRoot::find(n) << '\n';
	}
}
