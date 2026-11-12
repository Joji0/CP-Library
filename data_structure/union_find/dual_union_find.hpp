#pragma once
#include <cassert>
#include <vector>
#include "data_structure/union_find/potential_union_find.hpp"

// Applies a left group action to every current member of a component.
// Merging preserves each vertex's value. Group follows PotentialUnionFind's contract.
template <typename Group>
class DualUnionFind {
	using Value = typename Group::value_type;
	using Operations = union_find_detail::PotentialOperations<Group>;
	PotentialUnionFind<Group> base;
	std::vector<Value> root_values;

       public:
	explicit DualUnionFind(int size = 0) : base(size), root_values(std::size_t(size) + 1, Group::identity()) {}
	explicit DualUnionFind(const std::vector<Value> &values)
	    : base(int(values.size()) - 1), root_values(values) {
		assert(!values.empty());
		root_values[0] = Group::identity();
	}

	int count() const { return base.count(); }
	int find(int vertex) { return base.find(vertex); }
	int size(int vertex) { return base.size(vertex); }
	bool same(int first, int second) { return base.same(first, second); }

	Value get(int vertex) {
		const int root = find(vertex);
		return Group::combine(root_values[root], base.weight(vertex));
	}

	void apply_component(int vertex, const Value &action) {
		const int root = find(vertex);
		root_values[root] = Group::combine(action, root_values[root]);
	}

	bool merge(int first, int second) {
		first = find(first);
		second = find(second);
		if (first == second) return false;
		return base.constrain(first, second, Operations::difference(root_values[first], root_values[second])) ==
		       PotentialMergeResult::merged;
	}
};
