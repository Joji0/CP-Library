#pragma once
#include <cassert>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

enum class PotentialMergeResult { merged,
				  already_consistent,
				  contradiction };

namespace union_find_detail {
template <typename Group>
struct PotentialOperations {
	using Value = typename Group::value_type;
	static Value difference(const Value& from, const Value& to) {
		if constexpr (requires { Group::difference(from, to); })
			return Group::difference(from, to);
		else
			return Group::combine(Group::inverse(from), to);
	}
	static bool matches(const Value& from, const Value& relation, const Value& to) {
		if constexpr (requires { Group::matches(from, relation, to); })
			return Group::matches(from, relation, to);
		else
			return difference(from, to) == relation;
	}
	static Value connection(const Value& from, const Value& relation, const Value& to, bool reverse) {
		if constexpr (requires { Group::connection(from, relation, to, reverse); }) {
			return Group::connection(from, relation, to, reverse);
		} else {
			const auto value = Group::combine(Group::combine(from, relation), Group::inverse(to));
			return reverse ? Group::inverse(value) : value;
		}
	}
};

template <typename Group, bool Rollback>
class PotentialUnionFindCore {
       public:
	using value_type = typename Group::value_type;

       private:
	using Operations = PotentialOperations<Group>;
	using Location = std::pair<int, value_type>;
	struct Change {
		int root, child, root_size, child_size;
	};
	struct NoHistory {};

	std::vector<int> parent;
	std::vector<value_type> relative;
	int components;
	[[no_unique_address]] std::conditional_t<Rollback, std::vector<Change>, NoHistory> history;

	static std::size_t storage_size(int size) {
		assert(size >= 0);
		return std::size_t(size) + 1;
	}

	Location locate(int vertex) const {
		assert(1 <= vertex && vertex <= size());
		auto value = Group::identity();
		while (parent[vertex] > 0) {
			value = Group::combine(relative[vertex], value);
			vertex = parent[vertex];
		}
		return {vertex, std::move(value)};
	}

	Location locate(int vertex) {
		if constexpr (Rollback) {
			return std::as_const(*this).locate(vertex);
		} else {
			assert(1 <= vertex && vertex <= size());
			if (parent[vertex] < 0) return {vertex, Group::identity()};
			const auto [root, prefix] = locate(parent[vertex]);
			relative[vertex] = Group::combine(prefix, relative[vertex]);
			parent[vertex] = root;
			return {root, relative[vertex]};
		}
	}

	static std::optional<value_type> relation_of(const Location& from, const Location& to) {
		if (from.first != to.first) return std::nullopt;
		return Operations::difference(from.second, to.second);
	}

       public:
	explicit PotentialUnionFindCore(int size = 0)
	    : parent(storage_size(size), -1), relative(storage_size(size), Group::identity()), components(size) {}

	int size() const { return int(parent.size() - 1); }
	int count() const { return components; }
	int find(int vertex) { return locate(vertex).first; }
	int find(int vertex) const { return locate(vertex).first; }
	int leader(int vertex) { return find(vertex); }
	int leader(int vertex) const { return find(vertex); }
	int size(int vertex) { return -parent[find(vertex)]; }
	int size(int vertex) const { return -parent[find(vertex)]; }
	bool same(int first, int second) { return find(first) == find(second); }
	bool same(int first, int second) const { return find(first) == find(second); }

	value_type weight(int vertex) { return locate(vertex).second; }
	value_type weight(int vertex) const { return locate(vertex).second; }

	std::optional<value_type> try_difference(int from, int to) {
		const auto first = locate(from);
		return relation_of(first, locate(to));
	}
	std::optional<value_type> try_difference(int from, int to) const {
		return relation_of(locate(from), locate(to));
	}
	value_type difference(int from, int to) { return try_difference(from, to).value(); }
	value_type difference(int from, int to) const { return try_difference(from, to).value(); }

	// Enforces inverse(potential[from]) * potential[to] == relation.
	PotentialMergeResult constrain(int from, int to, const value_type& relation) {
		const auto [first, first_weight] = locate(from);
		const auto [second, second_weight] = locate(to);
		if (first == second) {
			return Operations::matches(first_weight, relation, second_weight)
				   ? PotentialMergeResult::already_consistent
				   : PotentialMergeResult::contradiction;
		}

		const bool reverse = parent[first] > parent[second];
		const int root = reverse ? second : first;
		const int child = reverse ? first : second;
		auto weight = Operations::connection(first_weight, relation, second_weight, reverse);
		relative[child] = std::move(weight);
		if constexpr (Rollback) history.push_back({root, child, parent[root], parent[child]});
		parent[root] += parent[child];
		parent[child] = root;
		--components;
		return PotentialMergeResult::merged;
	}

	// True also for a redundant constraint that is consistent.
	bool merge(int from, int to, const value_type& relation) {
		return constrain(from, to, relation) != PotentialMergeResult::contradiction;
	}
	bool unite(int from, int to, const value_type& relation) { return merge(from, to, relation); }

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
			parent[change.root] = change.root_size;
			parent[change.child] = change.child_size;
			++components;
		}
	}
	void undo()
		requires Rollback
	{
		if (!history.empty()) rollback(history.size() - 1);
	}
};
}  // namespace union_find_detail
