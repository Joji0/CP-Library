#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det_arbitrary_mod"
#include <iostream>
#include <vector>
#include "mathematics/linear_algebra/modular_determinant.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int n;
	long long modulus;
	std::cin >> n >> modulus;
	Matrix<long long> matrix(n, n);
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= n; ++j) std::cin >> matrix[i][j];
	std::cout << modular_determinant(matrix, modulus) << '\n';
}
