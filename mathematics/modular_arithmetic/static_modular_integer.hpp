#pragma once
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <limits>
#include <numeric>
#include <optional>
#include <cassert>

template <uint32_t Mod>
class StaticModularInteger {
	static_assert(Mod > 0, "Modulus must be positive");
	static_assert(Mod <= std::numeric_limits<uint32_t>::max() / 2,
		      "Modulus * 2 must be <= uint32_t::max()");

       private:
	uint32_t val;
	static constexpr uint32_t inv1000000007[] = {
	    0, 1, 500000004, 333333336, 250000002, 400000003,
	    166666668, 142857144, 125000001, 111111112, 700000005};
	static constexpr uint32_t inv998244353[] = {
	    0, 1, 499122177, 332748118, 748683265, 598946612,
	    166374059, 855638017, 873463809, 443664157, 299473306};
	static constexpr uint32_t modular_inverse(long long a) {
		long long b = Mod, x = 1, u = 0, t = 0, tmp = 0;
		while (b) {
			t = a / b;
			tmp = a - t * b;
			a = b;
			b = tmp;
			tmp = x - t * u;
			x = u;
			u = tmp;
		}
		if (x < 0) x += Mod;
		return (uint32_t)x;
	}
	static constexpr uint32_t mod_reduce(long long v) {
		long long x = v % (long long)Mod;
		if (x < 0) x += Mod;
		return (uint32_t)x;
	}

       public:
	constexpr StaticModularInteger() : val(0) {}
	template <class U, std::enable_if_t<std::is_integral_v<U> && std::is_signed_v<U>, int> = 0>
	constexpr StaticModularInteger(U v) : val(mod_reduce((long long)v)) {}
	template <class U, std::enable_if_t<std::is_integral_v<U> && std::is_unsigned_v<U>, int> = 0>
	constexpr StaticModularInteger(U v) : val((uint32_t)(v % Mod)) {}
	constexpr uint32_t get() const { return val; }
	static constexpr uint32_t get_modulus() { return Mod; }
	static constexpr StaticModularInteger raw(uint32_t v) {
		StaticModularInteger res;
		res.val = v;
		return res;
	}
	constexpr StaticModularInteger inverse() const {
		if constexpr (Mod == 1000000007) {
			if (val <= 10) return raw(inv1000000007[val]);
		} else if constexpr (Mod == 998244353) {
			if (val <= 10) return raw(inv998244353[val]);
		}
		return raw(modular_inverse(val));
	}
	constexpr std::optional<StaticModularInteger> try_inverse() const {
		if (val == 0 || std::gcd(val, Mod) != 1) return std::nullopt;
		return inverse();
	}
	constexpr StaticModularInteger& operator++() {
		if (++val == Mod) val = 0;
		return *this;
	}
	constexpr StaticModularInteger operator++(int) {
		StaticModularInteger res = *this;
		++*this;
		return res;
	}
	constexpr StaticModularInteger& operator--() {
		if (val == 0) val = Mod;
		--val;
		return *this;
	}
	constexpr StaticModularInteger operator--(int) {
		StaticModularInteger res = *this;
		--*this;
		return res;
	}
	constexpr StaticModularInteger& operator+=(const StaticModularInteger& other) {
		val += other.val;
		if (val >= Mod) val -= Mod;
		return *this;
	}
	constexpr StaticModularInteger& operator-=(const StaticModularInteger& other) {
		if (val < other.val) val += Mod;
		val -= other.val;
		return *this;
	}
	constexpr StaticModularInteger& operator*=(const StaticModularInteger& other) {
		val = (uint32_t)((uint64_t)val * other.val % Mod);
		return *this;
	}
	constexpr StaticModularInteger& operator/=(const StaticModularInteger& other) {
		*this *= other.inverse();
		return *this;
	}
	friend constexpr StaticModularInteger operator+(const StaticModularInteger& lhs, const StaticModularInteger& rhs) { return StaticModularInteger(lhs) += rhs; }
	friend constexpr StaticModularInteger operator-(const StaticModularInteger& lhs, const StaticModularInteger& rhs) { return StaticModularInteger(lhs) -= rhs; }
	friend constexpr StaticModularInteger operator*(const StaticModularInteger& lhs, const StaticModularInteger& rhs) { return StaticModularInteger(lhs) *= rhs; }
	friend constexpr StaticModularInteger operator/(const StaticModularInteger& lhs, const StaticModularInteger& rhs) { return StaticModularInteger(lhs) /= rhs; }
	constexpr StaticModularInteger operator+() const { return *this; }
	constexpr StaticModularInteger operator-() const { return StaticModularInteger() - *this; }
	friend constexpr bool operator==(const StaticModularInteger& lhs, const StaticModularInteger& rhs) { return lhs.val == rhs.val; }
	friend constexpr bool operator!=(const StaticModularInteger& lhs, const StaticModularInteger& rhs) { return lhs.val != rhs.val; }
	constexpr StaticModularInteger power(long long a) const {
		StaticModularInteger v = *this, res = 1;
		while (a) {
			if (a & 1) res *= v;
			a >>= 1;
			v *= v;
		}
		return res;
	}
	friend std::ostream& operator<<(std::ostream& os, const StaticModularInteger& m) { return os << m.val; }
	friend std::istream& operator>>(std::istream& is, StaticModularInteger& m) {
		long long v;
		is >> v;
		m = StaticModularInteger(v);
		return is;
	}
};

using ModularInteger1000000007 = StaticModularInteger<1000000007>;
using ModularInteger998244353 = StaticModularInteger<998244353>;
