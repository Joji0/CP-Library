#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <array>
#include <cassert>
#include <limits>
#include <map>
#include <random>
#include <string>
#include "data_structure/segment_tree/dynamic_segment_tree.hpp"
#include "monoid/add.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
struct Concatenate {
	using value_type = std::string;
	static std::string identity() { return {}; }
	static std::string combine(const std::string &first, const std::string &second) { return first + second; }
};
}  // namespace

int main() {
	using Index = long long;
	constexpr Index lowest = std::numeric_limits<Index>::lowest();
	constexpr Index highest = std::numeric_limits<Index>::max();
	DynamicSegmentTree<Add<long long>> empty;
	assert(empty.left_bound() == 1 && empty.right_bound() == 0 && empty.product(1, 0) == 0);

	DynamicSegmentTree<Add<long long>> tree(lowest, highest);
	std::map<Index, long long> values;
	constexpr std::array<Index, 13> positions{lowest, lowest + 1, -(1LL << 60), -999, -1, 0, 1,
						  77, 123456789, 1LL << 42, 1LL << 61, highest - 1, highest};
	const auto expected = [&](Index left, Index right) {
		long long sum = 0;
		for (auto it = values.lower_bound(left); it != values.end() && it->first <= right; ++it) sum += it->second;
		return sum;
	};
	std::mt19937 random(193);
	assert(tree.product(lowest, highest) == 0 && tree.node_count() == 0);
	for (int step = 0; step < 3000; ++step) {
		const Index position = positions[random() % positions.size()];
		const long long value = int(random() % 101) - 50;
		if (step % 2 == 0) {
			tree.set(position, value);
			values[position] = value;
		} else {
			tree.apply(position, value);
			values[position] += value;
		}
		Index left = positions[random() % positions.size()], right = positions[random() % positions.size()];
		if (right < left) std::swap(left, right);
		const std::size_t allocated = tree.node_count();
		assert(tree.get(position) == values[position]);
		assert(tree.query(left, right) == expected(left, right));
		assert(tree.all_product() == expected(lowest, highest));
		assert(tree.product(1, 0) == 0 && tree.node_count() == allocated);
	}
	tree.clear();
	assert(tree.node_count() == 0 && tree.get(highest) == 0);
	tree.set(highest, 17);
	assert(tree.get(highest) == 17);

	DynamicSegmentTree<Concatenate> strings(lowest, highest);
	strings.set(highest, "right");
	strings.set(lowest, "left");
	strings.apply(0, "mid");
	strings.apply(0, "dle");
	assert(strings.all_product() == "leftmiddleright");
	assert(strings.product(0, highest) == "middleright");
	assert(strings.product(lowest, -1) == "left");
	answer_library_checker_aplusb();
}
