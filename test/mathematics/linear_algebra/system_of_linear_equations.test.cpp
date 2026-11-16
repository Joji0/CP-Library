#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"
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
	std::cin >> columns;
	Matrix<Modular> matrix(rows, columns);
	for (int i = 1; i <= rows; ++i)
		for (int j = 1; j <= columns; ++j) std::cin >> matrix[i][j];
	std::vector<Modular> right(rows + 1);
	for (int i = 1; i <= rows; ++i) std::cin >> right[i];
	auto result = LinearAlgebra<Modular>::solve_affine(matrix, right);
	if (!result) {
		std::cout << "-1\n";
		return 0;
	}
	std::cout << result->nullspace_basis.size() << '\n';
	auto print = [&](const std::vector<Modular>& values) {
		for (int i = 1; i <= columns; ++i) std::cout << values[i] << ' ';
		std::cout << '\n';
	};
	print(result->particular);
	for (const auto& values : result->nullspace_basis) print(values);
}
