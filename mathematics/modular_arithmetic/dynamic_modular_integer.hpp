#pragma once
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <limits>
#include <optional>
#include <stdexcept>
#include <cassert>
#include "mathematics/modular_arithmetic/barrett_reduction.hpp"

template <int ID>
class DynamicModularInteger {
       private:
	uint32_t val;
	static BarrettReduction bt;
	static std::optional<uint32_t> modular_inverse(long long a) {
		long long b = bt.get_modulus(), x = 1, u = 0, t = 0, tmp = 0;
		while (b) {
			t = a / b;
			tmp = a - t * b;
			a = b;
			b = tmp;
			tmp = x - t * u;
			x = u;
			u = tmp;
		}
		if (a != 1) return std::nullopt;
		if (x < 0) x += bt.get_modulus();
		return (uint32_t)x;
	}

       public:
	DynamicModularInteger() : val(0) {}
	template <class U, std::enable_if_t<std::is_integral_v<U> && std::is_signed_v<U>, int> = 0>
	DynamicModularInteger(U v) {
		long long x = (long long)(v % (long long)bt.get_modulus());
		if (x < 0) x += bt.get_modulus();
		val = (uint32_t)x;
	}
	template <class U, std::enable_if_t<std::is_integral_v<U> && std::is_unsigned_v<U>, int> = 0>
	DynamicModularInteger(U v) : val((uint32_t)(v % bt.get_modulus())) {}
	uint32_t get() const { return val; }
	static uint32_t get_modulus() { return bt.get_modulus(); }
	// Existing Modular values belong to the old ring and must not be reused.
	static void set_modulus(uint32_t m) {
		if (m == 0 || m > std::numeric_limits<uint32_t>::max() / 2) throw std::invalid_argument("modulus outside supported interval");
		bt = BarrettReduction(m);
	}
	// Precondition: v < get_modulus(). Skips modular reduction.
	static DynamicModularInteger raw(uint32_t v) {
		DynamicModularInteger res;
		res.val = v;
		return res;
	}
	// Inversion requires a unit. Use try_inverse() when invertibility is unknown.
	DynamicModularInteger inverse() const {
		auto result = try_inverse();
		if (!result) std::abort();
		return *result;
	}
	std::optional<DynamicModularInteger> try_inverse() const {
		if (val == 0) return std::nullopt;
		const auto result = modular_inverse(val);
		if (!result) return std::nullopt;
		return raw(*result);
	}
	DynamicModularInteger& operator++() {
		if (++val == bt.get_modulus()) val = 0;
		return *this;
	}
	DynamicModularInteger operator++(int) {
		DynamicModularInteger res = *this;
		++*this;
		return res;
	}
	DynamicModularInteger& operator--() {
		if (val == 0) val = bt.get_modulus();
		--val;
		return *this;
	}
	DynamicModularInteger operator--(int) {
		DynamicModularInteger res = *this;
		--*this;
		return res;
	}
	DynamicModularInteger& operator+=(const DynamicModularInteger& other) {
		val += other.val;
		if (val >= bt.get_modulus()) val -= bt.get_modulus();
		return *this;
	}
	DynamicModularInteger& operator-=(const DynamicModularInteger& other) {
		if (val < other.val) val += bt.get_modulus();
		val -= other.val;
		return *this;
	}
	DynamicModularInteger& operator*=(const DynamicModularInteger& other) {
		val = bt.multiply(val, other.val);
		return *this;
	}
	DynamicModularInteger& operator/=(const DynamicModularInteger& other) {
		*this *= other.inverse();
		return *this;
	}
	friend DynamicModularInteger operator+(const DynamicModularInteger& lhs, const DynamicModularInteger& rhs) { return DynamicModularInteger(lhs) += rhs; }
	friend DynamicModularInteger operator-(const DynamicModularInteger& lhs, const DynamicModularInteger& rhs) { return DynamicModularInteger(lhs) -= rhs; }
	friend DynamicModularInteger operator*(const DynamicModularInteger& lhs, const DynamicModularInteger& rhs) { return DynamicModularInteger(lhs) *= rhs; }
	friend DynamicModularInteger operator/(const DynamicModularInteger& lhs, const DynamicModularInteger& rhs) { return DynamicModularInteger(lhs) /= rhs; }
	DynamicModularInteger operator+() const { return *this; }
	DynamicModularInteger operator-() const { return DynamicModularInteger() - *this; }
	friend bool operator==(const DynamicModularInteger& lhs, const DynamicModularInteger& rhs) { return lhs.val == rhs.val; }
	friend bool operator!=(const DynamicModularInteger& lhs, const DynamicModularInteger& rhs) { return lhs.val != rhs.val; }
	// Negative exponents use the multiplicative inverse.
	DynamicModularInteger power(long long exponent) const {
		const auto magnitude = static_cast<unsigned long long>(exponent);
		if (exponent < 0) return inverse().power(0ULL - magnitude);
		return power(magnitude);
	}
	template <typename Unsigned>
		requires(std::is_integral_v<Unsigned> && std::is_unsigned_v<Unsigned>)
	DynamicModularInteger power(Unsigned exponent) const {
		DynamicModularInteger base = *this, result = 1;
		while (exponent != 0) {
			if (exponent & 1) result *= base;
			exponent >>= 1;
			base *= base;
		}
		return result;
	}
	friend std::ostream& operator<<(std::ostream& os, const DynamicModularInteger& m) { return os << m.val; }
	friend std::istream& operator>>(std::istream& is, DynamicModularInteger& m) {
		long long v;
		if (is >> v) m = DynamicModularInteger(v);
		return is;
	}
};

template <int ID>
BarrettReduction DynamicModularInteger<ID>::bt = BarrettReduction(998244353);
