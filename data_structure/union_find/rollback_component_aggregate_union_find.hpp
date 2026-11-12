#pragma once
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include "data_structure/union_find/rollback_union_find.hpp"

// Rolls back unions and point contributions to a commutative component aggregate.
template <typename Monoid>
class RollbackComponentAggregateUnionFind {
	using Value = typename Monoid::value_type;
	struct Change {
		int union_snapshot, root;
		Value previous;
	};
	RollbackUnionFind base;
	std::vector<Value> aggregates;
	std::vector<Change> history;

	void replace(int root, Value value) {
		history.push_back({base.snapshot(), root, aggregates[root]});
		try {
			aggregates[root] = std::move(value);
		} catch (...) {
			history.pop_back();
			throw;
		}
	}

       public:
	explicit RollbackComponentAggregateUnionFind(int size = 0)
	    : base(size), aggregates(std::size_t(size) + 1, Monoid::identity()) {}
	explicit RollbackComponentAggregateUnionFind(const std::vector<Value> &values)
	    : base(int(values.size()) - 1), aggregates(values) {
		assert(!values.empty());
		aggregates[0] = Monoid::identity();
	}

	int count() const { return base.count(); }
	int find(int vertex) const { return base.find(vertex); }
	int size(int vertex) const { return base.size(vertex); }
	bool same(int first, int second) const { return base.same(first, second); }
	Value product(int vertex) const { return aggregates[find(vertex)]; }

	void apply(int vertex, const Value &contribution) {
		const int root = find(vertex);
		replace(root, Monoid::combine(aggregates[root], contribution));
	}

	bool merge(int first, int second) {
		first = find(first);
		second = find(second);
		if (first == second) return false;
		const auto combined = Monoid::combine(aggregates[first], aggregates[second]);
		const int root = base.size(first) >= base.size(second) ? first : second;
		const auto previous = snapshot();
		replace(root, combined);
		try {
			base.merge(first, second);
		} catch (...) {
			rollback(previous);
			throw;
		}
		return true;
	}

	std::size_t snapshot() const { return history.size(); }
	void rollback(std::size_t target) {
		assert(target <= history.size());
		while (history.size() > target) {
			auto &change = history.back();
			aggregates[change.root] = std::move(change.previous);
			base.rollback(change.union_snapshot);
			history.pop_back();
		}
	}
	void undo() {
		if (!history.empty()) rollback(history.size() - 1);
	}
};
