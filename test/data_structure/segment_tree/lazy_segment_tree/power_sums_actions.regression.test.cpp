#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <array>
#include <cassert>
#include <random>
#include <vector>
#include "acted_monoid/power_sums_add.hpp"
#include "acted_monoid/power_sums_affine.hpp"
#include "acted_monoid/power_sums_multiply.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
enum class Update { add,
		    multiply,
		    affine };

template <typename Monoid, typename Number, std::size_t Degree, Update Operation>
void check() {
	constexpr int size = 23;
	std::mt19937 random(6378);
	std::vector<Number> values(size + 1);
	for (int i = 1; i <= size; ++i) values[i] = Number(int(random() % 11) - 5);
	LazySegmentTree<Monoid> tree(values);
	assert(tree.query(2, 1) == Monoid::identity());
	for (int iteration = 0; iteration < 380; ++iteration) {
		const int left = iteration % 7 == 0 ? 1 : 1 + random() % size;
		const int right = iteration % 7 == 0 ? size : left + random() % (size - left + 1);
		const Number offset = Operation == Update::multiply ? Number(0) : Number(int(random() % 11) - 5);
		const Number factor = Operation == Update::add ? Number(1) : Number(int(random() % 3) - 1);
		if constexpr (Operation == Update::add) tree.update(left, right, offset);
		if constexpr (Operation == Update::multiply) tree.update(left, right, factor);
		if constexpr (Operation == Update::affine) tree.update(left, right, {factor, offset});
		for (int i = left; i <= right; ++i) values[i] = factor * values[i] + offset;

		const int begin = iteration % 3 == 0 ? 1 : 1 + random() % size;
		const int end = iteration % 3 == 0 ? size : begin + random() % (size - begin + 1);
		std::array<Number, Degree + 1> expected{};
		for (int i = begin; i <= end; ++i) {
			Number power(1);
			for (std::size_t degree = 1; degree <= Degree; ++degree) {
				power *= values[i];
				expected[degree] += power;
			}
		}
		const auto actual = tree.query(begin, end);
		assert(actual.count == end - begin + 1 && actual.powers == expected);
	}
}

void check_wide_intermediates() {
	using Moments = PowerSumsAffine<long long, 2>;
	const long long shift = 1LL << 31;
	// Use one leaf so the initial squared sum is representable. The transformed
	// result also fits even though a term in the binomial expansion is too wide.
	LazySegmentTree<Moments> single(std::vector<long long>{0, -shift});
	single.update(1, 1, {2, 2 * shift});
	assert(single.all_product().powers[1] == 0 && single.all_product().powers[2] == 0);
}
}  // namespace

int main() {
	using Modular = ModularInteger998244353;
	check<PowerSumsAdd<long long, 5>, long long, 5, Update::add>();
	check<PowerSumsMultiply<long long, 6>, long long, 6, Update::multiply>();
	check<PowerSumsAffine<long long, 4>, long long, 4, Update::affine>();
	check<PowerSumsAffine<Modular, 7>, Modular, 7, Update::affine>();
	check<PowerSumsAdd<long long, 0>, long long, 0, Update::add>();
	check_wide_intermediates();
	answer_library_checker_aplusb();
}
