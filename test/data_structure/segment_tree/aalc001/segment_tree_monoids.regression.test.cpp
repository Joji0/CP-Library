#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <limits>
#include <random>
#include <vector>
#include "acted_monoid/array_permutation.hpp"
#include "acted_monoid/historical_sum_add_assign.hpp"
#include "acted_monoid/minimum_threshold_history_add.hpp"
#include "acted_monoid/minimum_weight_sum_add.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/add_modulo_composition.hpp"
#include "monoid/equal_runs.hpp"
#include "monoid/finite_alphabet_inversions.hpp"
#include "monoid/maximum_disjoint_subarray_sum.hpp"
#include "monoid/maximum_subarray_sum.hpp"
#include "monoid/position_weighted_sum.hpp"
#include "monoid/weighted_pattern_subsequences.hpp"
#include "test/detail/library_checker_aplusb.hpp"

void test_point_updates(std::mt19937 &random) {
	using Inversions = FiniteAlphabetInversions<3>;
	using Runs = EqualRuns<int>;
	using Subarrays = MaximumDisjointSubarraySum<long long, 2>;
	using Pattern = WeightedPatternSubsequences<std::array{0, 1, 0}>;
	using Weighted = PositionWeightedSum<>;
	using AddMod = AddModuloComposition<13>;
	const int n = 8;
	std::vector<int> symbols(n + 1);
	std::vector<long long> weights(n + 1);
	SegmentTree<Inversions> inversions(n);
	SegmentTree<Runs> runs(n);
	SegmentTree<Subarrays> subarrays(n);
	SegmentTree<Pattern> patterns(n);
	SegmentTree<Weighted> weighted(n);
	SegmentTree<AddMod> functions(n);
	auto set = [&](int position) {
		const int symbol = symbols[position] = int(random() % 3);
		const long long weight = weights[position] = int(random() % 25) - 11;
		inversions.set(position, Inversions::make_leaf(symbol));
		runs.set(position, Runs::make_leaf(symbol));
		subarrays.set(position, Subarrays::make_leaf(weight));
		patterns.set(position, Pattern::make_leaf(symbol, weight));
		weighted.set(position, Weighted::make_leaf(weight));
		functions.set(position, symbol == 0 ? AddMod::make_modulo() : AddMod::make_add(weight));
	};
	for (int i = 1; i <= n; ++i) set(i);
	for (int query = 0; query < 220; ++query) {
		const int position = 1 + random() % n;
		set(position);
		int left = 1 + random() % n, right = 1 + random() % n;
		if (left > right) std::swap(left, right);
		long long inverse_count = 0, pattern_count = 0, position_sum = 0;
		int longest = 0, length = 0;
		long long seed = int(random() % 60) - 30, transformed = seed;
		for (int i = left; i <= right; ++i) {
			length = i > left && symbols[i - 1] == symbols[i] ? length + 1 : 1;
			longest = std::max(longest, length);
			position_sum += weights[i] * (i - left + 1);
			transformed = symbols[i] ? transformed + weights[i] : (transformed % 13 + 13) % 13;
			for (int j = i + 1; j <= right; ++j) {
				inverse_count += symbols[i] > symbols[j];
				for (int k = j + 1; k <= right; ++k) {
					if (symbols[i] == 0 && symbols[j] == 1 && symbols[k] == 0) pattern_count += weights[i] * weights[j] * weights[k];
				}
			}
		}
		assert(inversions.query(left, right).inversions == inverse_count);
		assert(runs.query(left, right).longest == longest);
		assert(Pattern::weighted_count(patterns.query(left, right)) == pattern_count);
		assert(weighted.query(left, right).weighted_sum == position_sum);
		assert(AddMod::evaluate(functions.query(left, right), seed) == transformed);
		std::optional<long long> exactly_two;
		long long up_to_two = 0;
		for (unsigned mask = 0; mask < (1U << (right - left + 1)); ++mask) {
			const auto components = std::popcount(mask & ~(mask << 1));
			if (components > 2) continue;
			long long sum = 0;
			for (int i = left; i <= right; ++i)
				if ((mask >> (i - left)) & 1) sum += weights[i];
			up_to_two = std::max(up_to_two, sum);
			if (std::popcount(mask) >= 2 && (!exactly_two || *exactly_two < sum)) exactly_two = sum;
		}
		const auto product = subarrays.query(left, right);
		assert(Subarrays::maximum_sum(product, 0) == up_to_two);
		assert(Subarrays::maximum_sum(product, 2, 2) == exactly_two);
	}
	const long long endpoint = std::numeric_limits<long long>::max();
	auto wide = AddMod::combine(AddMod::make_add(endpoint), AddMod::make_modulo());
	assert(AddMod::evaluate(wide, endpoint) == (2 * __int128(endpoint)) % 13);
	const auto repeated = Weighted::make_repeated(-3, 7);
	assert(repeated.length == 7 && repeated.sum == -21 && repeated.weighted_sum == -84);
}

void test_range_actions(std::mt19937 &random) {
	using Prefix = MaximumSubarraySum<>;
	using Permute = ArrayPermutation<Prefix, 3>;
	using Historical = HistoricalSumAddAssign<>;
	using Observed = MinimumThresholdHistoryAdd<>;
	using Weighted = MinimumWeightSumAdd<>;
	const int n = 21;
	std::array<std::array<long long, n + 1>, 4> arrays{};
	std::vector<Permute::Value> permuted(n + 1, Permute::identity());
	std::vector<Weighted::Value> leaves(n + 1);
	std::vector<long long> scores(n + 1), weights(n + 1), current(n + 1), history(n + 1), observed_sum(n + 1);
	for (int i = 1; i <= n; ++i) {
		for (int component = 1; component <= 3; ++component) {
			arrays[component][i] = int(random() % 23) - 12;
			permuted[i][component] = Prefix::from_val(arrays[component][i]);
		}
		weights[i] = int(random() % 15) - 7;
		leaves[i] = Weighted::make_leaf(0, weights[i]);
	}
	LazySegmentTree<Permute> permute(permuted);
	LazySegmentTree<Historical> historical(n);
	LazySegmentTree<Observed> observed(n);
	LazySegmentTree<Weighted> weighted(leaves);
	for (int iteration = 0; iteration < 750; ++iteration) {
		int left = 1 + random() % n, right = 1 + random() % n;
		if (left > right) std::swap(left, right);
		const long long delta = int(random() % 21) - 10;
		const long long amount = random() % 7;
		const int source = 1 + random() % 3, target = 1 + random() % 3, kind = random() % 3;
		permute.update(left, right, Permute::make_swap(source, target));
		observed.update(left, right, Observed::make_add(delta));
		weighted.update(left, right, delta);
		if (kind == 0)
			historical.update(left, right, Historical::make_add(delta));
		else if (kind == 1)
			historical.update(left, right, Historical::make_assign(delta));
		else
			historical.update(left, right, Historical::make_accumulate(amount));
		for (int i = left; i <= right; ++i) {
			std::swap(arrays[source][i], arrays[target][i]);
			scores[i] += delta;
			if (kind == 0)
				current[i] += delta;
			else if (kind == 1)
				current[i] = delta;
			else
				history[i] += current[i] * amount;
		}
		const long long threshold = *std::min_element(scores.begin() + left, scores.begin() + right + 1) - static_cast<long long>(random() % 2);
		observed.update(left, right, Observed::make_observe(threshold, amount));
		for (int i = left; i <= right; ++i) {
			if (scores[i] == threshold) observed_sum[i] += amount;
		}
		// Query an independent range, including strict subranges of pending actions.
		left = 1 + random() % n;
		right = 1 + random() % n;
		if (left > right) std::swap(left, right);
		long long weight_sum = 0, history_sum = 0, current_sum = 0, observation_sum = 0;
		const long long minimum = *std::min_element(scores.begin() + left, scores.begin() + right + 1);
		for (int i = left; i <= right; ++i) {
			history_sum += history[i];
			current_sum += current[i];
			observation_sum += observed_sum[i];
			if (scores[i] == minimum) weight_sum += weights[i];
		}
		auto record = historical.query(left, right);
		assert(record.current_sum == current_sum && record.historical_sum == history_sum);
		assert(observed.query(left, right).historical_sum == observation_sum);
		auto min_weights = weighted.query(left, right);
		assert(min_weights.minimum == minimum && min_weights.weight_sum == weight_sum);
		for (int c = 1; c <= 3; ++c) {
			long long prefix_sum = 0, best = std::numeric_limits<long long>::min();
			for (int i = left; i <= right; ++i) best = std::max(best, prefix_sum += arrays[c][i]);
			assert(permute.query(left, right)[c].pref == best);
		}
	}
}

int main() {
	std::mt19937 random(291734);
	test_point_updates(random);
	test_range_actions(random);
	answer_library_checker_aplusb();
}
