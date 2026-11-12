#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <random>
#include <vector>
#include "acted_monoid/flip_sum.hpp"
#include "acted_monoid/sum_add.hpp"
#include "acted_monoid/sum_add_assign.hpp"
#include "acted_monoid/sum_affine.hpp"
#include "acted_monoid/sum_assign.hpp"
#include "data_structure/segment_tree/dual_segment_tree.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "data_structure/segment_tree/persistent_segment_tree.hpp"
#include "data_structure/segment_tree/segment_tree.hpp"
#include "data_structure/segment_tree/segment_tree_beats.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "monoid/add.hpp"
#include "monoid/affine.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
void boundary_searches() {
	SegmentTree<Add<int>> empty(std::vector<int>{999});
	assert(empty.empty() && empty.all_product() == 0 && empty.query(1, 0) == 0);
	assert(empty.max_right(1, [](int) { return true; }) == 0);
	assert(empty.min_left(0, [](int) { return true; }) == 1);

	const std::vector<int> values{999, 2, 3, 5};
	SegmentTree<Add<int>> tree(values);
	PersistentSegmentTree<Add<int>> persistent(values);
	const int root = persistent.root_at_version(1);
	const int changed = persistent.update(root, 3, 7);
	const int branched = persistent.update(root, 1, 4);
	assert(tree.query(1, 3) == 10 && tree.get(1) == 2 && tree.get(3) == 5);
	assert(persistent.query(root, 1, 3) == 10);
	assert(persistent.query(changed, 1, 3) == 12);
	assert(persistent.query(branched, 1, 3) == 12 && persistent.get(branched, 3) == 5);

	const auto at_most_four = [](int sum) { return sum <= 4; };
	assert(tree.max_right(1, at_most_four) == 1);
	assert(tree.max_right(3, at_most_four) == 2);
	assert(tree.min_left(2, at_most_four) == 2);
	assert(tree.min_left(3, at_most_four) == 4);
	assert(persistent.min_left(root, 1, at_most_four) == 1);
	assert(persistent.size() == 3 && persistent.version_count() == 3 && persistent.latest_root() == branched);
	assert(persistent.all_product(root) == 10 && persistent.all_product(branched) == 12);
	tree.set(3, -5);
	assert(tree.size() == 3 && !tree.empty() && tree.all_product() == 0);
}

void automatic_leaves() {
	LazySegmentTree<SumAdd<int>> empty(0);
	assert(empty.empty() && empty.all_product().cnt == 0);
	LazySegmentTree<SumAdd<int>> sum(5);
	assert(sum.size() == 5 && sum.all_product().cnt == 5);
	sum.update(1, 5, 2);
	sum.update(2, 4, -1);
	assert(sum.all_product().val == 7);
	LazySegmentTree<SumAssign<int>> assigned(4, SumAssign<int>::make_leaf(3));
	assigned.update(2, 4, 7);
	assert(assigned.all_product().val == 24);
	LazySegmentTree<SumAddAssign<int>> both(4);
	both.update(1, 4, SumAddAssign<int>::make_add(3));
	both.update(2, 4, SumAddAssign<int>::make_set(7));
	both.update(1, 2, SumAddAssign<int>::make_add(-1));
	assert(both.all_product().val == 22);
	LazySegmentTree<FlipSum<int>> bits(7);
	bits.update(2, 6, true);
	bits.update(4, 7, true);
	assert(bits.all_product().sum == 3);
}

void overlapping_updates() {
	using Modular = ModularInteger998244353;
	using Action = SumAffine<Modular>;
	using Function = Affine<Modular>;
	constexpr int size = 13;
	std::vector<Modular> values(size + 1, 999);
	std::vector<Function::value_type> points(size + 1, {999, 999});
	for (int position = 1; position <= size; ++position) {
		values[position] = position;
		points[position] = {0, position};
	}
	auto lazy = LazySegmentTree<Action>::from_values(values);
	DualSegmentTree<Function> dual(points);
	lazy.update(size + 1, size, {7, 11});
	dual.apply(size + 1, size, {7, 11});
	assert(lazy.query(2, 1).val == 0);

	std::mt19937 random(1847);
	for (int step = 0; step < 600; ++step) {
		int left = 1 + random() % size, right = 1 + random() % size;
		if (left > right) std::swap(left, right);
		const Modular scale = random() % 9, offset = random() % 13;
		lazy.update(left, right, {scale, offset});
		dual.apply(left, right, {scale, offset});
		for (int i = left; i <= right; ++i) values[i] = scale * values[i] + offset;
		if (step % 5 == 0) {
			const int point = 1 + random() % size;
			const Modular replacement = random() % 40;
			values[point] = replacement;
			lazy.set_value(point, replacement);
			dual.set(point, {0, replacement});
		}
		for (int i : {1, left, right, size}) {
			assert(dual.get(i).b == values[i] && lazy.get(i).val == values[i]);
		}
		assert(lazy.query(left, right).val ==
		       std::accumulate(values.begin() + left, values.begin() + right + 1, Modular(0)));
	}
	assert(dual.to_vector().size() == size + 1);
}

void beats_updates() {
	constexpr int size = 23;
	std::vector<long long> values(size + 1, 3);
	values[0] = 999;
	SegmentTreeBeats<long long> tree(values);
	tree.add(size + 1, size, 77);
	assert(tree.sum(1, 0) == 0);
	std::mt19937 random(49713);
	for (int step = 0; step < 1000; ++step) {
		int left = 1 + random() % size, right = 1 + random() % size;
		if (left > right) std::swap(left, right);
		const long long value = int(random() % 41) - 20;
		if (step % 3 == 0) {
			tree.chmin(left, right, value);
			for (int i = left; i <= right; ++i) values[i] = std::min(values[i], value);
		} else if (step % 3 == 1) {
			tree.chmax(left, right, value);
			for (int i = left; i <= right; ++i) values[i] = std::max(values[i], value);
		} else {
			tree.add(left, right, value);
			for (int i = left; i <= right; ++i) values[i] += value;
		}
		assert(tree.sum(left, right) == std::accumulate(values.begin() + left, values.begin() + right + 1, 0LL));
		const auto [minimum, maximum] = std::minmax_element(values.begin() + left, values.begin() + right + 1);
		assert(tree.min(left, right) == *minimum && tree.max(left, right) == *maximum);
		assert(tree.get(1) == values[1] && tree.get(size) == values[size]);
		assert(tree.sum(1, size) == std::accumulate(values.begin() + 1, values.end(), 0LL));
	}
}
void beats_extreme_values() {
	constexpr long long lowest = std::numeric_limits<long long>::lowest();
	constexpr long long highest = std::numeric_limits<long long>::max();
	SegmentTreeBeats<long long> single(1);
	single.chmin(1, 1, lowest);
	assert(single.get(1) == lowest);
	single.chmax(1, 1, highest);
	assert(single.get(1) == highest);
	single.set(1, lowest);
	assert(single.min(1, 1) == lowest && single.max(1, 1) == lowest);

	SegmentTreeBeats<long long> low_pair(std::vector<long long>{999, lowest, 1});
	low_pair.add(1, 2, 1);
	assert(low_pair.min(1, 2) == lowest + 1);
	low_pair.chmax(1, 2, 1);
	assert(low_pair.sum(1, 2) == 3 && low_pair.get(1) == 1);
	SegmentTreeBeats<long long> high_pair(std::vector<long long>{999, highest, -2});
	high_pair.add(1, 2, -1);
	assert(high_pair.max(1, 2) == highest - 1);
	high_pair.chmin(1, 2, -2);
	assert(high_pair.sum(1, 2) == -5 && high_pair.get(1) == -2);

	const long long large = highest - 5;
	SegmentTreeBeats<long long> capped(std::vector<long long>{999, large, -large});
	capped.chmin(1, 2, 0);
	capped.add(1, 2, 10);
	assert(capped.get(1) == 10 && capped.get(2) == -large + 10);
	SegmentTreeBeats<long long> floored(std::vector<long long>{999, -large, large});
	floored.chmax(1, 2, 0);
	floored.add(1, 2, -10);
	assert(floored.get(1) == -10 && floored.get(2) == large - 10);

	SegmentTreeBeats<long long> pending(std::vector<long long>{999, -highest, -highest});
	pending.add(1, 2, highest);
	pending.add(1, 2, highest);
	assert(pending.get(2) == highest);
	pending.chmin(1, 2, highest / 2);
	assert(pending.sum(1, 2) == 2 * (highest / 2));

	using Unsigned = unsigned long long;
	constexpr Unsigned unsigned_max = std::numeric_limits<Unsigned>::max();
	SegmentTreeBeats<Unsigned> unsigned_tree(std::vector<Unsigned>{999, unsigned_max - 1, 0});
	unsigned_tree.chmin(1, 2, 0);
	unsigned_tree.add(1, 2, 2);
	assert(unsigned_tree.get(1) == 2 && unsigned_tree.sum(1, 2) == 4);
}
}  // namespace

int main() {
	boundary_searches();
	automatic_leaves();
	overlapping_updates();
	beats_updates();
	beats_extreme_values();
	answer_library_checker_aplusb();
}
