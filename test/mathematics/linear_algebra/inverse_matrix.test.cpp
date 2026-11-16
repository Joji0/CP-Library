#define PROBLEM "https://judge.yosupo.jp/problem/inverse_matrix"
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
	auto result = LinearAlgebra<Modular>::inverse(matrix);
	if (!result) {
		std::cout << "-1\n";
		return 0;
	}
	for (int i = 1; i <= rows; ++i) {
		for (int j = 1; j <= columns; ++j) std::cout << (*result)[i][j] << ' ';
		std::cout << '\n';
	}
}
