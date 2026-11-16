#include <cassert>
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "mathematics/combinatorics.hpp"

int main() {
	using Modular = StaticModularInteger<5>;
	Combinatorics<Modular> combinations(3);
	assert(combinations.binomial_coefficient(4, 0) == 1);
	assert(combinations.binomial_coefficient(4, 2) == 1);
	assert(combinations.inverse(4) == 4);
	assert(combinations.factorial(5) == 0);
	assert(combinations.binomial_coefficient(6, 1) == 1);
	assert(combinations.binomial_coefficient(6, 2) == 0);
	assert(combinations.binomial_coefficient(4, 1) == 4);
	const Combinatorics<Modular> with_large_hint(40);
	assert(with_large_hint.binomial_coefficient(4, 0) == 1);
}
