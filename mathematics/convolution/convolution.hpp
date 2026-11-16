#pragma once
#include <vector>
#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <cassert>
#include <limits>
#include <stdexcept>
#include <utility>
#include "mathematics/modular_arithmetic/dynamic_modular_integer.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

constexpr uint64_t primitive_root_for_convolution(uint64_t p) {
	if (p == 2) return 1;
	if (p == 998244353) return 3;
	if (p == 469762049) return 3;
	if (p == 1811939329) return 11;
	if (p == 2013265921) return 11;
	for (uint64_t g = 2; g < p; ++g) {
		uint64_t base = g, exp = (p - 1) >> 1, res = 1;
		while (exp > 0) {
			if (exp & 1) res = (res * base) % p;
			base = (base * base) % p;
			exp >>= 1;
		}
		if (res != 1) return g;
	}
	return -1;
}

namespace convolution_detail {

constexpr bool is_prime_modulus(uint32_t modulus) {
	if (modulus % 2 == 0) return modulus == 2;
	if (modulus < 2) return false;
	for (uint32_t divisor = 3; uint64_t(divisor) * divisor <= modulus; divisor += 2) {
		if (modulus % divisor == 0) return false;
	}
	return true;
}

constexpr int msb(uint32_t x) { return x == 0 ? -1 : 31 - __builtin_clz(x); }
constexpr int ceil_log2(uint32_t x) { return x <= 1 ? 0 : 32 - __builtin_clz(x - 1); }

template <class T>
class NthRoot {
       private:
	static constexpr unsigned int lg = msb((T::get_modulus() - 1) & (1 - T::get_modulus()));
	T root[lg + 1];
	T inv_root[lg + 1];
	T rate[lg + 1];
	T inv_rate[lg + 1];

       public:
	constexpr NthRoot() : root{}, inv_root{}, rate{}, inv_rate{} {
		root[lg] = T(primitive_root_for_convolution(T::get_modulus())).power((T::get_modulus() - 1) >> lg);
		inv_root[lg] = root[lg].inverse();
		for (int i = lg - 1; i >= 0; i--) {
			root[i] = root[i + 1] * root[i + 1];
			inv_root[i] = inv_root[i + 1] * inv_root[i + 1];
		}
		T r = 1;
		for (int i = 2; i <= (int)lg; i++) {
			rate[i - 2] = r * root[i];
			r = r * inv_root[i];
		}
		r = 1;
		for (int i = 2; i <= (int)lg; i++) {
			inv_rate[i - 2] = r * inv_root[i];
			r = r * root[i];
		}
	}
	static constexpr unsigned int get_lg() { return lg; }
	constexpr T get(int n) const { return root[n]; }
	constexpr T inverse(int n) const { return inv_root[n]; }
	constexpr T get_rate(int n) const { return rate[n]; }
	constexpr T get_inv_rate(int n) const { return inv_rate[n]; }
};

template <class T>
void number_theoretic_transform(std::vector<T>& a) {
	static constexpr NthRoot<T> nth_root;
	int n = (int)a.size();
	for (int i = n >> 1; i > 0; i >>= 1) {
		T z = T::raw(1);
		for (int j = 0; j < n; j += (i << 1)) {
			for (int k = 0; k < i; k++) {
				const T x = a[j + k];
				const T y = a[j + i + k] * z;
				a[j + k] = x + y;
				a[j + i + k] = x - y;
			}
			z *= nth_root.get_rate(__builtin_popcount(j & ~(j + (i << 1))));
		}
	}
}

template <class T>
void inverse_number_theoretic_transform(std::vector<T>& a) {
	static constexpr NthRoot<T> nth_root;
	int n = (int)a.size();
	for (int i = 1; i < n; i <<= 1) {
		T z = T::raw(1);
		for (int j = 0; j < n; j += (i << 1)) {
			for (int k = 0; k < i; k++) {
				const T x = a[j + k];
				const T y = a[j + i + k];
				a[j + k] = x + y;
				a[j + i + k] = (x - y) * z;
			}
			z *= nth_root.get_inv_rate(__builtin_popcount(j & ~(j + (i << 1))));
		}
	}
	T inv_n = T(1) / T(n);
	for (auto& x : a) x *= inv_n;
}

template <class T>
std::vector<T> convolution_naive(const std::vector<T>& a, const std::vector<T>& b) {
	int n = (int)a.size(), m = (int)b.size();
	if (n == 0 || m == 0) return {};
	std::vector<T> c(n + m - 1);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) c[i + j] += a[i] * b[j];
	}
	return c;
}

template <class T>
std::vector<T> convolution_square(std::vector<T> a) {
	int n = (int)a.size() * 2 - 1;
	if (n <= 1) return convolution_naive(a, a);
	int lg = msb(n - 1) + 1;
	if (n - (1 << (lg - 1)) <= 5) {
		--lg;
		int m = (int)a.size() - (1 << (lg - 1));
		std::vector<T> a1(a.begin(), a.begin() + m);
		std::vector<T> a2(a.begin() + m, a.end());
		std::vector<T> c(n);
		std::vector<T> c1 = convolution_naive(a1, a1);
		std::vector<T> c2 = convolution_naive(a1, a2);
		std::vector<T> c3 = convolution_square(a2);
		for (int i = 0; i < (int)c1.size(); i++) c[i] += c1[i];
		for (int i = 0; i < (int)c2.size(); i++) c[i + m] += c2[i] * 2;
		for (int i = 0; i < (int)c3.size(); i++) c[i + m * 2] += c3[i];
		return c;
	}
	int m = 1 << lg;
	a.resize(m);
	number_theoretic_transform(a);
	for (int i = 0; i < m; i++) a[i] *= a[i];
	inverse_number_theoretic_transform(a);
	a.resize(n);
	return a;
}

template <class T>
std::vector<T> convolution(std::vector<T> a, std::vector<T> b) {
	int n = (int)a.size() + (int)b.size() - 1;
	if (n <= 1) return convolution_naive(a, b);
	int lg = ceil_log2(n);
	int m = 1 << lg;
	if (n - (1 << (lg - 1)) <= 5) {
		--lg;
		if (a.size() < b.size()) std::swap(a, b);
		int half_m = n - (1 << lg);
		std::vector<T> a1(a.begin(), a.begin() + half_m);
		std::vector<T> a2(a.begin() + half_m, a.end());
		std::vector<T> c(n);
		std::vector<T> c1 = convolution_naive(a1, b);
		std::vector<T> c2 = convolution(a2, b);
		for (int i = 0; i < (int)c1.size(); i++) c[i] += c1[i];
		for (int i = 0; i < (int)c2.size(); i++) c[i + half_m] += c2[i];
		return c;
	}
	a.resize(m);
	b.resize(m);
	number_theoretic_transform(a);
	number_theoretic_transform(b);
	for (int i = 0; i < m; i++) a[i] *= b[i];
	inverse_number_theoretic_transform(a);
	a.resize(n);
	return a;
}

}  // namespace convolution_detail

template <uint32_t p>
struct is_ntt_friendly : std::false_type {};
template <>
struct is_ntt_friendly<998244353> : std::true_type {};
template <>
struct is_ntt_friendly<469762049> : std::true_type {};
template <>
struct is_ntt_friendly<1811939329> : std::true_type {};
template <>
struct is_ntt_friendly<2013265921> : std::true_type {};

template <uint32_t p>
std::vector<StaticModularInteger<p>> convolution_for_any_modulus(const std::vector<StaticModularInteger<p>>& a, const std::vector<StaticModularInteger<p>>& b);

template <uint32_t p>
std::vector<StaticModularInteger<p>> convolution_large(const std::vector<StaticModularInteger<p>>& a, const std::vector<StaticModularInteger<p>>& b);

template <uint32_t p>
std::vector<StaticModularInteger<p>> convolution(const std::vector<StaticModularInteger<p>>& a, const std::vector<StaticModularInteger<p>>& b) {
	int n = (int)a.size(), m = (int)b.size();
	if (n == 0 || m == 0) return {};
	if (n <= 60 || m <= 60) return convolution_detail::convolution_naive(a, b);
	if constexpr (convolution_detail::is_prime_modulus(p)) {
		if (n + m - 1 <= (int)((1 - p) & (p - 1))) {
			if (n == m && a == b) return convolution_detail::convolution_square(a);
			return convolution_detail::convolution(a, b);
		}
	}
	if constexpr (is_ntt_friendly<p>::value) {
		return convolution_large<p>(a, b);
	}
	return convolution_for_any_modulus(a, b);
}

template <uint32_t p>
std::vector<StaticModularInteger<p>> convolution_for_any_modulus(const std::vector<StaticModularInteger<p>>& a, const std::vector<StaticModularInteger<p>>& b) {
	int n = (int)a.size(), m = (int)b.size();
	if (n == 0 || m == 0) return {};
	assert(n + m - 1 <= (1 << 26));
	static constexpr uint32_t MOD1 = 469762049;
	static constexpr uint32_t MOD2 = 1811939329;
	static constexpr uint32_t MOD3 = 2013265921;
	using mint1 = StaticModularInteger<MOD1>;
	using mint2 = StaticModularInteger<MOD2>;
	using mint3 = StaticModularInteger<MOD3>;
	std::vector<mint1> a1(n), b1(m);
	std::vector<mint2> a2(n), b2(m);
	std::vector<mint3> a3(n), b3(m);
	for (int i = 0; i < n; i++) {
		uint32_t v = a[i].get();
		a1[i] = v;
		a2[i] = v;
		a3[i] = v;
	}
	for (int i = 0; i < m; i++) {
		uint32_t v = b[i].get();
		b1[i] = v;
		b2[i] = v;
		b3[i] = v;
	}
	auto c1 = convolution_detail::convolution(a1, b1);
	auto c2 = convolution_detail::convolution(a2, b2);
	auto c3 = convolution_detail::convolution(a3, b3);
	static constexpr uint32_t INV1_2 = mint2(MOD1).inverse().get();
	static constexpr uint32_t INV1_3 = mint3(MOD1).inverse().get();
	static constexpr uint32_t INV2_3 = mint3(MOD2).inverse().get();
	std::vector<StaticModularInteger<p>> res(n + m - 1);
	for (int i = 0; i < n + m - 1; i++) {
		long long t1 = c1[i].get();
		long long t2 = (c2[i].get() - t1 + MOD2) * INV1_2 % MOD2;
		if (t2 < 0) t2 += MOD2;
		long long t3 = ((c3[i].get() - t1 + MOD3) * INV1_3 % MOD3 - t2 + MOD3) * INV2_3 % MOD3;
		if (t3 < 0) t3 += MOD3;
		res[i] = StaticModularInteger<p>(t1 + (t2 + t3 * MOD2) % p * (MOD1 % p));
	}
	return res;
}

template <uint32_t p>
std::vector<StaticModularInteger<p>> convolution_large(const std::vector<StaticModularInteger<p>>& a, const std::vector<StaticModularInteger<p>>& b) {
	int n = (int)a.size(), m = (int)b.size();
	static_assert(p > 2 && convolution_detail::is_prime_modulus(p));
	if (n == 0 || m == 0) return {};
	int limit = (int)((1 - p) & (p - 1));
	int L = limit >> 1;
	int k = (n + L - 1) / L;
	int l = (m + L - 1) / L;
	std::vector<std::vector<StaticModularInteger<p>>> A_hat(k, std::vector<StaticModularInteger<p>>(limit));
	std::vector<std::vector<StaticModularInteger<p>>> B_hat(l, std::vector<StaticModularInteger<p>>(limit));
	for (int i = 0; i < k; ++i) {
		int start = i * L;
		int end = std::min(n, start + L);
		for (int j = start; j < end; ++j) {
			A_hat[i][j - start] = a[j];
		}
		convolution_detail::number_theoretic_transform(A_hat[i]);
	}
	for (int i = 0; i < l; ++i) {
		int start = i * L;
		int end = std::min(m, start + L);
		for (int j = start; j < end; ++j) {
			B_hat[i][j - start] = b[j];
		}
		convolution_detail::number_theoretic_transform(B_hat[i]);
	}
	std::vector<StaticModularInteger<p>> res(n + m - 1);
	for (int c = 0; c < k + l - 1; ++c) {
		std::vector<StaticModularInteger<p>> C_chunk(limit);
		for (int i = 0; i < k; ++i) {
			int j = c - i;
			if (0 <= j && j < l) {
				for (int x = 0; x < limit; ++x) {
					C_chunk[x] += A_hat[i][x] * B_hat[j][x];
				}
			}
		}
		convolution_detail::inverse_number_theoretic_transform(C_chunk);
		int start = c * L;
		int end = std::min(n + m - 1, start + limit);
		for (int j = start; j < end; ++j) {
			res[j] += C_chunk[j - start];
		}
	}
	return res;
}

template <uint32_t p>
std::vector<long long> convolution(const std::vector<long long>& a, const std::vector<long long>& b) {
	int n = (int)a.size(), m = (int)b.size();
	std::vector<StaticModularInteger<p>> a2(n), b2(m);
	for (int i = 0; i < n; i++) a2[i] = a[i];
	for (int i = 0; i < m; i++) b2[i] = b[i];
	auto c2 = convolution<p>(a2, b2);
	std::vector<long long> c(c2.size());
	for (int i = 0; i < (int)c2.size(); i++) c[i] = c2[i].get();
	return c;
}

namespace convolution_detail {

template <bool Signed>
std::vector<std::conditional_t<Signed, long long, uint64_t>> reconstruct_integer_convolution(const std::vector<long long>& a, const std::vector<long long>& b) {
	int n = (int)a.size(), m = (int)b.size();
	if (n == 0 || m == 0) return {};
	static constexpr uint32_t MOD1 = 469762049;
	static constexpr uint32_t MOD2 = 1811939329;
	static constexpr uint32_t MOD3 = 2013265921;
	using mint1 = StaticModularInteger<MOD1>;
	using mint2 = StaticModularInteger<MOD2>;
	using mint3 = StaticModularInteger<MOD3>;
	std::vector<mint1> a1(n), b1(m);
	std::vector<mint2> a2(n), b2(m);
	std::vector<mint3> a3(n), b3(m);
	for (int i = 0; i < n; i++) {
		long long v = a[i] % MOD1;
		if (v < 0) v += MOD1;
		a1[i] = v;
		v = a[i] % MOD2;
		if (v < 0) v += MOD2;
		a2[i] = v;
		v = a[i] % MOD3;
		if (v < 0) v += MOD3;
		a3[i] = v;
	}
	for (int i = 0; i < m; i++) {
		long long v = b[i] % MOD1;
		if (v < 0) v += MOD1;
		b1[i] = v;
		v = b[i] % MOD2;
		if (v < 0) v += MOD2;
		b2[i] = v;
		v = b[i] % MOD3;
		if (v < 0) v += MOD3;
		b3[i] = v;
	}
	auto c1 = convolution_detail::convolution(a1, b1);
	auto c2 = convolution_detail::convolution(a2, b2);
	auto c3 = convolution_detail::convolution(a3, b3);
	static constexpr uint32_t INV1_2 = mint2(MOD1).inverse().get();
	static constexpr uint32_t INV1_3 = mint3(MOD1).inverse().get();
	static constexpr uint32_t INV2_3 = mint3(MOD2).inverse().get();
	std::vector<std::conditional_t<Signed, long long, uint64_t>> res(n + m - 1);
	for (int i = 0; i < n + m - 1; i++) {
		long long t1 = c1[i].get();
		long long t2 = c2[i].get() - t1;
		t2 %= MOD2;
		if (t2 < 0) t2 += MOD2;
		t2 = (t2 * INV1_2) % MOD2;
		long long t3 = c3[i].get() - t1;
		t3 %= MOD3;
		if (t3 < 0) t3 += MOD3;
		t3 = (t3 * INV1_3) % MOD3;
		t3 = (t3 - t2) % MOD3;
		if (t3 < 0) t3 += MOD3;
		t3 = (t3 * INV2_3) % MOD3;
		__int128_t result = t1;
		result += (__int128_t)t2 * MOD1;
		result += (__int128_t)t3 * MOD1 * MOD2;
		if constexpr (Signed) {
			constexpr __int128 combined_modulus = __int128(MOD1) * MOD2 * MOD3;
			if (result > combined_modulus / 2) result -= combined_modulus;
			if (result < std::numeric_limits<long long>::min() || result > std::numeric_limits<long long>::max()) {
				throw std::overflow_error("signed convolution coefficient exceeds long long");
			}
			res[i] = static_cast<long long>(result);
		} else {
			res[i] = static_cast<uint64_t>(result);
		}
	}
	return res;
}

}  // namespace convolution_detail

// Exact if every output coefficient fits in long long. For out-of-domain
// inputs the CRT residues need not distinguish overflow from a small result.
inline std::vector<long long> signed_integer_convolution(const std::vector<long long>& a, const std::vector<long long>& b) {
	return convolution_detail::reconstruct_integer_convolution<true>(a, b);
}

// https://judge.yosupo.jp/problem/convolution_mod_2_64
inline std::vector<uint64_t> unsigned_integer_convolution(const std::vector<uint64_t>& a, const std::vector<uint64_t>& b) {
	int n = (int)a.size(), m = (int)b.size();
	if (n == 0 || m == 0) return {};
	std::vector<long long> a_lo(n), a_hi(n), b_lo(m), b_hi(m);
	for (int i = 0; i < n; i++) {
		a_lo[i] = a[i] & 0xFFFFFFFF;
		a_hi[i] = a[i] >> 32;
	}
	for (int i = 0; i < m; i++) {
		b_lo[i] = b[i] & 0xFFFFFFFF;
		b_hi[i] = b[i] >> 32;
	}
	auto c_lolo = convolution_detail::reconstruct_integer_convolution<false>(a_lo, b_lo);
	auto c_hilo = convolution_detail::reconstruct_integer_convolution<false>(a_hi, b_lo);
	auto c_lohi = convolution_detail::reconstruct_integer_convolution<false>(a_lo, b_hi);
	std::vector<uint64_t> res(n + m - 1);
	for (int i = 0; i < n + m - 1; i++) {
		uint64_t lolo = (uint64_t)c_lolo[i];
		uint64_t hilo = (uint64_t)c_hilo[i];
		uint64_t lohi = (uint64_t)c_lohi[i];
		res[i] = lolo + (hilo << 32) + (lohi << 32);
	}
	return res;
}

template <class T>
void double_number_theoretic_transform(std::vector<T>& a, std::vector<T> b) {
	static constexpr convolution_detail::NthRoot<T> nth_root;
	int n = (int)a.size();
	const T z = nth_root.get(convolution_detail::msb(n) + 1);
	T r = 1;
	for (int i = 0; i < n; i++) {
		b[i] *= r;
		r *= z;
	}
	convolution_detail::number_theoretic_transform(b);
	a.reserve(2 * n);
	a.insert(a.end(), b.begin(), b.end());
}

template <class T>
void double_number_theoretic_transform(std::vector<T>& a) {
	auto b = a;
	convolution_detail::inverse_number_theoretic_transform(b);
	double_number_theoretic_transform(a, std::move(b));
}

template <int ID>
std::vector<DynamicModularInteger<ID>> convolution(const std::vector<DynamicModularInteger<ID>>& a, const std::vector<DynamicModularInteger<ID>>& b) {
	int n = (int)a.size(), m = (int)b.size();
	if (n == 0 || m == 0) return {};
	if (n <= 60 || m <= 60) return convolution_detail::convolution_naive(a, b);
	uint32_t mod = DynamicModularInteger<ID>::get_modulus();
	if (mod == 1) return std::vector<DynamicModularInteger<ID>>(n + m - 1);
	if (mod == 998244353) {
		std::vector<ModularInteger998244353> left, right;
		left.reserve(n);
		right.reserve(m);
		for (auto x : a) left.push_back(ModularInteger998244353::raw(x.get()));
		for (auto x : b) right.push_back(ModularInteger998244353::raw(x.get()));
		auto product = convolution(left, right);
		std::vector<DynamicModularInteger<ID>> result;
		result.reserve(product.size());
		for (auto x : product) result.push_back(DynamicModularInteger<ID>::raw(x.get()));
		return result;
	}
	if (1LL * n + m - 1 > (1 << 26)) throw std::length_error("convolution exceeds CRT transform length");
	static constexpr uint32_t MOD1 = 469762049;
	static constexpr uint32_t MOD2 = 1811939329;
	static constexpr uint32_t MOD3 = 2013265921;
	using mint1 = StaticModularInteger<MOD1>;
	using mint2 = StaticModularInteger<MOD2>;
	using mint3 = StaticModularInteger<MOD3>;
	std::vector<mint1> a1(n), b1(m);
	std::vector<mint2> a2(n), b2(m);
	std::vector<mint3> a3(n), b3(m);
	for (int i = 0; i < n; i++) {
		uint32_t v = a[i].get();
		a1[i] = v;
		a2[i] = v;
		a3[i] = v;
	}
	for (int i = 0; i < m; i++) {
		uint32_t v = b[i].get();
		b1[i] = v;
		b2[i] = v;
		b3[i] = v;
	}
	auto c1 = convolution_detail::convolution(a1, b1);
	auto c2 = convolution_detail::convolution(a2, b2);
	auto c3 = convolution_detail::convolution(a3, b3);
	static constexpr uint32_t INV1_2 = mint2(MOD1).inverse().get();
	static constexpr uint32_t INV1_3 = mint3(MOD1).inverse().get();
	static constexpr uint32_t INV2_3 = mint3(MOD2).inverse().get();
	std::vector<DynamicModularInteger<ID>> res(n + m - 1);
	for (int i = 0; i < n + m - 1; i++) {
		long long t1 = c1[i].get();
		long long t2 = (c2[i].get() - t1 + MOD2) * INV1_2 % MOD2;
		if (t2 < 0) t2 += MOD2;
		long long t3 = ((c3[i].get() - t1 + MOD3) * INV1_3 % MOD3 - t2 + MOD3) * INV2_3 % MOD3;
		if (t3 < 0) t3 += MOD3;
		res[i] = DynamicModularInteger<ID>(t1 + (t2 + t3 * MOD2) % mod * MOD1);
	}
	return res;
}

namespace convolution_detail {

template <class T, class Multiply>
std::vector<T> truncated_polynomial_power(std::vector<T> coefficients, long long exponent, int terms, Multiply multiply) {
	if (exponent < 0 || terms < 0) throw std::invalid_argument("expected nonnegative exponent and term count");
	if (terms == 0) return {};
	std::vector<T> result(terms, T(0));
	if (exponent == 0) {
		result[0] = T(1);
		return result;
	}

	if (coefficients.size() > std::size_t(terms)) coefficients.resize(terms);
	const auto first = std::find_if(coefficients.begin(), coefficients.end(), [](const T& value) { return value != T(0); });
	if (first == coefficients.end()) return result;
	const int leading_zeros = int(first - coefficients.begin());
	if (leading_zeros > 0 && exponent > (terms - 1) / leading_zeros) return result;
	const int shift = int(leading_zeros * exponent);
	const std::size_t keep = terms - shift;
	coefficients.erase(coefficients.begin(), first);
	if (coefficients.size() > keep) coefficients.resize(keep);

	std::vector<T> power{T(1)};
	while (exponent > 0) {
		if (exponent & 1) {
			power = multiply(power, coefficients);
			if (power.size() > keep) power.resize(keep);
		}
		exponent >>= 1;
		if (exponent != 0) {
			coefficients = multiply(coefficients, coefficients);
			if (coefficients.size() > keep) coefficients.resize(keep);
		}
	}
	std::copy(power.begin(), power.end(), result.begin() + shift);
	return result;
}

}  // namespace convolution_detail

// Returns the first terms coefficients of a(x)^exponent, including trailing zeros.
template <class T>
std::vector<T> polynomial_power(std::vector<T> coefficients, long long exponent, int terms) {
	return convolution_detail::truncated_polynomial_power(std::move(coefficients), exponent, terms,
							      [](const std::vector<T>& left, const std::vector<T>& right) { return convolution(left, right); });
}

// Returns 1 for each reachable degree and 0 otherwise. Every nonzero input is
// treated as 1. Requires terms < 998244353 so exact pair counts fit in the
// internal convolution modulus, regardless of the caller's coefficient type.
template <class T>
std::vector<T> boolean_polynomial_power(std::vector<T> coefficients, long long exponent, int terms) {
	using Count = ModularInteger998244353;
	if (terms < 0 || exponent < 0) throw std::invalid_argument("expected nonnegative exponent and term count");
	if (terms >= static_cast<int>(Count::get_modulus())) throw std::length_error("too many boolean polynomial terms");

	const std::size_t input_size = std::min(coefficients.size(), std::size_t(terms));
	std::vector<Count> support(input_size);
	for (std::size_t i = 0; i < input_size; ++i) support[i] = coefficients[i] != T(0);

	auto reachable = convolution_detail::truncated_polynomial_power(std::move(support), exponent, terms,
									[](const std::vector<Count>& left, const std::vector<Count>& right) {
										auto counts = convolution(left, right);
										for (auto& count : counts) count = count != Count(0);
										return counts;
									});
	coefficients.resize(terms);
	for (int i = 0; i < terms; ++i) coefficients[i] = T(reachable[i].get());
	return coefficients;
}
