#pragma once
#include <cassert>
#include "data_structure/union_find/successor_union_find.hpp"
#include "data_structure/union_find/union_find.hpp"

// Unites all vertices in an inclusive interval, skipping processed adjacent pairs.
class IntervalUnionFind {
	UnionFind base;
	SuccessorUnionFind remaining_pairs;

       public:
	explicit IntervalUnionFind(int size = 0) : base(size), remaining_pairs(size > 0 ? size - 1 : 0) {}

	int count() const { return base.count(); }
	int size(int vertex) { return base.size(vertex); }
	bool same(int first, int second) { return base.same(first, second); }
	bool merge(int first, int second) { return base.merge(first, second); }
	int merge_interval(int left, int right) {
		if (left > right) return 0;
		assert(1 <= left && right <= base.vertex_count());
		int merged = 0;
		for (int vertex = remaining_pairs.next(left); vertex < right; vertex = remaining_pairs.next(vertex)) {
			merged += base.merge(vertex, vertex + 1);
			remaining_pairs.erase(vertex);
		}
		return merged;
	}
};
