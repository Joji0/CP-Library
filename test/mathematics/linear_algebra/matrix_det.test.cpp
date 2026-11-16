#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det"
#include <iostream>
#include <vector>
#include "mathematics/linear_algebra/linear_algebra.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	using Modular = ModularInteger998244353;
	int rows, columns;
	std::cin >> rows;
	columns = rows;
	Matrix<Modular> matrix(rows, columns);
	for (int i = 1; i <= rows; ++i)
		for (int j = 1; j <= columns; ++j) std::cin >> matrix[i][j];
	std::cout << LinearAlgebra<Modular>::determinant(matrix) << '\n';
}
