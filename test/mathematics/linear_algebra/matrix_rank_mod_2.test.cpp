#define PROBLEM "https://judge.yosupo.jp/problem/matrix_rank_mod_2"
#include <iostream>
#include <vector>
#include "mathematics/linear_algebra/bit_matrix.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int rows, columns;
	std::cin >> rows >> columns;
	BitMatrix matrix(rows, columns);
	for (int i = 1; i <= rows; ++i) {
		std::string line;
		std::cin >> line;
		for (int j = 1; j <= columns; ++j) matrix.set(i, j, line[j - 1] == '1');
	}
	std::cout << matrix.rank() << '\n';
}
