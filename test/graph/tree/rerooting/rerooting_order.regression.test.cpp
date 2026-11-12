#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <random>
#include <string>
#include <vector>
#include "graph/tree/rerooting.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
struct Concatenate {
	using value_type = std::string;
	static value_type identity() { return {}; }
	static value_type combine(const value_type &left, const value_type &right) { return left + right; }
};

auto transform = [](const std::string &value, const Edge<int> &edge) {
	return std::to_string(edge.from) + ">" + std::to_string(edge.to) + "(" + value + ")";
};

std::string naive(const Graph<int> &graph, int vertex, int excluded, const std::vector<std::string> &initial,
		  bool every_vertex) {
	if (!every_vertex && (graph[vertex].empty() || (graph[vertex].size() == 1 && excluded != 0))) return initial[vertex];
	std::string result = every_vertex ? initial[vertex] : "";
	for (const auto &edge : graph[vertex]) {
		if (edge.to == excluded) continue;
		result += transform(naive(graph, edge.to, vertex, initial, every_vertex),
				    Edge<int>(edge.to, vertex, edge.cost, edge.id));
	}
	return result;
}
}  // namespace

int main() {
	assert(make_rerooting<Concatenate>(Graph<int>(), transform).results().size() == 1);
	std::mt19937 random(15791);
	for (int trial = 0; trial < 90; ++trial) {
		const int size = 1 + random() % 10;
		Graph<int> graph(size);
		std::vector<std::string> initial(size + 1, "dummy");
		for (int vertex = 1; vertex <= size; ++vertex) {
			initial[vertex] = std::string(1, char('a' + vertex - 1));
			if (vertex > 1) graph.add_undirected_edge(vertex, 1 + random() % (vertex - 1));
		}
		const Rerooting<Concatenate, int, decltype(transform)> leaves_only(graph, transform, initial);
		const auto every_vertex = make_rerooting_with_vertex_values<Concatenate>(graph, transform, initial);
		for (int vertex = 1; vertex <= size; ++vertex) {
			assert(leaves_only[vertex] == naive(graph, vertex, 0, initial, false));
			assert(every_vertex.results()[vertex] == naive(graph, vertex, 0, initial, true));
			for (int position = 1; position <= int(graph[vertex].size()); ++position) {
				const int excluded = graph[vertex][position - 1].to;
				assert(leaves_only.get_dp(vertex, position) == naive(graph, vertex, excluded, initial, false));
				assert(every_vertex.get_dp(vertex, position) == naive(graph, vertex, excluded, initial, true));
			}
		}
	}
	answer_library_checker_aplusb();
}
