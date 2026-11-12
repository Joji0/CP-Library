#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <random>
#include <vector>
#include "acted_monoid/maximum_add.hpp"
#include "acted_monoid/maximum_assign.hpp"
#include "acted_monoid/maximum_count_add.hpp"
#include "acted_monoid/maximum_index_add.hpp"
#include "acted_monoid/minimum_add.hpp"
#include "acted_monoid/minimum_assign.hpp"
#include "acted_monoid/minimum_count_add.hpp"
#include "acted_monoid/minimum_index_add.hpp"
#include "acted_monoid/minimum_maximum_affine.hpp"
#include "acted_monoid/sum_add_assign.hpp"
#include "acted_monoid/sum_assign.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "data_structure/treap/implicit_treap.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
void boundaries() {
	constexpr auto low = std::numeric_limits<long long>::lowest();
	constexpr auto high = std::numeric_limits<long long>::max();
	LazySegmentTree<MinimumAssign<>> minimum(std::vector<long long>{999, 1, 2});
	LazySegmentTree<MaximumAssign<>> maximum(2);
	minimum.update(1, 2, high);
	maximum.update(1, 2, low);
	assert(minimum.query(1, 1) == high && maximum.query(2, 2) == low);
	assert(!minimum.query(2, 1) && !maximum.query(2, 1));
	minimum.update(2, low);
	maximum.update(2, high);
	assert(minimum.all_product() == low && maximum.all_product() == high);
	LazySegmentTree<MinimumAdd<>> minimum_add(2, high);
	ImplicitTreap<MaximumAdd<>> maximum_add(std::vector<long long>{999, low, low});
	minimum_add.update(1, 2, -1);
	maximum_add.apply(1, 2, 1);
	assert(minimum_add.query(2, 2) == high - 1 && maximum_add.get(2) == low + 1);
	assert(!MinimumAdd<>::mapping(1, MinimumAdd<>::identity()));
	assert(!MaximumAdd<>::mapping(-1, MaximumAdd<>::identity()));

	using MinimumCount = MinimumCountAdd<>;
	using MaximumCount = MaximumCountAdd<>;
	using MinimumIndex = MinimumIndexAdd<>;
	using MaximumIndex = MaximumIndexAdd<>;
	assert(MinimumCount::mapping(-1, MinimumCount::make_leaf(high)).val == high - 1);
	assert(MaximumCount::mapping(1, MaximumCount::make_leaf(low)).val == low + 1);
	assert(MinimumIndex::mapping(-1, MinimumIndex::make_leaf(high, 2)).val == high - 1);
	assert(MaximumIndex::mapping(1, MaximumIndex::make_leaf(low, 2)).val == low + 1);
	assert(MinimumIndex::combine(MinimumIndex::identity(), MinimumIndex::make_leaf(high, 2)).idx == 2);
	assert(MaximumIndex::combine(MaximumIndex::make_leaf(low, 2), MaximumIndex::identity()).idx == 2);

	LazySegmentTree<SumAssign<>> sum(1);
	sum.update(1, low);
	assert(sum.get(1).val == low);
	LazySegmentTree<SumAddAssign<>> sum_add(1);
	sum_add.update(1, SumAddAssign<>::make_set(low));
	sum_add.update(1, SumAddAssign<>::make_add(2));
	assert(sum_add.get(1).val == low + 2);
}

void mixed_updates() {
	using Sum = SumAddAssign<long long>;
	using Bounds = MinimumMaximumAffine<long long>;
	std::mt19937 random(32871);
	constexpr int size = 24;
	std::vector<long long> sums(size + 1, 0), extrema(size + 1, 0);
	sums[0] = extrema[0] = 999;
	LazySegmentTree<Sum> sum_tree(sums);
	ImplicitTreap<Sum> sum_sequence(sums);
	LazySegmentTree<Bounds> extrema_tree(extrema);
	ImplicitTreap<Bounds> extrema_sequence(extrema);
	for (int step = 0; step < 1800; ++step) {
		int left = 1 + random() % size, right = 1 + random() % size;
		if (left > right) std::swap(left, right);
		const long long value = int(random() % 21) - 10;
		const bool assign = step % 2 == 0;
		const auto update = assign ? Sum::make_set(value) : Sum::make_add(value);
		sum_tree.update(left, right, update);
		sum_sequence.apply(left, right, update);
		const long long slope = int(random() % 3) - 1;
		extrema_tree.update(left, right, {slope, value});
		extrema_sequence.apply(left, right, {slope, value});
		for (int i = left; i <= right; ++i) {
			sums[i] = assign ? value : sums[i] + value;
			extrema[i] = slope * extrema[i] + value;
		}
		if (step % 5 == 0) {
			extrema_sequence.reverse(left, right);
			std::reverse(extrema.begin() + left, extrema.begin() + right + 1);
			for (int i = left; i <= right; ++i) extrema_tree.set_value(i, extrema[i]);
		}
		int first = 1 + random() % size, last = 1 + random() % size;
		if (first > last) std::swap(first, last);
		const long long expected_sum = std::accumulate(sums.begin() + first, sums.begin() + last + 1, 0LL);
		assert(sum_tree.query(first, last).val == expected_sum && sum_sequence.product(first, last).val == expected_sum);
		const auto [minimum, maximum] = std::minmax_element(extrema.begin() + first, extrema.begin() + last + 1);
		const Bounds::Extrema expected{*minimum, *maximum};
		assert(extrema_tree.query(first, last) == expected && extrema_sequence.product(first, last) == expected);
	}
}
}  // namespace

int main() {
	boundaries();
	mixed_updates();
	answer_library_checker_aplusb();
}
