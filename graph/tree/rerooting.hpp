#pragma once
#include <cassert>
#include <utility>
#include <vector>
#include "graph/graph.hpp"

// Two iterative passes over a connected undirected tree.
// Combines contributions in adjacency order. The transform receives an edge
// oriented from the contributing neighbor toward the destination vertex.
template <class Monoid, class T, class Transform>
class Rerooting {
	using Value = typename Monoid::value_type;
	std::vector<std::vector<Value>> without_neighbor;
	std::vector<Value> values;

	void initialize(const Graph<T> &graph, const Transform &transform,
			const std::vector<Value> *initial, bool at_every_vertex) {
		const int size = graph.size();
		assert(!initial || initial->size() == std::size_t(size) + 1);
		assert(graph.edge_count() == (size == 0 ? 0 : size - 1));
		const Value identity = Monoid::identity();
		auto initial_value = [&](int vertex) -> Value { return initial ? (*initial)[vertex] : identity; };
		values.assign(size + 1, identity);
		without_neighbor.resize(size + 1);
		if (size <= 1) {
			if (initial) values = *initial;
			return;
		}

		std::vector<int> parent(size + 1, 0), toward_parent(size + 1, 0), toward_child(size + 1, 0);
		std::vector<int> order{1};
		order.reserve(size);
		parent[1] = 1;
		for (std::size_t index = 0; index < order.size(); ++index) {
			const int vertex = order[index];
			const auto &neighbors = graph[vertex];
			without_neighbor[vertex].assign(neighbors.size() + 1, identity);
			for (int position = 1; position <= int(neighbors.size()); ++position) {
				const int neighbor = neighbors[position - 1].to;
				if (neighbor == parent[vertex]) {
					toward_parent[vertex] = position;
					continue;
				}
				assert(parent[neighbor] == 0);
				parent[neighbor] = vertex;
				toward_child[neighbor] = position;
				order.push_back(neighbor);
			}
		}
		assert(int(order.size()) == size);

		for (int index = int(order.size()) - 1; index > 0; --index) {
			const int vertex = order[index];
			const auto &neighbors = graph[vertex];
			assert(toward_parent[vertex] != 0);
			Value aggregate = at_every_vertex || neighbors.size() == 1 ? initial_value(vertex) : identity;
			for (const auto &edge : neighbors) {
				const int child = edge.to;
				if (child == parent[vertex]) continue;
				aggregate = Monoid::combine(aggregate, transform(without_neighbor[child][toward_parent[child]],
										 Edge<T>(child, vertex, edge.cost, edge.id)));
			}
			without_neighbor[vertex][toward_parent[vertex]] = std::move(aggregate);
		}

		std::vector<Value> contributions;
		for (int vertex : order) {
			const auto &neighbors = graph[vertex];
			contributions.clear();
			contributions.reserve(neighbors.size());
			for (const auto &edge : neighbors) {
				const int neighbor = edge.to;
				const int position = neighbor == parent[vertex] ? toward_child[vertex] : toward_parent[neighbor];
				contributions.push_back(transform(without_neighbor[neighbor][position],
								  Edge<T>(neighbor, vertex, edge.cost, edge.id)));
			}

			auto &excluding = without_neighbor[vertex];
			Value suffix = identity;
			for (int position = int(neighbors.size()); position >= 1; --position) {
				excluding[position] = suffix;
				suffix = Monoid::combine(contributions[position - 1], suffix);
			}

			Value prefix = at_every_vertex ? initial_value(vertex) : identity;
			for (int position = 1; position <= int(neighbors.size()); ++position) {
				excluding[position] = Monoid::combine(prefix, excluding[position]);
				prefix = Monoid::combine(prefix, contributions[position - 1]);
			}
			values[vertex] = std::move(prefix);
			if (!at_every_vertex && neighbors.size() == 1) excluding[1] = initial_value(vertex);
		}
	}

	Rerooting(const Graph<T> &graph, const Transform &transform, const std::vector<Value> &initial, bool at_every_vertex) {
		initialize(graph, transform, &initial, at_every_vertex);
	}

       public:
	Rerooting(const Graph<T> &graph, const Transform &transform) {
		initialize(graph, transform, nullptr, false);
	}

	// The three-argument constructor supplies base values only for leaf messages.
	Rerooting(const Graph<T> &graph, const Transform &transform, const std::vector<Value> &initial)
	    : Rerooting(graph, transform, initial, false) {}

	Rerooting(const Graph<T> &graph, const Transform &transform, std::vector<Value> &&initial) {
		const auto owned_initial = std::move(initial);
		initialize(graph, transform, &owned_initial, false);
	}

	// Uses initial[v] before the contributions from neighbors at every vertex.
	static Rerooting with_vertex_values(const Graph<T> &graph, const Transform &transform,
					    const std::vector<Value> &initial) {
		return Rerooting(graph, transform, initial, true);
	}

	const std::vector<Value> &results() const & { return values; }
	std::vector<Value> results() && { return std::move(values); }
	const std::vector<Value> &get_res() const & { return results(); }
	std::vector<Value> get_res() && { return std::move(*this).results(); }
	typename std::vector<Value>::const_reference operator[](int vertex) const & { return values[vertex]; }
	Value operator[](int vertex) && { return std::move(values[vertex]); }
	const std::vector<std::vector<Value>> &get_dp() const & { return without_neighbor; }
	std::vector<std::vector<Value>> get_dp() && { return std::move(without_neighbor); }
	typename std::vector<Value>::const_reference get_dp(int vertex, int position) const & { return without_neighbor[vertex][position]; }
	Value get_dp(int vertex, int position) && { return std::move(without_neighbor[vertex][position]); }
};

template <class Monoid, class T, class Transform>
Rerooting<Monoid, T, Transform> make_rerooting(const Graph<T> &graph, const Transform &transform) {
	return Rerooting<Monoid, T, Transform>(graph, transform);
}

template <class Monoid, class T, class Transform>
Rerooting<Monoid, T, Transform> make_rerooting_with_vertex_values(const Graph<T> &graph, const Transform &transform,
								  const std::vector<typename Monoid::value_type> &initial) {
	return Rerooting<Monoid, T, Transform>::with_vertex_values(graph, transform, initial);
}
