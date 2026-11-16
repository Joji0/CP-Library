#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <limits>
#include <vector>
#include "mathematics/combinatorics/multinomial_coefficient.hpp"
#include "mathematics/combinatorics/surjection_count.hpp"
#include "mathematics/combinatorics/bounded_composition_count.hpp"
#include "mathematics/combinatorics/composite_modular_binomial_coefficient.hpp"
#include "mathematics/combinatorics/eulerian_number.hpp"
#include "mathematics/combinatorics/narayana_number.hpp"
#include "mathematics/combinatorics/non_modular_binomial_coefficient.hpp"
#include "mathematics/combinatorics/pascal_triangle.hpp"
#include "mathematics/combinatorics/q_binomial_coefficient.hpp"
#include "mathematics/modular_arithmetic/dynamic_modular_integer.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	using Modular = DynamicModularInteger<30>;
	Combinatorics<Modular> prime(0);
	const auto exact = pascal_triangle(66);
	for (int modulus = 1; modulus <= 81; ++modulus) {
		const CompositeModularBinomialCoefficient cached(modulus), no_cache(modulus, 0);
		const bool check_prime = NumberTheory::is_prime(modulus);
		if (check_prime) Modular::set_modulus(modulus);
		for (int n = 0; n <= 66; ++n) {
			for (int r = 0; r <= n; ++r) {
				const auto expected = exact[n][r] % modulus;
				assert(cached.binomial_coefficient(n, r) == expected);
				assert(no_cache.binomial_coefficient(n, r) == expected);
				if (check_prime) assert(prime.binomial_coefficient(n, r) == expected);
			}
		}
	}
	const auto maximum = std::numeric_limits<long long>::max();
	assert(non_modular_binomial_coefficient(maximum, 1) == maximum);
	assert(non_modular_binomial_coefficient(66, 33) == 7219428434016265740LL);
	assert(!try_non_modular_binomial_coefficient(67, 33));

	Modular::set_modulus(5);
	assert(prime.factorial(4) == 4);
	assert(prime.catalan(4) == 4);
	assert(prime.binomial_coefficient(15, 5) == 3);
	Modular::set_modulus(7);
	assert(prime.factorial(4) == 3);
	assert(prime.permutation_count(maximum - 1, 0) == 1);
	assert(prime.multiset_combination_count(maximum, 2) == 0);  // maximum is divisible by 7
	for (int modulus : {2, 3, 7, 11, 29}) {
		Modular::set_modulus(modulus);
		for (int base = 0; base < modulus; ++base) {
			const QBinomialCoefficient<Modular> cached(base, 25), no_cache(base);
			std::vector<Modular> row{1};
			for (int n = 1; n <= 25; ++n) {
				row.push_back(1);
				for (int r = n - 1; r > 0; --r) row[r] = row[r - 1] + Modular(base).power(r) * row[r];
				for (int r = 0; r <= n; ++r) {
					assert(cached.binomial_coefficient(n, r) == row[r]);
					assert(no_cache.binomial_coefficient(n, r) == row[r]);
				}
			}
		}
	}
	using Fixed = ModularInteger998244353;
	Combinatorics<Fixed> combinations;
	assert(multinomial_coefficient({-1, 2, 3, 5}, combinations) == 2520);
	assert(multinomial_coefficient({-1, 2, 3, 5}, CompositeModularBinomialCoefficient(18)) == 0);
	assert(multinomial_coefficient({0, 1, 2, 3}, CompositeModularBinomialCoefficient(14)) == 4);
	assert(surjection_count(4, 3, combinations) == 36);
	assert(surjection_count(0, 0, combinations) == 1);
	assert((eulerian_numbers<Fixed>(5) == std::vector<Fixed>{1, 26, 66, 26, 1}));
	assert((narayana_numbers<Fixed>(5) == std::vector<Fixed>{0, 1, 10, 20, 10, 1}));
	for (int parts = 0; parts <= 5; ++parts) {
		std::vector<Fixed> dp(21);
		dp[0] = 1;
		for (int round = 0; round < parts; ++round) {
			auto next = std::vector<Fixed>(21);
			for (int sum = 0; sum <= 20; ++sum)
				for (int value = 0; value <= 4 && value <= sum; ++value) next[sum] += dp[sum - value];
			dp.swap(next);
		}
		for (int sum = 0; sum <= 20; ++sum) assert(bounded_composition_count(sum, parts, 4, combinations) == dp[sum]);
	}
	answer_library_checker_aplusb();
}
