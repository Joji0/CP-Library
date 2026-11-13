#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <limits>
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	using Prime = StaticModularInteger<7>;
	using Composite = StaticModularInteger<12>;
	static_assert(Prime(2).power(-1) == 4);
	static_assert(Prime(2).power(-2) == 2);
	static_assert(Prime(2).power(std::numeric_limits<long long>::min()) == 2);
	static_assert(Prime(2).power(std::numeric_limits<unsigned long long>::max()) == 1);
	static_assert(Prime(0).power(0) == 1);
	static_assert(Composite(5).power(-3) == 5);
	static_assert(!Composite(6).try_inverse());
	assert(ModularInteger998244353(3).inverse() * 3 == 1);
	assert(!ModularInteger998244353(0).try_inverse());
	answer_library_checker_aplusb();
}
