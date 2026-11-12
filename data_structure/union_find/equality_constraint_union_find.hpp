#pragma once
#include <cstddef>
#include <unordered_set>
#include <vector>
#include "data_structure/union_find/union_find.hpp"

enum class EqualityRelation { unknown,
			      equal,
			      different };

// Equality and inequality constraints on an unrestricted domain.
// Different(x, y) and different(y, z) do not imply a relation between x and z.
class EqualityConstraintUnionFind {
	UnionFind base;
	std::vector<std::unordered_set<int>> different;

	void add_inequality(int first, int second) {
		if (!different[first].insert(second).second) return;
		try {
			different[second].insert(first);
		} catch (...) {
			different[first].erase(second);
			throw;
		}
	}

       public:
	explicit EqualityConstraintUnionFind(int size = 0) : base(size), different(std::size_t(size) + 1) {}

	int count() const { return base.count(); }
	EqualityRelation relation(int first, int second) {
		first = base.find(first);
		second = base.find(second);
		if (first == second) return EqualityRelation::equal;
		return different[first].contains(second) ? EqualityRelation::different : EqualityRelation::unknown;
	}

	bool constrain_different(int first, int second) {
		first = base.find(first);
		second = base.find(second);
		if (first == second) return false;
		add_inequality(first, second);
		return true;
	}

	bool constrain_equal(int first, int second) {
		first = base.find(first);
		second = base.find(second);
		if (first == second) return true;
		if (different[first].contains(second)) return false;
		if (base.size(first) < base.size(second)) std::swap(first, second);
		std::vector<int> inserted;
		inserted.reserve(different[second].size());
		try {
			for (int neighbor : different[second]) {
				if (different[first].contains(neighbor)) continue;
				add_inequality(first, neighbor);
				inserted.push_back(neighbor);
			}
		} catch (...) {
			for (int neighbor : inserted) {
				different[first].erase(neighbor);
				different[neighbor].erase(first);
			}
			throw;
		}
		for (int neighbor : different[second]) different[neighbor].erase(second);
		different[second].clear();
		base.merge(first, second);
		return true;
	}
};
