#pragma once
#include <cassert>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <vector>
#include "data_structure/union_find/detail/potential_union_find_core.hpp"

namespace union_find_detail {
template <typename Group, bool Rollback>
class ConstraintUnionFindCore {
	using Base = PotentialUnionFindCore<Group, Rollback>;
	struct Change {
		std::size_t potential_snapshot;
		int root, bad_count;
		bool root_was_bad;
	};
	struct NoHistory {};

	Base base;
	std::vector<char> bad;
	int bad_count = 0;
	[[no_unique_address]] std::conditional_t<Rollback, std::vector<Change>, NoHistory> history;

	void record(int root) {
		if constexpr (Rollback) history.push_back({base.snapshot(), root, bad_count, bool(bad[root])});
	}

       public:
	explicit ConstraintUnionFindCore(int size = 0) : base(size), bad(std::size_t(size) + 1, false) {}

	int count() const { return base.count(); }
	int count_inconsistent() const { return bad_count; }
	bool is_consistent() const { return bad_count == 0; }
	bool is_consistent(int vertex) { return !bad[base.find(vertex)]; }
	bool is_consistent(int vertex) const { return !bad[base.find(vertex)]; }
	bool same(int first, int second) { return base.same(first, second); }
	bool same(int first, int second) const { return base.same(first, second); }
	int size(int vertex) { return base.size(vertex); }
	int size(int vertex) const { return base.size(vertex); }

	// Records every constraint. Returns the consistency of the whole system afterwards.
	bool add_constraint(int first, int second, const typename Group::value_type &relation) {
		const int first_root = base.find(first);
		const int second_root = base.find(second);
		if (first_root == second_root) {
			if (!bad[first_root] &&
			    base.constrain(first, second, relation) == PotentialMergeResult::contradiction) {
				record(first_root);
				bad[first_root] = true;
				++bad_count;
			}
		} else {
			const int root = base.size(first_root) >= base.size(second_root) ? first_root : second_root;
			record(root);
			try {
				if (bad[first_root] || bad[second_root])
					base.constrain(first_root, second_root, Group::identity());
				else
					base.constrain(first, second, relation);
			} catch (...) {
				if constexpr (Rollback) history.pop_back();
				throw;
			}
			const bool first_bad = bad[first_root], second_bad = bad[second_root];
			bad[root] = first_bad || second_bad;
			if (first_bad && second_bad) --bad_count;
		}
		return is_consistent();
	}

	// Null when disconnected or when the component is inconsistent.
	std::optional<typename Group::value_type> try_difference(int first, int second) {
		if (!is_consistent(first)) return std::nullopt;
		return base.try_difference(first, second);
	}
	std::optional<typename Group::value_type> try_difference(int first, int second) const {
		if (!is_consistent(first)) return std::nullopt;
		return base.try_difference(first, second);
	}

	std::size_t snapshot() const
		requires Rollback
	{ return history.size(); }
	void rollback(std::size_t target)
		requires Rollback
	{
		assert(target <= history.size());
		while (history.size() > target) {
			const auto change = history.back();
			history.pop_back();
			base.rollback(change.potential_snapshot);
			bad[change.root] = change.root_was_bad;
			bad_count = change.bad_count;
		}
	}
	void undo()
		requires Rollback
	{
		if (!history.empty()) rollback(history.size() - 1);
	}
};
}  // namespace union_find_detail
