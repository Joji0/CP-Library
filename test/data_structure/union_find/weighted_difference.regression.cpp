#include <cassert>
#include "data_structure/union_find/weighted_union_find.hpp"

int main() {
	WeightedUnionFind<long long> components(4);
	assert(!components.try_difference(1, 2));
	assert(components.try_difference(3, 3) == 0);
	assert(components.merge(1, 2, -9));
	assert(components.merge(3, 4, 7));
	assert(components.merge(2, 4, 3));
	assert(components.try_difference(1, 3) == -13);
	assert(components.try_difference(3, 1) == 13);
	assert(!components.merge(1, 3, 2));
	assert(components.try_difference(1, 3) == -13);
}
