#pragma once
#include <bit>
#include <cassert>
#include <vector>
#include "data_structure/union_find/union_find.hpp"

// Unites corresponding positions in two equally sized intervals.
// Each successful block merge is expanded at most once over all calls.
class RangeParallelUnionFind {
	int length;
	std::vector<UnionFind> levels;

	template <typename OnMerge>
	void merge_block(int level, int first, int second, const OnMerge &on_merge) {
		auto &sets = levels[level];
		if (level == 0) {
			const int left = sets.find(first), right = sets.find(second);
			if (sets.merge(left, right)) {
				const int root = sets.find(left);
				on_merge(root, root == left ? right : left);
			}
			return;
		}
		if (!sets.merge(first, second)) return;
		const int half = 1 << (level - 1);
		merge_block(level - 1, first, second, on_merge);
		merge_block(level - 1, first + half, second + half, on_merge);
	}

       public:
	explicit RangeParallelUnionFind(int size = 0) : length(size) {
		assert(size >= 0);
		levels.emplace_back(size);
		for (unsigned span = 2; span <= static_cast<unsigned>(size); span <<= 1) {
			levels.emplace_back(size - int(span) + 1);
		}
	}
	int find(int vertex) { return levels.front().find(vertex); }
	int count() const { return levels.front().count(); }
	int size(int vertex) { return levels.front().size(vertex); }
	bool same(int first, int second) { return levels.front().same(first, second); }

	// The callback receives (surviving root, absorbed root) for every new vertex merge.
	template <typename OnMerge>
	void merge_pairs(int first, int second, int count, const OnMerge &on_merge) {
		assert(0 <= count && count <= length);
		if (count == 0) return;
		assert(1 <= first && first <= length - count + 1);
		assert(1 <= second && second <= length - count + 1);
		const int level = int(std::bit_width(static_cast<unsigned>(count))) - 1;
		const int shift = count - (1 << level);
		merge_block(level, first, second, on_merge);
		if (shift != 0) merge_block(level, first + shift, second + shift, on_merge);
	}

	void merge_pairs(int first, int second, int count) {
		merge_pairs(first, second, count, [](int, int) {});
	}

	template <typename OnMerge>
	void merge_ranges(int first_left, int first_right, int second_left, int second_right, const OnMerge &on_merge) {
		if (first_left > first_right && second_left > second_right) return;
		assert(1 <= first_left && first_left <= first_right && first_right <= length);
		assert(1 <= second_left && second_left <= second_right && second_right <= length);
		assert(first_right - first_left == second_right - second_left);
		merge_pairs(first_left, second_left, first_right - first_left + 1, on_merge);
	}

	void merge_ranges(int first_left, int first_right, int second_left, int second_right) {
		merge_ranges(first_left, first_right, second_left, second_right, [](int, int) {});
	}
};
