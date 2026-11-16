#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <limits>
#include <random>
#include <set>
#include "mathematics/linear_algebra/exclusive_or_basis.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	std::mt19937 random(8177);
	for (int test = 0; test < 40; ++test) {
		ExclusiveOrBasis<unsigned, 7> left, right;
		std::set<unsigned> expected{0};
		for (int j = 0; j < 8; ++j) {
			const unsigned value = random() % 256;
			(j < 4 ? left : right).add(value);
			auto copy = expected;
			for (unsigned previous : copy) expected.insert(previous ^ value);
		}
		left.merge(right);
		assert(left.count_distinct() == expected.size());
		assert(left.maximum_exclusive_or() == *expected.rbegin());
		assert(!left.can_represent(256));
		const unsigned seed = random() % 512;
		unsigned minimum = 512, maximum = 0;
		unsigned rank = 1;
		for (auto value : expected) {
			assert(left.kth_smallest(rank++) == value);
			minimum = std::min(minimum, seed ^ value);
			maximum = std::max(maximum, seed ^ value);
		}
		assert(!left.kth_smallest(rank));
		assert(left.minimum_exclusive_or(seed) == minimum);
		assert(left.maximum_exclusive_or(seed) == maximum);
		for (unsigned i = 0; i < 256; ++i) assert(left.can_represent(i) == expected.contains(i));
	}
	ExclusiveOrBasis<> full;
	for (int bit = 0; bit < 64; ++bit) full.add(uint64_t(1) << bit);
	assert(full.kth_smallest(__uint128_t(1) << 64) == std::numeric_limits<uint64_t>::max());
	answer_library_checker_aplusb();
}
