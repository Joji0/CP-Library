#pragma once
#include <vector>
#include <cassert>
#include <utility>

template <typename T = int>
struct Edge {
	int from, to;
	T cost;
	int id;
	Edge() : from(-1), to(-1), cost(0), id(-1) {}
	Edge(int _from, int _to, const T& _cost = 1, int _id = -1)
	    : from(_from), to(_to), cost(_cost), id(_id) {}
	Edge(int _from, int _to, T&& _cost, int _id = -1)
	    : from(_from), to(_to), cost(std::move(_cost)), id(_id) {}
	operator int() const { return to; }
	friend bool operator<(const Edge<T>& lhs, const Edge<T>& rhs) {
		return lhs.cost < rhs.cost;
	}
	friend bool operator>(const Edge<T>& lhs, const Edge<T>& rhs) {
		return lhs.cost > rhs.cost;
	}
};

template <typename T = int>
using Edges = std::vector<Edge<T>>;

template <typename T = int>
using GraphMatrix = std::vector<std::vector<T>>;

template <typename T = int>
class Graph {
	std::vector<Edges<T>> adjacency;
	int number_of_edges = 0;

       public:
	explicit Graph(int n = 0) : adjacency(std::size_t(n) + 1) { assert(n >= 0); }

	int size() const { return adjacency.empty() ? 0 : int(adjacency.size()) - 1; }
	bool empty() const { return size() == 0; }
	int edge_count() const { return adjacency.empty() ? 0 : number_of_edges; }
	int edge_size() const { return edge_count(); }

	// Returns a 1-based snapshot indexed by edge ID in O(vertices + edges).
	// An undirected edge is listed once, with the smaller vertex as its source.
	// Structural changes should go through add_edge to preserve edge IDs.
	Edges<T> edge_list() const {
		Edges<T> result(edge_count() + 1);
		int found = 0;
		for (const auto& neighbors : *this) {
			for (const auto& edge : neighbors) {
				assert(1 <= edge.id && edge.id <= edge_count());
				if (result[edge.id].id != -1) continue;
				result[edge.id] = edge;
				++found;
			}
		}
		assert(found == edge_count());
		return result;
	}

	Edges<T>& operator[](int vertex) {
		assert(1 <= vertex && vertex <= size());
		return adjacency[vertex];
	}
	const Edges<T>& operator[](int vertex) const {
		assert(1 <= vertex && vertex <= size());
		return adjacency[vertex];
	}
	auto begin() { return adjacency.empty() ? adjacency.begin() : adjacency.begin() + 1; }
	auto end() { return adjacency.end(); }
	auto begin() const { return adjacency.empty() ? adjacency.begin() : adjacency.begin() + 1; }
	auto end() const { return adjacency.end(); }

	int add_edge(int a, int b, const T& c = 1, bool is_directed = false) {
		assert(1 <= a && a <= size());
		assert(1 <= b && b <= size());
		const int id = number_of_edges + 1;
		adjacency[a].emplace_back(a, b, c, id);
		if (!is_directed) {
			try {
				adjacency[b].emplace_back(b, a, adjacency[a].back().cost, id);
			} catch (...) {
				adjacency[a].pop_back();
				throw;
			}
		}
		return ++number_of_edges;
	}
	int add_directed_edge(int a, int b, const T& c = 1) { return add_edge(a, b, c, true); }
	int add_undirected_edge(int a, int b, const T& c = 1) { return add_edge(a, b, c, false); }
};
