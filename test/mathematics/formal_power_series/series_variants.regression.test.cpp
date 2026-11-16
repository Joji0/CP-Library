#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <limits>
#include <random>
#include <vector>
#include "mathematics/bernoulli_number.hpp"
#include "mathematics/formal_power_series/sparse_formal_power_series_exponential.hpp"
#include "mathematics/formal_power_series/sparse_formal_power_series_inverse.hpp"
#include "mathematics/lagrange_interpolation.hpp"
#include "mathematics/linear_recurrence/kitamasa.hpp"
#include "mathematics/linear_recurrence/linear_recurrence.hpp"
#include "mathematics/polynomial/polynomial_greatest_common_divisor.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	std::mt19937 random(375);
	using Modular = ModularInteger998244353;
	using Series = FormalPowerSeries<Modular>;
	for (int test = 0; test < 40; ++test) {
		Series dense(100);
		std::vector<std::pair<int, Modular>> sparse;
		for (int i = 1; i < 100; ++i) {
			const auto value = random();
			if (value % 9 == 0) {
				dense[i] = value;
				sparse.emplace_back(i, dense[i]);
			}
		}
		assert(dense.exponential() == sparse_formal_power_series_exponential(sparse, 100));
		dense[0] = random() % 100 + 1;
		sparse.emplace(sparse.begin(), 0, dense[0]);
		assert(dense.inverse() == sparse_formal_power_series_inverse(sparse, 100));

		int order = random() % 12 + 1;
		std::vector<Modular> initial(order), coefficients(order);
		for (int i = 0; i < order; ++i) {
			initial[i] = random();
			coefficients[i] = random();
		}
		auto terms = initial;
		for (int i = order; i < 103; ++i) {
			Modular value = 0;
			for (int j = 0; j < order; ++j) value += coefficients[j] * terms[i - j - 1];
			terms.push_back(value);
		}
		Modular prefix = 0;
		for (auto value : terms) prefix += value;
		assert(kitamasa(initial, coefficients, 102) == terms[102]);
		assert(linear_recurrence_prefix_sum(initial, coefficients, 102) == prefix);
	}
	Series common{3, 1, 1};
	assert(polynomial_greatest_common_divisor(common * Series{11, 2}, common * Series{27, 9}) == common);
	BernoulliNumber<Modular> bernoulli(10);
	for (int degree = 0; degree <= 10; ++degree) {
		Modular expected = 0;
		for (int i = -5; i <= 17; ++i) expected += Modular(i).power(degree);
		assert(bernoulli.power_sum(-5, 17, degree) == expected);
		assert(bernoulli.power_sum(8, 7, degree) == 0);
	}
	const long long low = std::numeric_limits<long long>::min();
	assert(LagrangeInterpolation<Modular>::evaluate({-1, 0, 1, 4}, low) == Modular(low).power(2));
	answer_library_checker_aplusb();
}
