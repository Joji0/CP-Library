#pragma once
#include <optional>
#include "data_structure/union_find/detail/constraint_union_find_core.hpp"
#include "data_structure/union_find/detail/potential_groups.hpp"

namespace union_find_detail {
template <bool Rollback>
class BipartiteUnionFindCore : public ConstraintUnionFindCore<XorPotentialGroup<bool>, Rollback> {
	using Base = ConstraintUnionFindCore<XorPotentialGroup<bool>, Rollback>;

       public:
	using Base::Base;

	bool add_edge(int first, int second) { return this->add_constraint(first, second, true); }
	bool is_bipartite() const { return this->is_consistent(); }
	bool is_bipartite(int vertex) { return this->is_consistent(vertex); }
	bool is_bipartite(int vertex) const { return this->is_consistent(vertex); }
	int count_non_bipartite() const { return this->count_inconsistent(); }

	std::optional<bool> try_same_color(int first, int second) {
		const auto difference = this->try_difference(first, second);
		if (!difference) return std::nullopt;
		return !*difference;
	}
	std::optional<bool> try_same_color(int first, int second) const {
		const auto difference = this->try_difference(first, second);
		if (!difference) return std::nullopt;
		return !*difference;
	}

       private:
	using Base::add_constraint;
};
}  // namespace union_find_detail
