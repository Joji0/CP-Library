#include <cassert>
#include <random>
#include <utility>
#include <vector>
#include "data_structure/union_find/bipartite_union_find.hpp"
#include "data_structure/union_find/rollback_bipartite_union_find.hpp"
#include "data_structure/union_find/rollback_union_find.hpp"
#include "graph/connectivity/offline_dynamic_connectivity.hpp"

namespace {
struct Edge {
	int first, second, start, finish;
};
struct Coloring {
	std::vector<int> component, color;
	std::vector<char> valid{true};
};

Coloring brute(int size, const std::vector<Edge> &edges, int time) {
	std::vector<std::vector<int>> graph(size + 1);
	for (const auto &edge : edges) {
		if (edge.start <= time && time < edge.finish) {
			graph[edge.first].push_back(edge.second);
			graph[edge.second].push_back(edge.first);
		}
	}
	Coloring result;
	result.component.resize(size + 1);
	result.color.resize(size + 1);
	for (int start = 1; start <= size; ++start) {
		if (result.component[start] != 0) continue;
		const int id = int(result.valid.size());
		result.valid.push_back(true);
		result.component[start] = id;
		std::vector<int> queue{start};
		for (std::size_t i = 0; i < queue.size(); ++i) {
			const int current = queue[i];
			for (int to : graph[current]) {
				if (result.component[to] == 0) {
					result.component[to] = id;
					result.color[to] = 1 - result.color[current];
					queue.push_back(to);
				} else if (result.color[to] == result.color[current]) {
					result.valid[id] = false;
				}
			}
		}
	}
	return result;
}
}  // namespace

int main() {
	BipartiteUnionFind online(6);
	assert(online.add_edge(1, 2));
	assert(online.add_edge(2, 3));
	assert(online.try_same_color(1, 3) == true);
	assert(!online.add_edge(1, 3));
	assert(!online.add_edge(4, 4));
	assert(online.count_non_bipartite() == 2);
	assert(!online.add_edge(3, 4));
	assert(online.count_non_bipartite() == 1);
	assert(!online.try_same_color(2, 4) && online.is_bipartite(5));

	std::mt19937 random(45718);
	constexpr int times = 21, size = 8;
	for (int trial = 0; trial < 220; ++trial) {
		OfflineDynamicConnectivity schedule(times);
		std::vector<Edge> edges;
		for (int i = 0; i < 30; ++i) {
			const int first = 1 + random() % size, second = 1 + random() % size;
			const int start = 1 + random() % times;
			const int finish = start + random() % (times - start + 2);
			const int id = schedule.add_edge(start, first, second);
			if (finish <= times) schedule.remove_edge(finish, id);
			edges.push_back({first, second, start, finish});
		}

		RollbackBipartiteUnionFind parity(size);
		schedule.execute(parity, [&](int time, const auto &state) {
					 const auto expected = brute(size, edges, time);
					 int bad = 0;
					 for (bool valid : expected.valid) bad += !valid;
					 assert(state.count_non_bipartite() == bad && state.is_bipartite() == (bad == 0));
					 assert(state.count() == int(expected.valid.size()) - 1);
					 for (int u = 1; u <= size; ++u) {
						 assert(state.is_bipartite(u) == bool(expected.valid[expected.component[u]]));
						 for (int v = 1; v <= size; ++v) {
							 const bool connected = expected.component[u] == expected.component[v];
							 assert(state.same(u, v) == connected);
							 const auto actual = state.try_same_color(u, v);
							 assert(actual.has_value() ==
								(connected && expected.valid[expected.component[u]]));
							 if (actual) assert(*actual == (expected.color[u] == expected.color[v]));
						 }
					 } });
		assert(parity.snapshot() == 0 && parity.count() == size && parity.is_bipartite());

		RollbackUnionFind connectivity(size);
		schedule.execute(connectivity, [&](int time, const auto &state) {
			const auto expected = brute(size, edges, time);
			assert(state.count() == int(expected.valid.size()) - 1);
		});
		assert(connectivity.snapshot() == 0);
	}
}
