#pragma once
#include <cstdint>
#include "mathematics/number_theory/number_theory.hpp"

// Number of units modulo n. Convention: euler_totient(0) = 0.
inline uint64_t euler_totient(uint64_t n) {
	uint64_t result = n, previous = 0;
	for (uint64_t prime : NumberTheory::factorize(n)) {
		if (prime != previous) result -= result / prime;
		previous = prime;
	}
	return result;
}
