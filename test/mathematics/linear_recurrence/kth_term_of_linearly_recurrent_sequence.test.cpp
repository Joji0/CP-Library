#define PROBLEM "https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence"
#include <iostream>
#include <vector>
#include "mathematics/linear_recurrence/linear_recurrence.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int order;
	uint64_t index;
	std::cin >> order >> index;
	std::vector<Modular> initial(order), coefficients(order);
	for (auto& x : initial) std::cin >> x;
	for (auto& x : coefficients) std::cin >> x;
	std::cout << linear_recurrence_term(initial, coefficients, index) << '\n';
}
