#include <cassert>
#include <limits>
#include <random>
#include <set>
#include <vector>
#include "data_structure/union_find/dynamic_union_find.hpp"
#include "data_structure/union_find/interval_union_find.hpp"
#include "data_structure/union_find/kruskal_reconstruction_tree.hpp"
#include "data_structure/union_find/range_parallel_union_find.hpp"
#include "data_structure/union_find/successor_union_find.hpp"
#include "data_structure/union_find/union_find.hpp"

int main() {
	constexpr auto low = std::numeric_limits<long long>::lowest();
	constexpr auto high = std::numeric_limits<long long>::max();
	DynamicUnionFind<> sparse;
	assert(!sparse.same(low, low) && sparse.count() == 0);
	assert(sparse.merge(low, high) && sparse.count() == 1 && sparse.size(high) == 2);
	assert(!sparse.add(high) && !sparse.leader(0));
	assert(sparse.leader(low) == sparse.leader(high));
	assert(sparse.add(0) && sparse.vertex_count() == 3);

	std::mt19937 random(15647);
	constexpr int size = 30;
	for (int trial = 0; trial < 50; ++trial) {
		RangeParallelUnionFind parallel(size);
		IntervalUnionFind interval(size);
		SuccessorUnionFind successor(size);
		UnionFind parallel_expected(size), interval_expected(size);
		std::set<int> remaining;
		for (int vertex = 1; vertex <= size; ++vertex) remaining.insert(vertex);
		std::vector<int> aggregate(size + 1, 1);
		for (int step = 0; step < 70; ++step) {
			const int count = random() % (size + 1);
			const int first = 1 + random() % (size - count + 1), second = 1 + random() % (size - count + 1);
			parallel.merge_ranges(first, first + count - 1, second, second + count - 1, [&](int root, int absorbed) {
				aggregate[root] += aggregate[absorbed];
			});
			for (int offset = 0; offset < count; ++offset) parallel_expected.merge(first + offset, second + offset);
			int left = 1 + random() % size, right = 1 + random() % size;
			if (left > right) std::swap(left, right);
			int merges = 0;
			for (int vertex = left; vertex < right; ++vertex) merges += interval_expected.merge(vertex, vertex + 1);
			assert(interval.merge_interval(left, right) == merges);
			assert(successor.erase(right) == (remaining.erase(right) != 0));
			const auto expected_next = remaining.lower_bound(left);
			assert(successor.next(left) == (expected_next == remaining.end() ? size + 1 : *expected_next));
			assert(successor.remaining() == int(remaining.size()));
			for (int vertex = 1; vertex <= size; ++vertex) {
				assert(parallel.same(left, vertex) == parallel_expected.same(left, vertex));
				assert(aggregate[parallel.find(vertex)] == parallel_expected.size(vertex));
				assert(interval.same(left, vertex) == interval_expected.same(left, vertex));
			}
		}
	}

	constexpr int chain = 200000;
	KruskalReconstructionTree reconstruction(chain);
	for (int vertex = 2; vertex <= chain; ++vertex) assert(reconstruction.join(vertex - 1, vertex) > chain);
	assert(reconstruction.find(1) == 2 * chain - 1);
	assert(reconstruction.find(chain) == reconstruction.find(1));
}
