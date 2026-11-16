#define PROBLEM "https://judge.yosupo.jp/problem/min_plus_convolution_convex_arbitrary"
#include <iostream>
#include <vector>
#include "mathematics/convolution/minimum_plus_convolution_with_convex_sequence.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int n, m;
	std::cin >> n >> m;
	std::vector<long long> left(n), right(m);
	for (auto& x : left) std::cin >> x;
	for (auto& x : right) std::cin >> x;
	auto result = minimum_plus_convolution_with_convex_sequence(right, left);
	for (int i = 0; i < int(result.size()); ++i) {
		if (i > 0) std::cout << ' ';
		std::cout << result[i];
	}
	std::cout << '\n';
}
