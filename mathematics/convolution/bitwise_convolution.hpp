#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>
#include <type_traits>
#include <bit>
#include "mathematics/set_function/subset_zeta_transform.hpp"
#include "mathematics/set_function/subset_mobius_transform.hpp"
#include "mathematics/set_function/superset_zeta_transform.hpp"
#include "mathematics/set_function/superset_mobius_transform.hpp"
#include <vector>

// Bitwise (ExclusiveOr, AND, OR) Convolutions via Walsh-Hadamard and SOS / Fast Mobius-Zeta Transforms.
class BitwiseConvolution {
       public:
	// Fast Walsh-Hadamard Transform (FWHT) for ExclusiveOr Convolution
	template <typename T>
	static void walsh_hadamard_transform(std::vector<T> &a, bool inverse = false) {
		const std::size_t n = a.size();
		if (!std::has_single_bit(n)) throw std::invalid_argument("expected one value per bitmask");
		for (std::size_t len = 1; len < n; len <<= 1) {
			for (std::size_t i = 0; i < n; i += 2 * len) {
				for (std::size_t j = 0; j < len; ++j) {
					T u = a[i + j];
					T v = a[i + len + j];
					a[i + j] = u + v;
					a[i + len + j] = u - v;
				}
			}
		}
		if (inverse) {
			if constexpr (std::is_integral_v<T>) {
				for (T &x : a) x /= T(n);
			} else {
				T inv_n = T(1) / T(n);
				for (T &x : a) x *= inv_n;
			}
		}
	}

	// Subset Zeta Transform (SOS DP) for OR Convolution
	template <typename T>
	static void subset_zeta(std::vector<T> &a) {
		subset_zeta_transform(a);
	}

	// Subset Mobius Transform (Inverse SOS DP)
	template <typename T>
	static void subset_mobius(std::vector<T> &a) {
		subset_mobius_transform(a);
	}

	// Superset Zeta Transform for AND Convolution
	template <typename T>
	static void superset_zeta(std::vector<T> &a) {
		superset_zeta_transform(a);
	}

	// Superset Mobius Transform
	template <typename T>
	static void superset_mobius(std::vector<T> &a) {
		superset_mobius_transform(a);
	}

	// ExclusiveOr Convolution: c[k] = sum_{i ^ j = k} a[i] * b[j]
	template <typename T>
	static std::vector<T> multiply_exclusive_or(std::vector<T> a, std::vector<T> b) {
		const std::size_t n = std::bit_ceil(std::max({std::size_t(1), a.size(), b.size()}));
		a.resize(n, T(0));
		b.resize(n, T(0));
		walsh_hadamard_transform(a);
		walsh_hadamard_transform(b);
		for (std::size_t i = 0; i < n; ++i) a[i] *= b[i];
		walsh_hadamard_transform(a, true);
		return a;
	}

	// AND Convolution: c[k] = sum_{i & j = k} a[i] * b[j]
	template <typename T>
	static std::vector<T> multiply_bitwise_and(std::vector<T> a, std::vector<T> b) {
		const std::size_t n = std::bit_ceil(std::max({std::size_t(1), a.size(), b.size()}));
		a.resize(n, T(0));
		b.resize(n, T(0));
		superset_zeta(a);
		superset_zeta(b);
		for (std::size_t i = 0; i < n; ++i) a[i] *= b[i];
		superset_mobius(a);
		return a;
	}

	// OR Convolution: c[k] = sum_{i | j = k} a[i] * b[j]
	template <typename T>
	static std::vector<T> multiply_inclusive_or(std::vector<T> a, std::vector<T> b) {
		const std::size_t n = std::bit_ceil(std::max({std::size_t(1), a.size(), b.size()}));
		a.resize(n, T(0));
		b.resize(n, T(0));
		subset_zeta(a);
		subset_zeta(b);
		for (std::size_t i = 0; i < n; ++i) a[i] *= b[i];
		subset_mobius(a);
		return a;
	}
};
