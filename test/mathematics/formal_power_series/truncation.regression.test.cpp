#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <limits>
#include "mathematics/formal_power_series/formal_power_series.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	using Series = FormalPowerSeries<>;
	assert((Series{0}).power(0, 3) == (Series{1, 0, 0}));
	assert(Series{}.power(0, 3) == (Series{1, 0, 0}));
	assert((Series{0, 1}).power(2, 4) == (Series{0, 0, 1, 0}));
	assert((Series{1}).logarithm(0).empty());
	assert((Series{0}).exponential(0).empty());
	assert((Series{1}).inverse(0).empty());
	assert((Series{0, 1}).power(1, 0).empty());
	using Modular = ModularInteger998244353;
	const auto exponent = std::numeric_limits<unsigned long long>::max();
	assert((Series{2}).power(exponent, 1)[0] == Modular(2).power(exponent % (Modular::get_modulus() - 1)));
	answer_library_checker_aplusb();
}
