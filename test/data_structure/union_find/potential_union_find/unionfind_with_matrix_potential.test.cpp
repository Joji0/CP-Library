#define PROBLEM "https://judge.yosupo.jp/problem/unionfind_with_potential_non_commutative_group"
#include <iostream>
#include "data_structure/union_find/potential_union_find.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"

struct MatrixGroup {
	using Scalar = ModularInteger998244353;
	struct value_type {
		Scalar a, b, c, d;
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {1, 0, 0, 1}; }
	static value_type inverse(const value_type &value) { return {value.d, -value.b, -value.c, value.a}; }
	static value_type combine(const value_type &left, const value_type &right) {
		return {left.a * right.a + left.b * right.c, left.a * right.b + left.b * right.d,
			left.c * right.a + left.d * right.c, left.c * right.b + left.d * right.d};
	}
};

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int size, queries;
	std::cin >> size >> queries;
	PotentialUnionFind<MatrixGroup> sets(size);
	while (queries--) {
		int type, first, second;
		std::cin >> type >> first >> second;
		if (type == 0) {
			MatrixGroup::value_type value;
			std::cin >> value.a >> value.b >> value.c >> value.d;
			std::cout << sets.merge(second + 1, first + 1, value) << '\n';
		} else {
			const auto value = sets.try_difference(second + 1, first + 1);
			if (value)
				std::cout << value->a << ' ' << value->b << ' ' << value->c << ' ' << value->d << '\n';
			else
				std::cout << "-1\n";
		}
	}
}
