#include <cassert>
#include <vector>
#include "mathematics/linear_algebra/matrix.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

int main() {
	Matrix<long long> matrix(2, 3, std::vector<long long>{999, 2, 0, 4, -1, 3, 1});
	assert(matrix.multiply_vector({999, 4, 7, -2}) == (std::vector<long long>{0, 0, 15}));
	Matrix<long long> zero_columns(3, 0);
	assert(zero_columns.multiply_vector({999}) == (std::vector<long long>{0, 0, 0, 0}));
	Matrix<long long> empty;
	assert(empty.trace() == 0);
	using Modular = StaticModularInteger<7>;
	Matrix<Modular> square(2, 2, std::vector<Modular>{99, 3, 1, 2, 6});
	assert(square.trace() == 2);
	assert(square.multiply_vector({99, 2, 3}) == (std::vector<Modular>{0, 2, 1}));
}
