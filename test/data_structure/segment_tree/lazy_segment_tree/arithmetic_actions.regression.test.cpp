#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <optional>
#include <random>
#include <vector>
#include "acted_monoid/greatest_common_divisor_assign.hpp"
#include "acted_monoid/greatest_common_divisor_multiply.hpp"
#include "acted_monoid/least_common_multiple_assign.hpp"
#include "acted_monoid/least_common_multiple_multiply.hpp"
#include "acted_monoid/maximum_subarray_sum_assign.hpp"
#include "acted_monoid/maximum_subarray_sum_multiply.hpp"
#include "acted_monoid/minimum_maximum_subarray_sum_assign.hpp"
#include "acted_monoid/minimum_maximum_subarray_sum_multiply.hpp"
#include "acted_monoid/minimum_subarray_sum_assign.hpp"
#include "acted_monoid/minimum_subarray_sum_multiply.hpp"
#include "acted_monoid/product_assign.hpp"
#include "acted_monoid/product_multiply.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
enum class Query { minimum,
		   maximum,
		   both };

template <typename Monoid, bool Assign, typename Check>
void check_updates(const Check &check, int size, long long parameter_range) {
	using Action = typename Monoid::Action;
	std::mt19937 random(91823);
	std::vector<long long> values(size + 1);
	for (int i = 1; i <= size; ++i) values[i] = int(random() % 5) - 2;
	LazySegmentTree<Monoid> tree(values);
	assert(tree.query(2, 1) == Monoid::identity());

	for (int iteration = 0; iteration < 500; ++iteration) {
		const int left = iteration % 7 == 0 ? 1 : 1 + random() % size;
		const int right = iteration % 7 == 0 ? size : left + random() % (size - left + 1);
		const long long parameter = static_cast<long long>(random() % (2 * parameter_range + 1)) - parameter_range;
		const Action action = parameter;
		tree.update(left, right, action);
		for (int i = left; i <= right; ++i) {
			if constexpr (Assign)
				values[i] = parameter;
			else
				values[i] *= parameter;
		}
		const int position = 1 + random() % size;
		const long long replacement = int(random() % 5) - 2;
		values[position] = replacement;
		tree.set_value(position, replacement);
		const int begin = iteration % 7 == 1 ? 1 : 1 + random() % size;
		const int end = iteration % 7 == 1 ? size : begin + random() % (size - begin + 1);
		check(tree.query(begin, end), values, begin, end);
	}
}

template <typename Monoid, bool Assign, Query Result>
void check_subarray() {
	check_updates<Monoid, Assign>([](const auto &aggregate, const auto &values, int left, int right) {
		using Integer = std::numeric_limits<long long>;
		long long smallest = Integer::max(), largest = Integer::lowest();
		long long prefix_minimum = Integer::max(), prefix_maximum = Integer::lowest();
		long long suffix_minimum = Integer::max(), suffix_maximum = Integer::lowest();
		for (int i = left; i <= right; ++i) {
			long long sum = 0;
			for (int j = i; j <= right; ++j) {
				sum += values[j];
				smallest = std::min(smallest, sum);
				largest = std::max(largest, sum);
				if (i == left) {
					prefix_minimum = std::min(prefix_minimum, sum);
					prefix_maximum = std::max(prefix_maximum, sum);
				}
				if (j == right) {
					suffix_minimum = std::min(suffix_minimum, sum);
					suffix_maximum = std::max(suffix_maximum, sum);
				}
			}
		}
		assert(aggregate.count == right - left + 1);
		assert(aggregate.sum == std::accumulate(values.begin() + left, values.begin() + right + 1, 0LL));
		assert(aggregate.subarray.minimum == smallest && aggregate.subarray.maximum == largest);
		assert(aggregate.prefix.minimum == prefix_minimum && aggregate.prefix.maximum == prefix_maximum);
		assert(aggregate.suffix.minimum == suffix_minimum && aggregate.suffix.maximum == suffix_maximum);
		if constexpr (Result == Query::minimum) assert(Monoid::extremum(aggregate) == smallest);
		if constexpr (Result == Query::maximum) assert(Monoid::extremum(aggregate) == largest);
	},
				      17, Assign ? 5 : 1);
}

template <typename Monoid, bool Assign, bool Least>
void check_divisor() {
	check_updates<Monoid, Assign>([](const auto &aggregate, const auto &values, int left, int right) {
		unsigned long long result = Least ? 1 : 0;
		for (int i = left; i <= right; ++i) {
			const auto magnitude = static_cast<unsigned long long>(values[i] < 0 ? -values[i] : values[i]);
			if constexpr (Least)
				result = std::lcm(result, magnitude);
			else
				result = std::gcd(result, magnitude);
		}
		assert(!aggregate.empty && aggregate.value == result);
	},
				      15, Assign ? 5 : 2);
}

template <typename Monoid, bool Assign>
void check_product() {
	check_updates<Monoid, Assign>([](const auto &aggregate, const auto &values, int left, int right) {
		long long result = 1;
		for (int i = left; i <= right; ++i) result *= values[i];
		assert(aggregate.count == right - left + 1 && aggregate.value == result);
	},
				      11, Assign ? 5 : 1);
}

void check_boundaries() {
	using Assign = ProductAssign<>;
	LazySegmentTree<Assign> assigned(63);
	assigned.update(1, 63, -2);
	assert(assigned.all_product().value == std::numeric_limits<long long>::lowest());

	using Multiply = ProductMultiply<>;
	std::vector<long long> values(64, 1);
	values[1] = -1;
	LazySegmentTree<Multiply> multiplied(values);
	multiplied.update(1, 63, 2);
	assert(multiplied.all_product().value == std::numeric_limits<long long>::lowest());

	using Greatest = GreatestCommonDivisorMultiply<>;
	LazySegmentTree<Greatest> gcd(std::vector<long long>{0, 1, -1});
	gcd.update(1, 2, std::numeric_limits<long long>::lowest());
	assert(gcd.all_product().value == 1ULL << 63);
	assert(gcd.get(2).value == 1ULL << 63);
	gcd.update(1, 2, 0);
	assert(gcd.get(2).value == 0);

	using Least = LeastCommonMultipleAssign<>;
	LazySegmentTree<Least> lcm(std::vector<long long>{0, -3, -2});
	assert(lcm.all_product().value == 6);
	lcm.update(1, 2, std::numeric_limits<long long>::lowest());
	assert(lcm.get(1).value == 1ULL << 63);
}
}  // namespace

int main() {
	check_subarray<MinimumSubarraySumAssign<>, true, Query::minimum>();
	check_subarray<MaximumSubarraySumAssign<>, true, Query::maximum>();
	check_subarray<MinimumMaximumSubarraySumAssign<>, true, Query::both>();
	check_divisor<GreatestCommonDivisorAssign<>, true, false>();
	check_divisor<LeastCommonMultipleAssign<>, true, true>();
	check_product<ProductAssign<>, true>();
	check_subarray<MinimumSubarraySumMultiply<>, false, Query::minimum>();
	check_subarray<MaximumSubarraySumMultiply<>, false, Query::maximum>();
	check_subarray<MinimumMaximumSubarraySumMultiply<>, false, Query::both>();
	check_divisor<GreatestCommonDivisorMultiply<>, false, false>();
	check_divisor<LeastCommonMultipleMultiply<>, false, true>();
	check_product<ProductMultiply<>, false>();
	check_boundaries();
	answer_library_checker_aplusb();
}
