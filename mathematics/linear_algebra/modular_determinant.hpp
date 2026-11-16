#pragma once
#include <stdexcept>
#include <utility>
#include "mathematics/linear_algebra/matrix.hpp"
#include "mathematics/number_theory/number_theory.hpp"

// Determinant modulo any positive signed 64-bit modulus, including composites.
// Uses integer Euclid on pivot columns so no modular inverse is needed.
// O(n^3 log modulus) time, with 1-based row/column indexing.
inline long long modular_determinant(Matrix<long long> matrix, long long modulus) {
	if (modulus <= 0 || matrix.rows() != matrix.columns()) throw std::invalid_argument("expected square matrix and positive modulus");
	const int n = matrix.rows();
	if (modulus == 1) return 0;
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) matrix[i][j] = NumberTheory::safe_modulo(matrix[i][j], modulus);
	}
	auto reduce = [&](long long left, long long quotient, long long right) {
		const long long result = (modulus <= 3'037'000'499LL) ? (left - quotient * right) % modulus
								      : (__int128(left) - __int128(quotient) * right) % modulus;
		return result < 0 ? result + modulus : result;
	};
	long long result = 1;
	for (int i = 1; i <= n; ++i) {
		for (int row = i + 1; row <= n; ++row) {
			while (matrix[row][i] != 0) {
				auto* pivot = matrix[i];
				auto* other = matrix[row];
				const long long quotient = pivot[i] / other[i];
				pivot[i] %= other[i];
				for (int col = i + 1; col <= n; ++col) pivot[col] = reduce(pivot[col], quotient, other[col]);
				for (int col = i; col <= n; ++col) std::swap(pivot[col], other[col]);
				if (result != 0) result = modulus - result;
			}
		}
		if (matrix[i][i] == 0) return 0;
		result = reduce(0, result, -matrix[i][i]);
	}
	return result;
}
