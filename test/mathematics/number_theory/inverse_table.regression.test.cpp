#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include "mathematics/number_theory/number_theory.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	constexpr long long modulus = 10000000019LL;
	const auto inverse = NumberTheory::inverse_table(1000, modulus);
	assert(inverse[3] == 3333333340LL);
	for (int value = 1; value <= 1000; ++value) {
		assert(__int128(value) * inverse[value] % modulus == 1);
	}
	answer_library_checker_aplusb();
}
