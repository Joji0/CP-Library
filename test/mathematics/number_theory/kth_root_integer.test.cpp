#define PROBLEM "https://judge.yosupo.jp/problem/kth_root_integer"
#include <iostream>
#include <vector>
#include "mathematics/number_theory/integer_kth_root.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int count;
	std::cin >> count;
	while (count--) {
		uint64_t value;
		unsigned degree;
		std::cin >> value >> degree;
		std::cout << integer_kth_root(value, degree) << '\n';
	}
}
