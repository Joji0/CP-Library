#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <vector>
#include "acted_monoid/binary_inversions_flip.hpp"
#include "acted_monoid/maximum_lower_bound.hpp"
#include "acted_monoid/maximum_upper_bound.hpp"
#include "acted_monoid/minimum_lower_bound.hpp"
#include "acted_monoid/minimum_upper_bound.hpp"
#include "acted_monoid/power_sums_assign.hpp"
#include "acted_monoid/minimum_maximum_add.hpp"
#include "acted_monoid/minimum_maximum_assign.hpp"
#include "acted_monoid/sum_maximum_add.hpp"
#include "acted_monoid/sum_maximum_assign.hpp"
#include "acted_monoid/sum_minimum_add.hpp"
#include "acted_monoid/sum_minimum_assign.hpp"
#include "acted_monoid/sum_minimum_maximum_affine.hpp"
#include "acted_monoid/sum_multiply.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/add.hpp"
#include "monoid/affine.hpp"
#include "monoid/array_monoid.hpp"
#include "monoid/assignment.hpp"
#include "monoid/bidirectional_monoid.hpp"
#include "monoid/bitwise_and.hpp"
#include "monoid/bitwise_function.hpp"
#include "monoid/bitwise_or.hpp"
#include "monoid/dihedral.hpp"
#include "monoid/exclusive_or.hpp"
#include "monoid/maximum_plus_matrix.hpp"
#include "monoid/minimum_plus_matrix.hpp"
#include "monoid/polynomial_hash.hpp"
#include "monoid/reverse_monoid.hpp"
#include "monoid/sum_minimum_maximum.hpp"
#include "monoid/trivial_monoid.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
template <typename ActedMonoid>
void check_clamp(int expected) {
	LazySegmentTree<ActedMonoid> tree(std::vector<int>{999, -5, 4});
	tree.update(1, 2, -2);
	tree.update(1, 2, 2);
	assert(tree.query(1, 2) == expected && !tree.query(1, 0));
}

template <typename Monoid, int Expected>
void check_matrix() {
	typename Monoid::value_type first{}, second{};
	first[1][1] = -3;
	first[1][2] = 2;
	second[1][2] = 3;
	second[2][2] = 5;
	const auto result = Monoid::combine(first, second);
	assert(result[1][2] == Expected && !result[2][2]);
	assert(Monoid::combine(Monoid::identity(), result) == result);
	assert(Monoid::combine(result, Monoid::identity()) == result);
}
}  // namespace

int main() {
	using Function = Affine<int>;
	using Directions = BidirectionalMonoid<Function>;
	const Function::Node first{2, 3}, second{3, 5};
	const auto both = Directions::combine(Directions::make_leaf(first), Directions::make_leaf(second));
	assert(both.forward == Function::combine(first, second));
	assert(both.backward == ReverseMonoid<Function>::combine(first, second) && both.forward != both.backward);
	using Array = ArrayMonoid<Add<int>, 2>;
	assert(Array::combine({0, 1, 2}, {0, 3, 4}) == (Array::value_type{0, 4, 6}));
	assert(Assignment<int>::combine(1, 3) == 3);
	assert(TrivialMonoid::combine({}, {}) == TrivialMonoid::identity());
	using Aggregate = SumMinimumMaximum<int>;
	const auto sum_bounds = Aggregate::combine(Aggregate::make_leaf(-3), Aggregate::make_leaf(6));
	assert(sum_bounds.first == 3 && sum_bounds.second->minimum == -3 && sum_bounds.second->maximum == 6);
	check_matrix<MinimumPlusMatrix<int, 2>, 0>();
	check_matrix<MaximumPlusMatrix<int, 2>, 7>();

	using Symmetry = Dihedral<7>;
	const auto reflection = Symmetry::make_reflection(-3);
	const auto rotation = Symmetry::make_rotation(15);
	assert(Symmetry::combine(reflection, reflection) == Symmetry::identity());
	for (int position = 0; position < 7; ++position) {
		assert(Symmetry::evaluate(Symmetry::combine(rotation, reflection), position) ==
		       Symmetry::evaluate(rotation, Symmetry::evaluate(reflection, position)));
	}

	const std::vector<unsigned> bit_values{0, 0b1010, 0b1100, 0b0011};
	SegmentTree<BitwiseAnd<unsigned>> bitwise_and(bit_values);
	SegmentTree<BitwiseOr<unsigned>> bitwise_or(bit_values);
	SegmentTree<ExclusiveOr<unsigned>> exclusive_or(bit_values);
	assert(bitwise_and.query(1, 2) == 0b1000 && bitwise_and.query(2, 1) == ~0U);
	assert(bitwise_or.query(1, 2) == 0b1110 && bitwise_or.query(2, 1) == 0U);
	assert(exclusive_or.query(1, 2) == 0b0110 && exclusive_or.query(2, 1) == 0U);
	assert(ExclusiveOr<unsigned>::remove_prefix(exclusive_or.query(1, 3), exclusive_or.query(1, 2)) == bit_values[3]);

	using Bits = BitwiseFunction<unsigned>;
	const auto apply_bits = Bits::combine(Bits::make_exclusive_or(3), Bits::combine(Bits::make_or(8), Bits::make_and(15)));
	assert(Bits::evaluate(apply_bits, 21) == (((21U & 15U) | 8U) ^ 3U));
	using Hash = PolynomialHash<>;
	const auto hash = Hash::combine(Hash::make_leaf(2), Hash::make_leaf(7));
	assert(hash.hash == ModularInteger998244353(2) * 911382323 + 7 && hash.length == 2);

	check_clamp<MinimumLowerBound<int>>(2);
	check_clamp<MaximumLowerBound<int>>(4);
	check_clamp<MinimumUpperBound<int>>(-5);
	check_clamp<MaximumUpperBound<int>>(-2);
	LazySegmentTree<SumMultiply<int>> products(std::vector<int>{999, 1, 2, -4});
	products.update(1, 3, -2);
	assert(products.all_product() == 2);
	LazySegmentTree<PowerSumsAssign<int, 3>> powers(3);
	powers.update(1, 2, -2);
	powers.update(3, 3);
	const auto moments = powers.all_product();
	assert(moments.count == 3 && moments.powers[1] == -1 && moments.powers[2] == 17 && moments.powers[3] == 11);
	LazySegmentTree<SumMinimumMaximumAffine<int>> affine(std::vector<int>{999, -1, 2, 4});
	affine.update(1, 3, {-2, 1});
	const auto result = affine.all_product();
	assert(result.sum.val == -7 && result.extrema->minimum == -7 && result.extrema->maximum == 3);

	LazySegmentTree<MinimumMaximumAdd<int>> bounds_add(std::vector<int>{999, -5, 1, 2});
	bounds_add.update(1, 2, 7);
	assert(bounds_add.all_product()->minimum == 2 && bounds_add.all_product()->maximum == 8);
	LazySegmentTree<MinimumMaximumAssign<int>> bounds_assign(3);
	bounds_assign.update(2, 3, -8);
	assert(bounds_assign.all_product()->minimum == -8 && bounds_assign.all_product()->maximum == 0);

	const std::vector<int> initial{999, -5, 1, 3};
	LazySegmentTree<SumMaximumAdd<int>> max_add(initial);
	LazySegmentTree<SumMinimumAdd<int>> min_add(initial);
	LazySegmentTree<SumMaximumAssign<int>> max_assign(initial);
	LazySegmentTree<SumMinimumAssign<int>> min_assign(initial);
	max_add.update(1, 2, 2);
	min_add.update(1, 2, 2);
	max_assign.update(1, 2, 2);
	min_assign.update(1, 2, 2);
	assert(max_add.all_product().first.val == 3 && max_add.all_product().second == 3);
	assert(min_add.all_product().first.val == 3 && min_add.all_product().second == -3);
	assert(max_assign.all_product().first.val == 7 && max_assign.all_product().second == 3);
	assert(min_assign.all_product().first.val == 7 && min_assign.all_product().second == 2);

	std::vector<int> bits{999, 1, 0, 1, 1, 0, 0};
	LazySegmentTree<BinaryInversionsFlip> inversions(bits);
	for (int left = 1; left <= 6; ++left) {
		for (int right = left; right <= 6; ++right) {
			inversions.update(left, right, true);
			for (int i = left; i <= right; ++i) bits[i] ^= 1;
			long long expected = 0;
			for (int i = 1; i <= 6; ++i) {
				for (int j = i + 1; j <= 6; ++j) expected += bits[i] > bits[j];
			}
			assert(inversions.all_product().inversions == expected);
		}
	}
	answer_library_checker_aplusb();
}
