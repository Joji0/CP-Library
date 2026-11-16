#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <limits>
#include <numeric>
#include <random>
#include <vector>
#include "mathematics/convolution/bitwise_convolution.hpp"
#include "mathematics/convolution/convolution.hpp"
#include "mathematics/convolution/dirichlet_convolution.hpp"
#include "mathematics/convolution/dirichlet_inverse.hpp"
#include "mathematics/convolution/greatest_common_divisor_convolution.hpp"
#include "mathematics/convolution/least_common_multiple_convolution.hpp"
#include "mathematics/set_function/set_power_series_exponential.hpp"
#include "mathematics/set_function/set_power_series_inverse.hpp"
#include "mathematics/set_function/set_power_series_logarithm.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	std::mt19937_64 random(159);
	const auto max_uint = std::numeric_limits<uint64_t>::max();
	for (uint32_t mod : {1U, 2U, 7U, 998244353U, 2147483647U, 4294967295U}) {
		BarrettReduction reducer(mod);
		for (int i = 0; i < 50; ++i) {
			for (auto n : {uint64_t(max_uint - i), static_cast<uint64_t>(random())}) {
				auto [quotient, remainder] = reducer.divmod(n);
				assert(quotient == n / mod && remainder == n % mod);
			}
		}
	}
	using Modular = DynamicModularInteger<20>;
	for (uint32_t mod : {1U, 9U, 998244353U, 1000000007U, 2147483647U, 17U}) {
		Modular::set_modulus(mod);
		std::vector<Modular> left(73), right(82), expected(154);
		for (auto& value : left) value = random();
		for (auto& value : right) value = random();
		for (int i = 0; i < int(left.size()); ++i)
			for (int j = 0; j < int(right.size()); ++j) expected[i + j] += left[i] * right[j];
		assert(convolution(left, right) == expected);
	}
	assert((signed_integer_convolution({-3, 4}, {2, -5}) == std::vector<long long>{-6, 23, -20}));
	assert((signed_integer_convolution({std::numeric_limits<long long>::min()}, {1}) ==
		std::vector<long long>{std::numeric_limits<long long>::min()}));

	using Field = ModularInteger998244353;
	for (int n = 1; n <= 30; ++n) {
		std::vector<Field> left(n + 1), right(n + 1), gcd(n + 1), lcm(n + 1);
		for (int i = 0; i <= n; ++i) {
			left[i] = random();
			right[i] = random();
		}
		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= n; ++j) {
				gcd[std::gcd(i, j)] += left[i] * right[j];
				int index = std::lcm(i, j);
				if (index <= n) lcm[index] += left[i] * right[j];
			}
		}
		assert(greatest_common_divisor_convolution(left, right) == gcd);
		assert(least_common_multiple_convolution(left, right) == lcm);
		if (left[1] == 0) left[1] = 1;
		auto identity = std::vector<Field>(n + 1, Field(0));
		identity[1] = 1;
		assert(dirichlet_convolution(left, dirichlet_inverse(left), n) == identity);
	}
	for (int bits = 0; bits <= 6; ++bits) {
		const int n = 1 << bits;
		std::vector<Field> left(n), right(n), sx(n), expected(n), combined(n);
		for (int i = 0; i < n; ++i) {
			left[i] = random();
			right[i] = random();
		}
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j) {
				sx[i ^ j] += left[i] * right[j];
				if (!(i & j)) expected[i | j] += left[i] * right[j];
			}
		assert(BitwiseConvolution::multiply_exclusive_or(left, right) == sx);
		assert(subset_convolution(left, right) == expected);
		left[0] = 0;
		assert(set_power_series_logarithm(set_power_series_exponential(left)) == left);
		left[0] = 42;
		combined[0] = 1;
		assert(subset_convolution(left, set_power_series_inverse(left)) == combined);
	}
	answer_library_checker_aplusb();
}
