#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <limits>
#include "acted_monoid/sum_add.hpp"
#include "acted_monoid/sum_affine.hpp"
#include "acted_monoid/sum_assign.hpp"
#include "monoid/add.hpp"
#include "monoid/affine.hpp"
#include "monoid/greatest_common_divisor.hpp"
#include "monoid/least_common_multiple.hpp"
#include "monoid/maximum_subarray_sum.hpp"
#include "monoid/multiply.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	constexpr long long low = std::numeric_limits<long long>::lowest();
	constexpr long long high = std::numeric_limits<long long>::max();
	constexpr auto unsigned_high = std::numeric_limits<unsigned long long>::max();
	assert(Add<long long>::combine(low, high) == -1);
	assert(Add<long long>::remove_prefix(-1, high) == low);
	assert(Multiply<long long>::combine(low, 1) == low);
	assert(Multiply<long long>::remove_prefix(low, 1) == low);
	assert(Add<unsigned long long>::combine(unsigned_high - 1, 1) == unsigned_high);
	assert(GreatestCommonDivisor<long long>::combine(low, 2) == 2);
	assert(GreatestCommonDivisor<unsigned long long>::combine(1ULL << 63, 0) == (1ULL << 63));
	assert(LeastCommonMultiple<long long>::combine(-21, 6) == 42);
	assert(LeastCommonMultiple<long long>::combine(low, 0) == 0);
	assert(LeastCommonMultiple<unsigned long long>::combine(unsigned_high, 1) == unsigned_high);
	assert(Affine<long long>::combine({2, -high}, {1, high}).b == high);

	const auto wide_sum = SumAffine<long long>::mapping({2, -high}, {high, 2});
	assert(wide_sum.val == 0 && wide_sum.cnt == 2);
	assert(SumAdd<long long>::mapping(high, {low, 2}).val == high - 1);
	assert(SumAdd<signed char>::mapping(0, {2, 1000}).val == 2);
	assert(SumAssign<signed char>::mapping(0, {2, 1000}).val == 0);
	assert(Add<__int128>::combine(__int128(high), 1) == __int128(high) + 1);

	using Subarray = MaximumSubarraySum<long long>;
	const long long old_sentinel = -(high / 4);
	const auto first = Subarray::from_val(old_sentinel);
	const auto second = Subarray::from_val(-1);
	const auto combined = Subarray::combine(first, second);
	assert(!combined.empty && combined.sum == old_sentinel - 1 && combined.ans == -1);
	assert(Subarray::combine(Subarray::identity(), Subarray::from_val(low)).ans == low);
	assert(Subarray::identity().empty);
	answer_library_checker_aplusb();
}
