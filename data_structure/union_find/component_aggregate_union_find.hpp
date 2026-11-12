#pragma once
#include <cassert>
#include <vector>
#include "data_structure/union_find/union_find.hpp"

// Component aggregation requires a commutative monoid.
template <typename Monoid>
class ComponentAggregateUnionFind {
	using Value = typename Monoid::value_type;
	UnionFind base;
	std::vector<Value> aggregates;

       public:
	explicit ComponentAggregateUnionFind(int size = 0)
	    : base(size), aggregates(std::size_t(size) + 1, Monoid::identity()) {}
	explicit ComponentAggregateUnionFind(const std::vector<Value> &values)
	    : base(int(values.size()) - 1), aggregates(values) {
		assert(!values.empty());
		aggregates[0] = Monoid::identity();
	}

	int count() const { return base.count(); }
	int find(int vertex) { return base.find(vertex); }
	int size(int vertex) { return base.size(vertex); }
	bool same(int first, int second) { return base.same(first, second); }
	Value product(int vertex) { return aggregates[find(vertex)]; }

	void apply(int vertex, const Value &contribution) {
		const int root = find(vertex);
		aggregates[root] = Monoid::combine(aggregates[root], contribution);
	}

	bool merge(int first, int second) {
		first = find(first);
		second = find(second);
		if (first == second) return false;
		auto combined = Monoid::combine(aggregates[first], aggregates[second]);
		const int root = base.size(first) >= base.size(second) ? first : second;
		aggregates[root] = std::move(combined);
		base.merge(first, second);
		return true;
	}
};
