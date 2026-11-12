#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <limits>
#include <random>
#include <vector>
#include "data_structure/union_find/equality_constraint_union_find.hpp"
#include "data_structure/union_find/parity_union_find.hpp"
#include "data_structure/union_find/rollback_parity_union_find.hpp"
#include "data_structure/union_find/rollback_weighted_constraint_union_find.hpp"
#include "data_structure/union_find/rollback_weighted_union_find.hpp"
#include "data_structure/union_find/rollback_xor_union_find.hpp"
#include "data_structure/union_find/union_find.hpp"
#include "data_structure/union_find/weighted_constraint_union_find.hpp"
#include "data_structure/union_find/weighted_union_find.hpp"
#include "data_structure/union_find/xor_union_find.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
void check_potentials() {
	constexpr long long low = std::numeric_limits<long long>::lowest();
	WeightedUnionFind<> extremes(3);
	assert(extremes.merge(1, 2, low));
	assert(extremes.merge(1, 2, low));
	assert(!extremes.merge(2, 1, 0));
	assert(extremes.difference(2, 2) == 0);
	assert(!extremes.try_difference(2, 3));
	assert(extremes.merge(1, 3, low));
	assert(extremes.difference(2, 3) == 0);

	std::mt19937 random(9073);
	constexpr int size = 30;
	std::vector<long long> additive(size + 1);
	std::vector<unsigned long long> bitmask(size + 1);
	for (int vertex = 1; vertex <= size; ++vertex) {
		additive[vertex] = int(random() % 501) - 250;
		bitmask[vertex] = (static_cast<unsigned long long>(random()) << 32) | random();
	}
	RollbackWeightedUnionFind<> weighted(size);
	RollbackXorUnionFind<> xor_rollback(size);
	XorUnionFind<> xor_online(size);
	UnionFind expected(size);
	std::vector<UnionFind> states{expected};
	for (int step = 0; step < 1600; ++step) {
		const int first = 1 + random() % size, second = 1 + random() % size;
		const long long difference = additive[second] - additive[first];
		const auto parity = bitmask[first] ^ bitmask[second];
		assert(xor_online.merge(first, second, parity));
		assert(xor_online.try_difference(first, second) == parity);
		assert(!xor_online.merge(first, second, parity ^ 1));
		if (random() % 5 == 0) {
			const std::size_t target = random() % states.size();
			weighted.rollback(target);
			xor_rollback.rollback(target);
			expected = states[target];
			states.resize(target + 1);
		} else {
			const bool joined = expected.merge(first, second);
			assert(weighted.merge(first, second, difference) && xor_rollback.merge(first, second, parity));
			assert(!weighted.merge(first, second, difference + 1));
			assert(!xor_rollback.merge(first, second, parity ^ 1));
			if (joined) states.push_back(expected);
		}
		assert(weighted.snapshot() + 1 == states.size() && xor_rollback.snapshot() == weighted.snapshot());
		for (int vertex = 1; vertex <= size; ++vertex) {
			const bool connected = expected.same(first, vertex);
			const auto actual_difference = weighted.try_difference(first, vertex);
			const auto actual_xor = xor_rollback.try_difference(first, vertex);
			assert(actual_difference.has_value() == connected && actual_xor.has_value() == connected);
			if (connected) {
				assert(*actual_difference == additive[vertex] - additive[first]);
				assert(*actual_xor == (bitmask[vertex] ^ bitmask[first]));
			}
		}
	}
}

void check_constraint_policies() {
	ParityUnionFind parity(4);
	assert(parity.constrain_same(1, 2));
	assert(parity.constrain_opposite(2, 3));
	assert(parity.try_same_color(1, 3) == false);
	assert(!parity.constrain_same(1, 3));
	assert(!parity.try_same_color(1, 4));
	RollbackParityUnionFind rolling(3);
	assert(rolling.constrain_opposite(1, 2));
	const auto previous = rolling.snapshot();
	assert(rolling.constrain_opposite(2, 3));
	assert(rolling.try_same_color(1, 3) == true);
	rolling.rollback(previous);
	assert(!rolling.try_same_color(1, 3));

	EqualityConstraintUnionFind equality(6);
	assert(equality.constrain_different(1, 2));
	assert(equality.constrain_different(2, 3));
	assert(equality.relation(1, 3) == EqualityRelation::unknown);
	assert(equality.constrain_equal(1, 3));
	assert(equality.relation(1, 3) == EqualityRelation::equal);
	assert(equality.relation(3, 2) == EqualityRelation::different);
	assert(!equality.constrain_equal(2, 3));
	assert(equality.constrain_different(4, 5));
	assert(equality.constrain_equal(4, 6));
	assert(equality.constrain_equal(1, 4));
	assert(equality.relation(6, 5) == EqualityRelation::different);
	assert(equality.relation(6, 2) == EqualityRelation::different);
	assert(!equality.constrain_different(1, 6));

	WeightedConstraintUnionFind<> tracked(4);
	assert(tracked.add_constraint(1, 2, 3));
	assert(!tracked.add_constraint(1, 2, 4));
	assert(!tracked.is_consistent(1) && tracked.is_consistent(3));
	assert(!tracked.try_difference(1, 2));
	assert(!tracked.add_constraint(2, 3, 4));
	assert(!tracked.is_consistent(3) && tracked.is_consistent(4));

	RollbackWeightedConstraintUnionFind<> reversible(3);
	assert(reversible.add_constraint(1, 2, 3));
	const auto consistent = reversible.snapshot();
	assert(!reversible.add_constraint(1, 2, 4));
	assert(!reversible.add_constraint(2, 3, 4));
	assert(reversible.count_inconsistent() == 1);
	reversible.rollback(consistent);
	assert(reversible.is_consistent() && reversible.count() == 2 && reversible.try_difference(1, 2) == 3);
}
}  // namespace

int main() {
	check_potentials();
	check_constraint_policies();
	answer_library_checker_aplusb();
}
