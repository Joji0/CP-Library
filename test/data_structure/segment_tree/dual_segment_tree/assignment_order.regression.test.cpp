#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <optional>
#include <vector>
#include "data_structure/segment_tree/dual_segment_tree.hpp"
#include "monoid/assignment.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	DualSegmentTree<Assignment<int>> tree(4);
	tree.apply(1, 4, 5);
	tree.apply(2, 3, 8);
	assert(tree.to_vector() == (std::vector<std::optional<int>>{std::nullopt, 5, 8, 8, 5}));
	tree.apply(2, 1, 99);
	tree.apply(1, 4, std::nullopt);
	tree.apply(3, 9);
	assert(tree.get(2) == 8 && tree.get(3) == 9);
	tree.apply(1, 4, 0);
	tree.set(2, -1);
	assert(tree.get(1) == 0 && tree.get(2) == -1 && tree.get(3) == 0);
	assert(Assignment<int>::combine(1, 3) == 3);
	answer_library_checker_aplusb();
}
