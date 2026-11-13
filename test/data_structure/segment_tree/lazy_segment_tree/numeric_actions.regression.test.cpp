#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <optional>
#include <random>
#include <utility>
#include <vector>
#include "acted_monoid/maximum_add_assign.hpp"
#include "acted_monoid/maximum_count_add_assign.hpp"
#include "acted_monoid/maximum_count_affine.hpp"
#include "acted_monoid/maximum_count_assign.hpp"
#include "acted_monoid/maximum_count_multiply.hpp"
#include "acted_monoid/maximum_index_add_assign.hpp"
#include "acted_monoid/maximum_index_affine.hpp"
#include "acted_monoid/maximum_index_assign.hpp"
#include "acted_monoid/maximum_index_multiply.hpp"
#include "acted_monoid/maximum_multiply.hpp"
#include "acted_monoid/minimum_add_assign.hpp"
#include "acted_monoid/minimum_count_add_assign.hpp"
#include "acted_monoid/minimum_count_affine.hpp"
#include "acted_monoid/minimum_count_assign.hpp"
#include "acted_monoid/minimum_count_multiply.hpp"
#include "acted_monoid/minimum_index_add_assign.hpp"
#include "acted_monoid/minimum_index_affine.hpp"
#include "acted_monoid/minimum_index_assign.hpp"
#include "acted_monoid/minimum_index_multiply.hpp"
#include "acted_monoid/minimum_maximum_add_assign.hpp"
#include "acted_monoid/minimum_maximum_count_add.hpp"
#include "acted_monoid/minimum_maximum_count_add_assign.hpp"
#include "acted_monoid/minimum_maximum_count_affine.hpp"
#include "acted_monoid/minimum_maximum_count_assign.hpp"
#include "acted_monoid/minimum_maximum_count_multiply.hpp"
#include "acted_monoid/minimum_maximum_index_add.hpp"
#include "acted_monoid/minimum_maximum_index_add_assign.hpp"
#include "acted_monoid/minimum_maximum_index_affine.hpp"
#include "acted_monoid/minimum_maximum_index_assign.hpp"
#include "acted_monoid/minimum_maximum_index_multiply.hpp"
#include "acted_monoid/minimum_maximum_multiply.hpp"
#include "acted_monoid/minimum_multiply.hpp"
#include "acted_monoid/sum_maximum_add_assign.hpp"
#include "acted_monoid/sum_maximum_affine.hpp"
#include "acted_monoid/sum_maximum_multiply.hpp"
#include "acted_monoid/sum_minimum_add_assign.hpp"
#include "acted_monoid/sum_minimum_affine.hpp"
#include "acted_monoid/sum_minimum_maximum_add.hpp"
#include "acted_monoid/sum_minimum_maximum_add_assign.hpp"
#include "acted_monoid/sum_minimum_maximum_assign.hpp"
#include "acted_monoid/sum_minimum_maximum_multiply.hpp"
#include "acted_monoid/sum_minimum_multiply.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
enum class Update { add,
		    assign,
		    add_assign,
		    multiply,
		    affine };
enum class Query { minimum,
		   maximum,
		   both };
enum class Kind { bounds,
		  sum_bounds,
		  count,
		  index };

template <typename Monoid, Update Operation, Query Result, Kind Aggregate>
void check() {
	using Action = typename Monoid::Action;
	constexpr bool indexed = Aggregate == Kind::index;
	constexpr int size = 19;
	std::mt19937 random(1021);
	std::vector<long long> values(size + 1);
	for (int i = 1; i <= size; ++i) values[i] = int(random() % 13) - 6;
	const auto make_tree = [&] {
		if constexpr (indexed) {
			std::vector<typename Monoid::Value> leaves(size + 1);
			for (int i = 1; i <= size; ++i) leaves[i] = Monoid::make_leaf(values[i], i);
			return LazySegmentTree<Monoid>(leaves);
		} else {
			return LazySegmentTree<Monoid>(values);
		}
	};
	auto tree = make_tree();
	for (int iteration = 0; iteration < 320; ++iteration) {
		const int left = iteration % 5 == 0 ? 1 : 1 + random() % size;
		const int right = iteration % 5 == 0 ? size : left + random() % (size - left + 1);
		const long long parameter = int(random() % 13) - 6;
		const long long slope = int(random() % 3) - 1;
		const bool set = (random() & 1) != 0;
		const auto [factor, offset] = [&]() -> std::pair<long long, long long> {
			if constexpr (Operation == Update::add) return {1, parameter};
			if constexpr (Operation == Update::assign) return set ? std::pair{0LL, parameter} : std::pair{1LL, 0LL};
			if constexpr (Operation == Update::add_assign) return {set ? 0 : 1, parameter};
			if constexpr (Operation == Update::multiply) return {slope, 0};
			if constexpr (Operation == Update::affine) return {slope, parameter};
		}();
		const Action action = [&]() -> Action {
			if constexpr (Operation == Update::add) return parameter;
			if constexpr (Operation == Update::assign) return set ? Action(parameter) : std::nullopt;
			if constexpr (Operation == Update::add_assign) return set ? Monoid::make_set(parameter) : Monoid::make_add(parameter);
			if constexpr (Operation == Update::multiply) return slope;
			if constexpr (Operation == Update::affine) return {factor, offset};
		}();
		tree.update(left, right, action);
		for (int i = left; i <= right; ++i) values[i] = factor * values[i] + offset;

		const int begin = 1 + random() % size;
		const int end = begin + random() % (size - begin + 1);
		const auto first = values.begin() + begin;
		const auto last = values.begin() + end + 1;
		const auto minimum = std::min_element(first, last);
		const auto maximum = std::max_element(first, last);
		const auto actual = tree.query(begin, end);
		if constexpr (Aggregate == Kind::bounds || Aggregate == Kind::sum_bounds) {
			const auto &bounds = [&]() -> const auto & {
				if constexpr (Aggregate == Kind::bounds)
					return actual;
				else
					return actual.extrema;
			}();
			assert(bounds->minimum == *minimum && bounds->maximum == *maximum);
			if constexpr (Aggregate == Kind::sum_bounds) assert(actual.sum.val == std::accumulate(first, last, 0LL));
			if constexpr (Result == Query::minimum) assert(Monoid::extremum(actual) == *minimum);
			if constexpr (Result == Query::maximum) assert(Monoid::extremum(actual) == *maximum);
		} else {
			assert(actual.count == end - begin + 1);
			assert(actual.minimum.value == *minimum && actual.maximum.value == *maximum);
			assert(actual.minimum.count == std::count(first, last, *minimum));
			assert(actual.maximum.count == std::count(first, last, *maximum));
			if constexpr (indexed) {
				assert(actual.minimum.index == minimum - values.begin());
				assert(actual.maximum.index == maximum - values.begin());
			}
			if constexpr (Result == Query::minimum) assert(Monoid::extremum(actual) == actual.minimum);
			if constexpr (Result == Query::maximum) assert(Monoid::extremum(actual) == actual.maximum);
		}
		if constexpr (Result != Query::both) assert(!Monoid::extremum(tree.query(2, 1)));
	}
}

void check_boundaries() {
	using Assign = MinimumMaximumIndexAssign<>;
	using Integer = std::numeric_limits<long long>;
	LazySegmentTree<Assign> tree(std::vector<Assign::Value>{{}, Assign::make_leaf(Integer::max(), 1), Assign::make_leaf(Integer::lowest(), 2), Assign::make_leaf(Integer::max(), 3)});
	assert(tree.all_product().minimum.index == 2);
	assert(tree.all_product().maximum.index == 1);
	tree.update(1, 3, Integer::lowest());
	assert(tree.all_product().maximum.count == 3);
	assert(tree.all_product().maximum.index == 1);

	using Affine = MinimumMaximumCountAffine<>;
	LazySegmentTree<Affine> affine(std::vector<long long>{0, Integer::max(), Integer::max(), Integer::lowest() + 1});
	affine.update(1, 3, {-1, 0});
	assert(affine.all_product().minimum.value == -Integer::max());
	assert(affine.all_product().minimum.count == 2);
	assert(affine.all_product().maximum.value == Integer::max());
}
}  // namespace

int main() {
	check<MinimumAddAssign<long long>, Update::add_assign, Query::minimum, Kind::bounds>();
	check<SumMinimumAddAssign<long long>, Update::add_assign, Query::minimum, Kind::sum_bounds>();
	check<MinimumMultiply<long long>, Update::multiply, Query::minimum, Kind::bounds>();
	check<SumMinimumMultiply<long long>, Update::multiply, Query::minimum, Kind::sum_bounds>();
	check<SumMinimumAffine<long long>, Update::affine, Query::minimum, Kind::sum_bounds>();
	check<MinimumCountAssign<long long>, Update::assign, Query::minimum, Kind::count>();
	check<MinimumCountAddAssign<long long>, Update::add_assign, Query::minimum, Kind::count>();
	check<MinimumCountMultiply<long long>, Update::multiply, Query::minimum, Kind::count>();
	check<MinimumCountAffine<long long>, Update::affine, Query::minimum, Kind::count>();
	check<MinimumIndexAssign<long long>, Update::assign, Query::minimum, Kind::index>();
	check<MinimumIndexAddAssign<long long>, Update::add_assign, Query::minimum, Kind::index>();
	check<MinimumIndexMultiply<long long>, Update::multiply, Query::minimum, Kind::index>();
	check<MinimumIndexAffine<long long>, Update::affine, Query::minimum, Kind::index>();
	check<MaximumAddAssign<long long>, Update::add_assign, Query::maximum, Kind::bounds>();
	check<SumMaximumAddAssign<long long>, Update::add_assign, Query::maximum, Kind::sum_bounds>();
	check<MaximumMultiply<long long>, Update::multiply, Query::maximum, Kind::bounds>();
	check<SumMaximumMultiply<long long>, Update::multiply, Query::maximum, Kind::sum_bounds>();
	check<SumMaximumAffine<long long>, Update::affine, Query::maximum, Kind::sum_bounds>();
	check<MaximumCountAssign<long long>, Update::assign, Query::maximum, Kind::count>();
	check<MaximumCountAddAssign<long long>, Update::add_assign, Query::maximum, Kind::count>();
	check<MaximumCountMultiply<long long>, Update::multiply, Query::maximum, Kind::count>();
	check<MaximumCountAffine<long long>, Update::affine, Query::maximum, Kind::count>();
	check<MaximumIndexAssign<long long>, Update::assign, Query::maximum, Kind::index>();
	check<MaximumIndexAddAssign<long long>, Update::add_assign, Query::maximum, Kind::index>();
	check<MaximumIndexMultiply<long long>, Update::multiply, Query::maximum, Kind::index>();
	check<MaximumIndexAffine<long long>, Update::affine, Query::maximum, Kind::index>();
	check<MinimumMaximumAddAssign<long long>, Update::add_assign, Query::both, Kind::bounds>();
	check<SumMinimumMaximumAddAssign<long long>, Update::add_assign, Query::both, Kind::sum_bounds>();
	check<MinimumMaximumMultiply<long long>, Update::multiply, Query::both, Kind::bounds>();
	check<SumMinimumMaximumMultiply<long long>, Update::multiply, Query::both, Kind::sum_bounds>();
	check<SumMinimumMaximumAdd<long long>, Update::add, Query::both, Kind::sum_bounds>();
	check<SumMinimumMaximumAssign<long long>, Update::assign, Query::both, Kind::sum_bounds>();
	check<MinimumMaximumCountAssign<long long>, Update::assign, Query::both, Kind::count>();
	check<MinimumMaximumCountAddAssign<long long>, Update::add_assign, Query::both, Kind::count>();
	check<MinimumMaximumCountMultiply<long long>, Update::multiply, Query::both, Kind::count>();
	check<MinimumMaximumCountAffine<long long>, Update::affine, Query::both, Kind::count>();
	check<MinimumMaximumCountAdd<long long>, Update::add, Query::both, Kind::count>();
	check<MinimumMaximumIndexAssign<long long>, Update::assign, Query::both, Kind::index>();
	check<MinimumMaximumIndexAddAssign<long long>, Update::add_assign, Query::both, Kind::index>();
	check<MinimumMaximumIndexMultiply<long long>, Update::multiply, Query::both, Kind::index>();
	check<MinimumMaximumIndexAffine<long long>, Update::affine, Query::both, Kind::index>();
	check<MinimumMaximumIndexAdd<long long>, Update::add, Query::both, Kind::index>();
	check_boundaries();
	answer_library_checker_aplusb();
}
