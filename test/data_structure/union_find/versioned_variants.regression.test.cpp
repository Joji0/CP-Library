#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <array>
#include <cassert>
#include <random>
#include <vector>
#include "data_structure/union_find/component_add_union_find.hpp"
#include "data_structure/union_find/component_aggregate_union_find.hpp"
#include "data_structure/union_find/dual_union_find.hpp"
#include "data_structure/union_find/partially_persistent_union_find.hpp"
#include "data_structure/union_find/persistent_union_find.hpp"
#include "data_structure/union_find/persistent_weighted_union_find.hpp"
#include "data_structure/union_find/persistent_xor_union_find.hpp"
#include "data_structure/union_find/rollback_component_aggregate_union_find.hpp"
#include "data_structure/union_find/rollback_potential_union_find.hpp"
#include "data_structure/union_find/union_find.hpp"
#include "monoid/add.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
struct Permutation {
	using value_type = std::array<int, 3>;

	static value_type identity() { return {0, 1, 2}; }
	static value_type combine(const value_type &left, const value_type &right) {
		return {left[right[0]], left[right[1]], left[right[2]]};
	}
	static value_type inverse(const value_type &value) {
		value_type result{};
		for (int i = 0; i < 3; ++i) result[value[i]] = i;
		return result;
	}
};

void check_versions() {
	constexpr int size = 26;
	PersistentUnionFind persistent(size);
	PersistentWeightedUnionFind<> weighted(size);
	PersistentXorUnionFind<> xor_sets(size);
	PartiallyPersistentUnionFind partial(size);
	std::vector<UnionFind> branching{UnionFind(), UnionFind(size)};
	std::vector<UnionFind> chronological{UnionFind(), UnionFind(size)};
	std::mt19937 random(87243);
	for (int step = 0; step < 600; ++step) {
		const int first = 1 + random() % size, second = 1 + random() % size;
		const int base = 1 + random() % (branching.size() - 1);
		auto branch = branching[base];
		branch.merge(first, second);
		branching.push_back(std::move(branch));
		assert(persistent.merge(base, first, second) == int(branching.size()) - 1);
		const auto weighted_update = weighted.constrain(base, first, second, second - first);
		const auto xor_update = xor_sets.constrain(base, first, second, first ^ second);
		assert(weighted_update.version == persistent.latest_version() && xor_update.version == weighted_update.version);
		assert(weighted_update.status != PotentialMergeResult::contradiction && xor_update.status == weighted_update.status);

		auto next = chronological.back();
		next.merge(first, second);
		chronological.push_back(std::move(next));
		assert(partial.merge(first, second) == int(chronological.size()) - 1);

		for (int version : {1, base, persistent.latest_version()}) {
			assert(persistent.count(version) == branching[version].count());
			assert(partial.count(version) == chronological[version].count());
			assert(weighted.count(version) == branching[version].count());
			for (int vertex = 1; vertex <= size; ++vertex) {
				const bool connected = branching[version].same(first, vertex);
				assert(persistent.same(version, first, vertex) == connected);
				assert(weighted.same(version, first, vertex) == connected);
				assert(xor_sets.same(version, first, vertex) == connected);
				assert(weighted.try_difference(version, first, vertex).has_value() == connected);
				assert(weighted.size(version, vertex) == branching[version].size(vertex));
				if (connected) {
					assert(weighted.difference(version, first, vertex) == vertex - first);
					assert(xor_sets.difference(version, first, vertex) == static_cast<unsigned>(first ^ vertex));
				}
				assert(persistent.size(version, vertex) == branching[version].size(vertex));
				assert(partial.same(version, first, vertex) == chronological[version].same(first, vertex));
				assert(partial.size(version, vertex) == chronological[version].size(vertex));
			}
		}
	}
	const auto conflict = weighted.constrain(1, 1, 1, 1);
	assert(conflict.status == PotentialMergeResult::contradiction);
	assert(weighted.count(conflict.version) == size && weighted.difference(conflict.version, 1, 1) == 0);
}

void check_component_operations() {
	const std::vector<long long> weights{999, 5, -2, 7, 11};
	ComponentAggregateUnionFind<Add<long long>> sum(weights);
	RollbackComponentAggregateUnionFind<Add<long long>> rolling(weights);
	assert(sum.merge(1, 2) && rolling.merge(1, 2));
	const auto checkpoint = rolling.snapshot();
	sum.apply(2, 10);
	rolling.apply(2, 10);
	assert(sum.merge(1, 3) && rolling.merge(1, 3));
	assert(sum.product(3) == 20 && rolling.product(3) == 20);
	rolling.rollback(checkpoint);
	assert(rolling.product(1) == 3 && rolling.product(3) == 7 && rolling.count() == 3);
	rolling.undo();
	assert(rolling.product(1) == 5 && rolling.product(2) == -2);

	ComponentAddUnionFind<> dual(weights);
	dual.apply_component(2, 3);
	assert(dual.merge(2, 1));
	assert(dual.get(1) == 5 && dual.get(2) == 1);
	dual.apply_component(1, -4);
	assert(dual.merge(3, 2));
	assert(dual.get(1) == 1 && dual.get(2) == -3 && dual.get(3) == 7);
	dual.apply_component(3, 10);
	assert(dual.get(1) == 11 && dual.get(2) == 7 && dual.get(3) == 17);

	const Permutation::value_type a{1, 0, 2}, b{0, 2, 1}, c{2, 1, 0};
	const std::vector<Permutation::value_type> values{{}, a, b, c};
	DualUnionFind<Permutation> non_commutative(values);
	assert(non_commutative.merge(2, 3));
	non_commutative.apply_component(3, a);
	assert(non_commutative.merge(1, 2));
	assert(non_commutative.get(1) == a);
	assert(non_commutative.get(2) == Permutation::combine(a, b));
	assert(non_commutative.get(3) == Permutation::combine(a, c));
	non_commutative.apply_component(1, c);
	assert(non_commutative.get(3) == Permutation::combine(c, Permutation::combine(a, c)));

	RollbackPotentialUnionFind<Permutation> potential(3);
	assert(potential.merge(1, 2, a));
	const auto snapshot = potential.snapshot();
	assert(potential.merge(3, 2, b));
	assert(potential.difference(1, 3) == Permutation::combine(a, Permutation::inverse(b)));
	assert(!potential.merge(3, 1, c));
	potential.rollback(snapshot);
	assert(!potential.try_difference(1, 3) && potential.difference(1, 2) == a);
}
}  // namespace

int main() {
	check_versions();
	check_component_operations();
	answer_library_checker_aplusb();
}
