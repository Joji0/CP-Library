#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <algorithm>
#include <cassert>
#include <optional>
#include <random>
#include <vector>
#include "acted_monoid/binary_inversions_assign.hpp"
#include "acted_monoid/binary_inversions_function.hpp"
#include "acted_monoid/binary_runs_assign.hpp"
#include "acted_monoid/binary_runs_flip.hpp"
#include "acted_monoid/binary_runs_function.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "monoid/binary_runs.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
enum class Update { flip,
		    assign,
		    function };

BinaryRuns::Run brute_run(const std::vector<int> &values, int left, int right, bool bit) {
	BinaryRuns::Run run;
	for (int i = left; i <= right && values[i] == bit; ++i) ++run.prefix;
	for (int i = right; i >= left && values[i] == bit; --i) ++run.suffix;
	int current = 0;
	for (int i = left; i <= right; ++i) {
		if (values[i] == bit) {
			if (current++ == 0) ++run.runs;
			run.longest = std::max(run.longest, current);
		} else {
			current = 0;
		}
	}
	return run;
}

template <typename Monoid, Update Operation, bool Runs>
void check() {
	using Action = typename Monoid::Action;
	constexpr int size = 27;
	std::mt19937 random(4231);
	std::vector<int> values(size + 1);
	for (int i = 1; i <= size; ++i) values[i] = random() & 1;
	LazySegmentTree<Monoid> tree(values);
	assert(tree.query(2, 1) == Monoid::identity());

	for (int iteration = 0; iteration < 400; ++iteration) {
		const int left = iteration % 5 == 0 ? 1 : 1 + random() % size;
		const int right = iteration % 5 == 0 ? size : left + random() % (size - left + 1);
		const bool first = (random() & 1) != 0;
		const bool second = (random() & 1) != 0;
		const Action action = [&]() -> Action {
			if constexpr (Operation == Update::flip) return first;
			if constexpr (Operation == Update::assign) return second ? Action(first) : std::nullopt;
			if constexpr (Operation == Update::function) return {first, second};
		}();
		tree.update(left, right, action);
		for (int i = left; i <= right; ++i) {
			if constexpr (Operation == Update::flip) values[i] ^= first;
			if constexpr (Operation == Update::assign) {
				if (second) values[i] = first;
			}
			if constexpr (Operation == Update::function) values[i] = values[i] ? second : first;
		}

		const int begin = iteration % 3 == 0 ? 1 : 1 + random() % size;
		const int end = iteration % 3 == 0 ? size : begin + random() % (size - begin + 1);
		const auto actual = tree.query(begin, end);
		if constexpr (Runs) {
			assert(actual.count == end - begin + 1);
			assert(actual.zero == brute_run(values, begin, end, false));
			assert(actual.one == brute_run(values, begin, end, true));
		} else {
			int ones = 0, zeros = 0, inversions = 0;
			for (int i = begin; i <= end; ++i) {
				if (values[i]) {
					++ones;
				} else {
					++zeros;
					inversions += ones;
				}
			}
			assert(actual.ones == ones && actual.zeros == zeros && actual.inversions == inversions);
		}
	}
}
}  // namespace

int main() {
	check<BinaryRunsFlip, Update::flip, true>();
	check<BinaryRunsAssign, Update::assign, true>();
	check<BinaryRunsFunction, Update::function, true>();
	check<BinaryInversionsAssign, Update::assign, false>();
	check<BinaryInversionsFunction, Update::function, false>();
	using Mixed = BinaryRunsFunction;
	LazySegmentTree<Mixed> tree(5);
	tree.update(1, 4, Mixed::make_assign(true));
	tree.update(2, 3, Mixed::make_flip());
	assert(tree.all_product().one.runs == 2 && tree.all_product().zero.longest == 2);
	answer_library_checker_aplusb();
}
