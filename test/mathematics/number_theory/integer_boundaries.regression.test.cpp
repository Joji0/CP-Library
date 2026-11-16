#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <limits>
#include <numeric>
#include <random>
#include "mathematics/number_theory/modular_tetration.hpp"
#include "mathematics/number_theory/multiplicative_order.hpp"
#include "mathematics/number_theory/segmented_sieve.hpp"
#include "mathematics/number_theory/sieve.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	const auto low = std::numeric_limits<long long>::min();
	const auto high = std::numeric_limits<long long>::max();
	for (long long a : {low, low + 1, -10LL, -1LL, 0LL, 1LL, 35LL, high}) {
		for (long long b : {low, low + 1, -5LL, -1LL, 0LL, 1LL, 28LL, high}) {
			long long x = 0, y = 0;
			auto magnitude = [](long long value) { return value < 0 ? 0ULL - value : static_cast<unsigned long long>(value); };
			auto expected = std::gcd(magnitude(a), magnitude(b));
			auto answer = NumberTheory::try_extended_greatest_common_divisor(a, b, x, y);
			assert(bool(answer) == (expected <= static_cast<unsigned long long>(high)));
			if (answer) assert(__int128(a) * x + __int128(b) * y == *answer && *answer == expected);
		}
	}
	assert(NumberTheory::modular_inverse(low, high) == high - 1);
	assert(!NumberTheory::modular_inverse(2, 4));
	assert(NumberTheory::floor_sum(high, 1, 0, 1) == high);
	assert(NumberTheory::floor_sum(0, 1, high, high) == 0);
	assert(!NumberTheory::try_floor_sum(high, 1, high, high));
	for (int n = 0; n <= 10; ++n) {
		for (int m = 1; m <= 8; ++m) {
			for (int a = 0; a < 17; ++a) {
				for (int b = 0; b < 17; ++b) {
					long long expected = 0;
					for (int i = 0; i < n; ++i) expected += (a * i + b) / m;
					assert(NumberTheory::floor_sum(n, m, a, b) == expected);
				}
			}
		}
	}
	using Pair = std::pair<long long, long long>;
	assert((NumberTheory::chinese_remainder_theorem({0, -1, 5}, {0, 6, 8}) == Pair(5, 24)));
	assert((NumberTheory::chinese_remainder_theorem({0, 1, 2}, {0, 4, 6}) == Pair(0, 0)));
	bool overflowed = false;
	try {
		NumberTheory::chinese_remainder_theorem({0, 0, 0}, {0, high, 2});
	} catch (const std::overflow_error&) { overflowed = true; }
	assert(overflowed);

	Sieve full(70'111);
	SegmentedSieve segmented(70'111);
	auto range = segmented.sieve_range(31, 70'111);
	for (int n = 31; n <= 70'111; ++n) assert(bool(range[n - 31]) == full.is_prime(n));
	assert(segmented.count_primes(70'111) == static_cast<long long>(full.primes().size()));
	for (int modulus = 2; modulus <= 90; ++modulus) {
		for (int value = 1; value < modulus; ++value) {
			if (std::gcd(value, modulus) != 1) {
				assert(!multiplicative_order(value, modulus));
				continue;
			}
			int result = value, length = 1;
			while (result != 1) {
				result = result * value % modulus;
				++length;
			}
			assert(multiplicative_order(value, modulus) == length);
		}
		for (int base = 0; base <= 4; ++base) {
			for (int height = 0; height <= 3; ++height) {
				uint64_t exponent = 1;
				for (int i = 1; i < height; ++i) {
					uint64_t next = 1;
					for (uint64_t j = 0; j < exponent; ++j) next *= base;
					exponent = next;
				}
				assert(modular_tetration(base, height, modulus) == (height == 0 ? 1 : NumberTheory::modular_power(base, exponent, modulus)));
			}
		}
	}
	answer_library_checker_aplusb();
}
