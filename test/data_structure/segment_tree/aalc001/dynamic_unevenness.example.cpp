// https://atcoder.jp/contests/aalc001/tasks/aalc001_x
#include <bits/stdc++.h>
#include "graph/flow/dynamic_minimum_cost_path_circulation.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int n, query_count;
	std::cin >> n >> query_count;
	std::vector<long long> prices(n + 1), path_capacity(n);
	std::vector<int> vertex_capacity(n + 1);
	for (int i = 1; i <= n; ++i) std::cin >> prices[i];
	for (int i = 1; i <= n; ++i) std::cin >> vertex_capacity[i];
	for (int i = 1; i < n; ++i) std::cin >> path_capacity[i];
	DynamicMinimumCostPathCirculation solver(prices, vertex_capacity, path_capacity);
	while (query_count--) {
		int position;
		long long price;
		std::cin >> position >> price;
		solver.set_price(position, price);
		std::cout << -solver.minimum_cost() << '\n';
	}
}
