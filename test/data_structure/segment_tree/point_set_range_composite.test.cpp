#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"
#include <iostream>
#include <vector>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "monoid/affine.hpp"

using Modular = ModularInteger998244353;
using Function = Affine<Modular>;

struct LeftToRightComposition : Function {
	static value_type combine(const value_type &left, const value_type &right) {
		return Function::combine(right, left);
	}
};

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int size, queries;
	std::cin >> size >> queries;
	std::vector<Function::value_type> functions(size + 1);
	for (int position = 1; position <= size; ++position) {
		std::cin >> functions[position].a >> functions[position].b;
	}
	SegmentTree<LeftToRightComposition> tree(functions);
	while (queries--) {
		int type, left;
		std::cin >> type >> left;
		if (type == 0) {
			Function::value_type function;
			std::cin >> function.a >> function.b;
			tree.set(left + 1, function);
		} else {
			int right;
			Modular value;
			std::cin >> right >> value;
			const auto function = tree.query(left + 1, right);
			std::cout << function.a * value + function.b << '\n';
		}
	}
}
