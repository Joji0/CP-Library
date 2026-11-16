#include <cassert>
#include <limits>
#include <numeric>
#include "mathematics/modular_arithmetic/dynamic_modular_integer.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	using Static = StaticModularInteger<12>;
	static_assert(!Static(6).try_inverse());
	static_assert(Static(5).try_inverse().value() == 5);
	using Dynamic = DynamicModularInteger<13>;
	for (int modulus = 1; modulus <= 150; ++modulus) {
		Dynamic::set_modulus(modulus);
		for (int value = 0; value < modulus; ++value) {
			const auto inverse = Dynamic(value).try_inverse();
			assert(inverse.has_value() == (value != 0 && std::gcd(value, modulus) == 1));
			if (inverse) {
				assert(Dynamic(value) * *inverse == 1);
				assert(Dynamic(value).power(-3) == inverse->power(3));
			}
		}
	}
	Dynamic::set_modulus(7);
	assert(Dynamic(2).power(std::numeric_limits<long long>::min()) == 2);
	assert(Dynamic(2).power(std::numeric_limits<unsigned long long>::max()) == 1);
}
