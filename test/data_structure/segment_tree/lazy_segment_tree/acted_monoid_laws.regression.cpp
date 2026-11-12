#include <cassert>
#include <initializer_list>
#include <limits>
#include "acted_monoid/flip_sum.hpp"
#include "acted_monoid/maximum_add.hpp"
#include "acted_monoid/maximum_affine.hpp"
#include "acted_monoid/maximum_assign.hpp"
#include "acted_monoid/maximum_count_add.hpp"
#include "acted_monoid/maximum_index_add.hpp"
#include "acted_monoid/minimum_add.hpp"
#include "acted_monoid/minimum_affine.hpp"
#include "acted_monoid/minimum_assign.hpp"
#include "acted_monoid/minimum_count_add.hpp"
#include "acted_monoid/minimum_index_add.hpp"
#include "acted_monoid/minimum_maximum_affine.hpp"
#include "acted_monoid/sum_add.hpp"
#include "acted_monoid/sum_add_assign.hpp"
#include "acted_monoid/sum_affine.hpp"
#include "acted_monoid/sum_assign.hpp"
#include "monoid/maximum.hpp"
#include "monoid/maximum_count.hpp"
#include "monoid/maximum_index.hpp"
#include "monoid/minimum.hpp"
#include "monoid/minimum_count.hpp"
#include "monoid/minimum_index.hpp"

namespace {
struct Equal {
	template <typename Value>
	bool operator()(const Value &left, const Value &right) const { return left == right; }
};

template <typename Monoid, typename Compare = Equal>
void check_laws(std::initializer_list<typename Monoid::Value> values,
		std::initializer_list<typename Monoid::Action> actions, Compare equal = {}) {
	const auto identity = Monoid::identity();
	const auto no_action = Monoid::identity_action();
	for (const auto &value : values) {
		assert(equal(Monoid::combine(identity, value), value));
		assert(equal(Monoid::combine(value, identity), value));
		assert(equal(Monoid::mapping(no_action, value), value));
		for (const auto &other : values) {
			const auto combined = Monoid::combine(value, other);
			for (const auto &last : values) {
				assert(equal(Monoid::combine(combined, last),
					     Monoid::combine(value, Monoid::combine(other, last))));
			}
			for (const auto &action : actions) {
				assert(equal(Monoid::mapping(action, combined),
					     Monoid::combine(Monoid::mapping(action, value), Monoid::mapping(action, other))));
			}
		}
		for (const auto &later : actions) {
			assert(equal(Monoid::mapping(later, identity), identity));
			for (const auto &earlier : actions) {
				assert(equal(Monoid::mapping(Monoid::composition(later, earlier), value),
					     Monoid::mapping(later, Monoid::mapping(earlier, value))));
			}
		}
	}
}

template <typename Add, typename Assign, typename Affine, typename Count, typename Index>
void check_extremum() {
	check_laws<Add>({Add::identity(), -4, 0, 3}, {-3, 0, 2});
	check_laws<Assign>({Assign::identity(), -4, 0, 3}, {Assign::identity_action(), -4, 0, 3});
	check_laws<Affine>({Affine::identity(), -4, 0, 3}, {{0, 3}, {1, 0}, {2, -4}});
	check_laws<Count>({Count::identity(), {-4, 1}, {3, 2}}, {-3, 0, 2},
			  [](const auto &left, const auto &right) { return left.val == right.val && left.cnt == right.cnt; });
	check_laws<Index>({Index::identity(), {-4, 1}, {3, 2}, {3, 3}}, {-3, 0, 2},
			  [](const auto &left, const auto &right) { return left.val == right.val && left.idx == right.idx; });
}
}  // namespace

int main() {
	check_extremum<MinimumAdd<int>, MinimumAssign<int>, MinimumAffine<int>, MinimumCountAdd<int>, MinimumIndexAdd<int>>();
	check_extremum<MaximumAdd<int>, MaximumAssign<int>, MaximumAffine<int>, MaximumCountAdd<int>, MaximumIndexAdd<int>>();
	using Both = MinimumMaximumAffine<int>;
	check_laws<Both>({Both::identity(), Both::Extrema{-4, 1}, Both::Extrema{0, 3}}, {{-2, 3}, {0, -5}, {1, 0}, {2, 4}});

	const auto equal_sum = [](const auto &left, const auto &right) { return left.val == right.val && left.cnt == right.cnt; };
	check_laws<SumAdd<int>>({{0, 0}, {-4, 1}, {7, 3}}, {-3, 0, 2}, equal_sum);
	check_laws<SumAssign<int>>({{0, 0}, {-4, 1}, {7, 3}}, {SumAssign<int>::NONE, -3, 0, 2}, equal_sum);
	check_laws<SumAffine<int>>({{0, 0}, {-4, 1}, {7, 3}}, {{-2, 3}, {0, -5}, {1, 0}, {2, 4}}, equal_sum);
	using AssignAdd = SumAddAssign<int>;
	check_laws<AssignAdd>({{0, 0}, {-4, 1}, {7, 3}}, {AssignAdd::identity_action(), AssignAdd::make_add(3), AssignAdd::make_set(-7), {5, -2}}, equal_sum);
	check_laws<FlipSum<int>>({{0, 0}, {0, 1}, {1, 2}, {3, 3}}, {false, true},
				 [](const auto &left, const auto &right) { return left.sum == right.sum && left.cnt == right.cnt; });

	constexpr int high = std::numeric_limits<int>::max();
	constexpr int low = std::numeric_limits<int>::lowest();
	assert(MinimumIndex<int>::combine(MinimumIndex<int>::identity(), {high, 1}).idx == 1);
	assert(MaximumIndex<int>::combine({low, 1}, MaximumIndex<int>::identity()).idx == 1);
	assert(MinimumCount<int>::combine(MinimumCount<int>::identity(), {high, 2}).count == 2);
	assert(MaximumCount<int>::combine(MaximumCount<int>::identity(), {low, 2}).count == 2);
	assert(Maximum<double>::combine(Maximum<double>::identity(), -1.5) == -1.5);
	const double infinity = std::numeric_limits<double>::infinity();
	assert(Minimum<double>::combine(Minimum<double>::identity(), infinity) == infinity);
	assert(Maximum<double>::combine(Maximum<double>::identity(), -infinity) == -infinity);
}
