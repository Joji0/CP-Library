#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <optional>
#include <type_traits>
#include <vector>
#include "mathematics/linear_algebra/matrix.hpp"

// Linear Algebra operations: Rank, Determinant, Inverse, and Linear System Solver.
template <typename T>
class LinearAlgebra {
	// Elimination uses division and is defined over fields (or floating point).
	static_assert(!std::is_integral_v<T>, "Use field coefficients or modular_determinant");

	static bool is_zero(const T& val) {
		if constexpr (std::is_floating_point_v<T>) {
			return std::abs(val) < 1e-9;
		} else {
			return val == T(0);
		}
	}

       public:
	// Gaussian Elimination: returns rank and transforms matrix into row echelon form
	static int gaussian_elimination(Matrix<T>& a) {
		int n = a.rows();
		int m = a.columns();
		int rank = 1;

		for (int col = 1; col <= m && rank <= n; ++col) {
			int pivot = rank;
			for (int row = rank + 1; row <= n; ++row) {
				if constexpr (std::is_floating_point_v<T>) {
					if (std::abs(a[row][col]) > std::abs(a[pivot][col])) pivot = row;
				} else {
					if (is_zero(a[pivot][col]) && !is_zero(a[row][col])) {
						pivot = row;
						break;
					}
				}
			}

			if (is_zero(a[pivot][col])) continue;

			if (pivot != rank) {
				for (int j = 1; j <= m; ++j) std::swap(a[rank][j], a[pivot][j]);
			}

			T inverse = T(1) / a[rank][col];
			for (int j = col; j <= m; ++j) a[rank][j] *= inverse;

			for (int row = 1; row <= n; ++row) {
				if (row != rank && !is_zero(a[row][col])) {
					T factor = a[row][col];
					for (int j = col; j <= m; ++j) {
						a[row][j] -= factor * a[rank][j];
					}
				}
			}
			rank++;
		}
		return rank - 1;
	}

	static int rank(Matrix<T> a) {
		return gaussian_elimination(a);
	}

	static T determinant(Matrix<T> a) {
		assert(a.rows() == a.columns());
		int n = a.rows();
		T det = T(1);

		for (int col = 1; col <= n; ++col) {
			int pivot = col;
			for (int row = col + 1; row <= n; ++row) {
				if constexpr (std::is_floating_point_v<T>) {
					if (std::abs(a[row][col]) > std::abs(a[pivot][col])) pivot = row;
				} else if (is_zero(a[pivot][col]) && !is_zero(a[row][col])) {
					pivot = row;
					break;
				}
			}

			if (is_zero(a[pivot][col])) return T(0);

			if (pivot != col) {
				for (int j = col; j <= n; ++j) std::swap(a[col][j], a[pivot][j]);
				det = -det;
			}

			det *= a[col][col];
			T inverse = T(1) / a[col][col];
			for (int row = col + 1; row <= n; ++row) {
				if (!is_zero(a[row][col])) {
					T factor = a[row][col] * inverse;
					for (int j = col; j <= n; ++j) {
						a[row][j] -= factor * a[col][j];
					}
				}
			}
		}
		return det;
	}

	static std::optional<Matrix<T>> inverse(Matrix<T> a) {
		assert(a.rows() == a.columns());
		int n = a.rows();
		Matrix<T> aug(n, 2 * n, T(0));
		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= n; ++j) aug[i][j] = a[i][j];
			aug[i][n + i] = T(1);
		}

		int r = gaussian_elimination(aug);
		if (r < n) return std::nullopt;
		for (int i = 1; i <= n; ++i) {
			if (!is_zero(aug[i][i] - T(1))) return std::nullopt;
		}

		Matrix<T> inverse(n, n);
		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= n; ++j) inverse[i][j] = aug[i][n + j];
		}
		return inverse;
	}

	struct AffineSolution {
		std::vector<T> particular;
		// Dense collection of 1-based vectors. All solutions are particular
		// plus an arbitrary linear combination of these basis vectors.
		std::vector<std::vector<T>> nullspace_basis;
	};

	static std::optional<AffineSolution> solve_affine(const Matrix<T>& matrix, const std::vector<T>& right) {
		const int rows = matrix.rows(), columns = matrix.columns();
		assert(right.size() == std::size_t(rows) + 1);
		Matrix<T> augmented(rows, columns + 1);
		for (int row = 1; row <= rows; ++row) {
			for (int column = 1; column <= columns; ++column) augmented[row][column] = matrix[row][column];
			augmented[row][columns + 1] = right[row];
		}
		gaussian_elimination(augmented);
		AffineSolution solution{std::vector<T>(columns + 1), {}};
		std::vector<int> pivot_row(columns + 1);
		for (int row = 1; row <= rows; ++row) {
			int column = 1;
			while (column <= columns && is_zero(augmented[row][column])) ++column;
			if (column <= columns) {
				pivot_row[column] = row;
				solution.particular[column] = augmented[row][columns + 1];
			} else if (!is_zero(augmented[row][columns + 1])) {
				return std::nullopt;
			}
		}
		for (int free = 1; free <= columns; ++free) {
			if (pivot_row[free]) continue;
			std::vector<T> basis(columns + 1);
			basis[free] = T(1);
			for (int column = 1; column <= columns; ++column) {
				if (pivot_row[column]) basis[column] = -augmented[pivot_row[column]][free];
			}
			solution.nullspace_basis.push_back(std::move(basis));
		}
		return solution;
	}
	// Solves Ax = b. Returns one solution vector x, or nullopt if no solution exists.
	static std::optional<std::vector<T>> solve(Matrix<T> a, std::vector<T> b) {
		int n = a.rows();
		int m = a.columns();
		assert(int(b.size()) == n + 1);

		Matrix<T> aug(n, m + 1);
		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= m; ++j) aug[i][j] = a[i][j];
			aug[i][m + 1] = b[i];
		}

		gaussian_elimination(aug);

		std::vector<T> x(m + 1, T(0));
		std::vector<int> pivot_col(n + 1, -1);

		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= m; ++j) {
				if (!is_zero(aug[i][j])) {
					pivot_col[i] = j;
					break;
				}
			}
			if (pivot_col[i] == -1 && !is_zero(aug[i][m + 1])) {
				return std::nullopt;  // Inconsistent system (0 = non-zero)
			}
		}

		for (int i = 1; i <= n; ++i) {
			if (pivot_col[i] != -1) {
				x[pivot_col[i]] = aug[i][m + 1];
			}
		}
		return x;
	}
};
