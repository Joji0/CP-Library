#include <algorithm>
#include <cassert>
#include <numeric>
#include <random>
#include <vector>
#include "data_structure/segment_tree/persistent_segment_tree.hpp"
#include "monoid/add.hpp"

int main() {
	PersistentSegmentTree<Add<long long>> empty(std::vector<long long>{999});
	assert(empty.query(empty.roots[0], 1, 0) == 0);

	PersistentSegmentTree<Add<long long>> single(std::vector<long long>{999, 7});
	const int initial = single.roots[1];
	const int latest = single.update(1, -4);
	const int branch = single.update(initial, 1, 10);
	assert(single.get(initial, 1) == 7);
	assert(single.get(latest, 1) == -4);
	assert(single.get(branch, 1) == 10);

	std::mt19937 random(43291);
	for (int trial = 0; trial < 32; ++trial) {
		const int size = 1 + random() % 40;
		std::vector<long long> values(size + 1, 999);
		for (int i = 1; i <= size; ++i) values[i] = int(random() % 31) - 15;
		PersistentSegmentTree<Add<long long>> tree(values);
		std::vector<std::vector<long long>> versions{{}, values};
		for (int step = 0; step < 150; ++step) {
			const int parent_version = 1 + random() % (versions.size() - 1);
			const int position = 1 + random() % size;
			const long long replacement = int(random() % 81) - 40;
			const int new_root = tree.update(tree.roots[parent_version], position, replacement);
			auto next = versions[parent_version];
			next[position] = replacement;
			versions.push_back(next);
			assert(tree.roots.size() == versions.size() && tree.roots.back() == new_root);
			assert(tree.get(new_root, position) == replacement);
			assert(tree.get(tree.roots[parent_version], position) == versions[parent_version][position]);
			for (int version : {1, parent_version, int(versions.size()) - 1}) {
				int left = 1 + random() % size, right = 1 + random() % size;
				if (left > right) std::swap(left, right);
				const auto &expected = versions[version];
				const int root = tree.roots[version];
				assert(tree.query(root, left, right) ==
				       std::accumulate(expected.begin() + left, expected.begin() + right + 1, 0LL));
				assert(tree.query(root, 1, size) == std::accumulate(expected.begin() + 1, expected.end(), 0LL));
			}
		}
	}
}
