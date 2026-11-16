#include <cassert>
#include <limits>
#include <vector>
#include "mathematics/convolution/convolution.hpp"

int main() {
	using Modular = StaticModularInteger<129>;
	const std::vector<Modular> ones(61, 1);
	const auto product = convolution(ones, ones);
	assert(product.size() == 121);
	for (int degree = 0; degree <= 120; ++degree) {
		assert(product[degree] == (degree < 61 ? degree + 1 : 121 - degree));
	}
	const std::vector<Modular> empty;
	assert(convolution_for_any_modulus(empty, ones).empty());
	assert(convolution_large(std::vector<ModularInteger998244353>{}, {1}).empty());

	// Two ways to reach degree 1 must remain reachable in characteristic two.
	using Binary = StaticModularInteger<2>;
	const std::vector<Binary> binary{1, 1};
	assert((polynomial_power(binary, 2, 4) == std::vector<Binary>{1, 0, 1, 0}));
	assert((boolean_polynomial_power(binary, 2, 4) == std::vector<Binary>{1, 1, 1, 0}));
	assert((boolean_polynomial_power(std::vector<Binary>{0, 1},
					 std::numeric_limits<long long>::max(), 4) == std::vector<Binary>(4)));
	// Also exercise the transform path and verify that intermediate coefficients
	// are reset to reachability between multiplications.
	const std::vector<Binary> dense(80, 1);
	const auto reachable = boolean_polynomial_power(dense, 3, 240);
	assert(reachable.size() == 240);
	for (int i = 0; i < 240; ++i) assert(reachable[i] == Binary(i <= 237));
}
