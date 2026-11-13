#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <vector>
#include "acted_monoid/bitwise_and_bitwise_and.hpp"
#include "acted_monoid/bitwise_and_bitwise_assign.hpp"
#include "acted_monoid/bitwise_and_bitwise_function.hpp"
#include "acted_monoid/bitwise_and_bitwise_not.hpp"
#include "acted_monoid/bitwise_and_bitwise_or.hpp"
#include "acted_monoid/bitwise_and_exclusive_or.hpp"
#include "acted_monoid/bitwise_or_bitwise_and.hpp"
#include "acted_monoid/bitwise_or_bitwise_assign.hpp"
#include "acted_monoid/bitwise_or_bitwise_function.hpp"
#include "acted_monoid/bitwise_or_bitwise_not.hpp"
#include "acted_monoid/bitwise_or_bitwise_or.hpp"
#include "acted_monoid/bitwise_or_exclusive_or.hpp"
#include "acted_monoid/exclusive_or_bitwise_and.hpp"
#include "acted_monoid/exclusive_or_bitwise_assign.hpp"
#include "acted_monoid/exclusive_or_bitwise_function.hpp"
#include "acted_monoid/exclusive_or_bitwise_not.hpp"
#include "acted_monoid/exclusive_or_bitwise_or.hpp"
#include "acted_monoid/exclusive_or_exclusive_or.hpp"
#include "acted_monoid/sum_bitwise_and.hpp"
#include "acted_monoid/sum_bitwise_assign.hpp"
#include "acted_monoid/sum_bitwise_function.hpp"
#include "acted_monoid/sum_bitwise_not.hpp"
#include "acted_monoid/sum_bitwise_or.hpp"
#include "acted_monoid/sum_exclusive_or.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
enum class Update { bitwise_and,
		    bitwise_or,
		    exclusive_or,
		    assign,
		    bitwise_not,
		    function };
enum class Query { sum,
		   bitwise_and,
		   bitwise_or,
		   exclusive_or };
constexpr unsigned mask = 1023;

template <typename Monoid, Update Operation, Query Result>
void check() {
	using Action = typename Monoid::Action;
	const auto action = [](unsigned first, unsigned second) -> Action {
		if constexpr (Operation == Update::bitwise_not) return (first & 1) != 0;
		if constexpr (Operation == Update::function) return {first, second};
		if constexpr (Operation == Update::assign) return static_cast<int>(first);
		if constexpr (Operation == Update::bitwise_and || Operation == Update::bitwise_or || Operation == Update::exclusive_or)
			return first;
	};
	const auto apply = [](unsigned value, unsigned first, unsigned second) {
		if constexpr (Operation == Update::bitwise_and) return value & first;
		if constexpr (Operation == Update::bitwise_or) return value | first;
		if constexpr (Operation == Update::exclusive_or) return value ^ first;
		if constexpr (Operation == Update::assign) return first;
		if constexpr (Operation == Update::bitwise_not) return (first & 1) ? (value ^ mask) : value;
		if constexpr (Operation == Update::function) return (~value & first) | (value & second);
	};

	constexpr int size = 23;
	std::mt19937 random(2391);
	std::vector<int> values(size + 1);
	values[0] = -1;
	for (int i = 1; i <= size; ++i) values[i] = random() & mask;
	LazySegmentTree<Monoid> tree(values);
	assert(tree.query(2, 1) == Monoid::identity());
	assert(Monoid::mapping(action(0, 0), Monoid::identity()) == Monoid::identity());

	for (int iteration = 0; iteration < 400; ++iteration) {
		const int left = iteration % 5 == 0 ? 1 : 1 + random() % size;
		const int right = iteration % 5 == 0 ? size : left + random() % (size - left + 1);
		const unsigned first = random() & mask;
		const unsigned second = random() & mask;
		const Action update = action(first, second);
		tree.update(left, right, update);
		for (int i = left; i <= right; ++i) values[i] = apply(values[i], first, second);
		if (iteration % 9 == 0) {
			const int position = 1 + random() % size;
			tree.set_value(position, first);
			values[position] = first;
		}
		const int begin = 1 + random() % size;
		const int end = begin + random() % (size - begin + 1);
		const auto actual = tree.query(begin, end);
		assert(actual.count == end - begin + 1);

		if constexpr (Result == Query::sum) {
			long long sum = 0;
			for (int i = begin; i <= end; ++i) sum += values[i];
			assert(Monoid::sum(actual) == sum);
			for (std::size_t bit = 0; bit < actual.ones.size(); ++bit) {
				int ones = 0;
				for (int i = begin; i <= end; ++i) ones += (values[i] >> bit) & 1;
				assert(actual.ones[bit] == ones);
			}
		} else {
			unsigned bitwise_and = mask, bitwise_or = 0, exclusive_or = 0;
			for (int i = begin; i <= end; ++i) {
				bitwise_and &= values[i];
				bitwise_or |= values[i];
				exclusive_or ^= values[i];
			}
			assert(actual.bitwise_and == bitwise_and);
			assert(actual.bitwise_or == bitwise_or);
			assert(actual.exclusive_or == exclusive_or);
			if constexpr (Result == Query::bitwise_and) assert(Monoid::product(actual) == static_cast<int>(bitwise_and));
			if constexpr (Result == Query::bitwise_or) assert(Monoid::product(actual) == static_cast<int>(bitwise_or));
			if constexpr (Result == Query::exclusive_or) assert(Monoid::product(actual) == static_cast<int>(exclusive_or));
		}
	}
}

void check_boundaries() {
	using Signed = SumExclusiveOr<>;
	const auto lowest = std::numeric_limits<long long>::lowest();
	const auto highest = std::numeric_limits<long long>::max();
	LazySegmentTree<Signed> signed_tree(std::vector<long long>{17, lowest, highest});
	assert(Signed::sum(signed_tree.all_product()) == -1);
	signed_tree.update(1, 2, 1ULL << 63);
	assert(Signed::sum(signed_tree.get(1)) == 0);
	assert(Signed::sum(signed_tree.get(2)) == -1);
	assert(Signed::sum(signed_tree.all_product()) == -1);

	using Wide = SumBitwiseAssign<unsigned long long, 64, unsigned __int128>;
	const auto maximum = std::numeric_limits<unsigned long long>::max();
	LazySegmentTree<Wide> unsigned_tree(4);
	unsigned_tree.update(1, 4, maximum);
	assert(Wide::sum(unsigned_tree.all_product()) == static_cast<unsigned __int128>(maximum) * 4);

	using Small = SumBitwiseNot<signed char>;
	LazySegmentTree<Small> bytes(std::vector<signed char>{0, -128, 0, 127, -1});
	bytes.update(1, 4, true);
	assert(Small::sum(bytes.query(1, 3)) == -2);
	assert(Small::sum(bytes.get(4)) == 0);

	using Mixed = SumBitwiseFunction<int, 5>;
	LazySegmentTree<Mixed> mixed(std::vector<int>{-100, 1, 2, 3, 4});
	mixed.update(1, 4, Mixed::make_assign(4));
	mixed.update(2, 4, Mixed::make_exclusive_or(1));
	mixed.update(1, 3, Mixed::make_and(6));
	mixed.update(1, 2, Mixed::make_or(16));
	mixed.update(3, 4, Mixed::make_not());
	assert(Mixed::sum(mixed.all_product()) == 93);
}
}  // namespace

int main() {
	check<SumBitwiseAnd<int, 10>, Update::bitwise_and, Query::sum>();
	check<SumBitwiseOr<int, 10>, Update::bitwise_or, Query::sum>();
	check<SumExclusiveOr<int, 10>, Update::exclusive_or, Query::sum>();
	check<SumBitwiseAssign<int, 10>, Update::assign, Query::sum>();
	check<SumBitwiseNot<int, 10>, Update::bitwise_not, Query::sum>();
	check<SumBitwiseFunction<int, 10>, Update::function, Query::sum>();
	check<BitwiseAndBitwiseAnd<int, 10>, Update::bitwise_and, Query::bitwise_and>();
	check<BitwiseAndBitwiseOr<int, 10>, Update::bitwise_or, Query::bitwise_and>();
	check<BitwiseAndExclusiveOr<int, 10>, Update::exclusive_or, Query::bitwise_and>();
	check<BitwiseAndBitwiseAssign<int, 10>, Update::assign, Query::bitwise_and>();
	check<BitwiseAndBitwiseNot<int, 10>, Update::bitwise_not, Query::bitwise_and>();
	check<BitwiseAndBitwiseFunction<int, 10>, Update::function, Query::bitwise_and>();
	check<BitwiseOrBitwiseAnd<int, 10>, Update::bitwise_and, Query::bitwise_or>();
	check<BitwiseOrBitwiseOr<int, 10>, Update::bitwise_or, Query::bitwise_or>();
	check<BitwiseOrExclusiveOr<int, 10>, Update::exclusive_or, Query::bitwise_or>();
	check<BitwiseOrBitwiseAssign<int, 10>, Update::assign, Query::bitwise_or>();
	check<BitwiseOrBitwiseNot<int, 10>, Update::bitwise_not, Query::bitwise_or>();
	check<BitwiseOrBitwiseFunction<int, 10>, Update::function, Query::bitwise_or>();
	check<ExclusiveOrBitwiseAnd<int, 10>, Update::bitwise_and, Query::exclusive_or>();
	check<ExclusiveOrBitwiseOr<int, 10>, Update::bitwise_or, Query::exclusive_or>();
	check<ExclusiveOrExclusiveOr<int, 10>, Update::exclusive_or, Query::exclusive_or>();
	check<ExclusiveOrBitwiseAssign<int, 10>, Update::assign, Query::exclusive_or>();
	check<ExclusiveOrBitwiseNot<int, 10>, Update::bitwise_not, Query::exclusive_or>();
	check<ExclusiveOrBitwiseFunction<int, 10>, Update::function, Query::exclusive_or>();
	check_boundaries();
	answer_library_checker_aplusb();
}
