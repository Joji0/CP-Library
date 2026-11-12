#pragma once
#include <cassert>
#include <limits>
#include <numeric>
#include <vector>

// Monotone deletions. next(x) is inclusive and returns size() + 1 if exhausted.
class SuccessorUnionFind {
	int count_remaining;
	std::vector<int> parent;

       public:
	explicit SuccessorUnionFind(int size = 0) : count_remaining(size), parent(std::size_t(size) + 2) {
		assert(0 <= size && size < std::numeric_limits<int>::max());
		std::iota(parent.begin(), parent.end(), std::size_t(0));
	}
	int size() const { return int(parent.size() - 2); }
	int remaining() const { return count_remaining; }
	int next(int position) {
		assert(1 <= position && position <= size() + 1);
		int root = position;
		while (parent[root] != root) root = parent[root];
		while (parent[position] != position) {
			const int next_position = parent[position];
			parent[position] = root;
			position = next_position;
		}
		return root;
	}
	bool erase(int position) {
		assert(1 <= position && position <= size());
		if (parent[position] != position) return false;
		parent[position] = next(position + 1);
		--count_remaining;
		return true;
	}
};
