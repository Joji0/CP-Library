#define PROBLEM "https://judge.yosupo.jp/problem/tree_path_composite_sum"
#include <iostream>
#include <vector>
#include "graph/tree/rerooting.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

using Modular = ModularInteger998244353;

struct SumAndCount {
	struct value_type {
		Modular sum;
		int count;
	};
	static value_type identity() { return {0, 0}; }
	static value_type combine(const value_type &left, const value_type &right) {
		return {left.sum + right.sum, left.count + right.count};
	}
};

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int size;
	std::cin >> size;
	std::vector<SumAndCount::value_type> values(size + 1);
	for (int vertex = 1; vertex <= size; ++vertex) {
		std::cin >> values[vertex].sum;
		values[vertex].count = 1;
	}

	struct AffineFunction {
		Modular scale, offset;
	};
	Graph<int> graph(size);
	std::vector<AffineFunction> functions(size);
	for (int i = 1; i < size; ++i) {
		int first, second;
		AffineFunction function;
		std::cin >> first >> second >> function.scale >> function.offset;
		const int id = graph.add_undirected_edge(first + 1, second + 1);
		functions[id] = function;
	}
	const auto transform = [&](const SumAndCount::value_type &aggregate, const Edge<int> &edge) {
		const auto [scale, offset] = functions[edge.id];
		return SumAndCount::value_type{scale * aggregate.sum + offset * aggregate.count, aggregate.count};
	};
	const auto result = make_rerooting_with_vertex_values<SumAndCount>(graph, transform, values);
	for (int vertex = 1; vertex <= size; ++vertex) {
		if (vertex > 1) std::cout << ' ';
		std::cout << result[vertex].sum;
	}
	std::cout << '\n';
}
