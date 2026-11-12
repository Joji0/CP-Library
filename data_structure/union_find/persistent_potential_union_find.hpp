#pragma once
#include <cassert>
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>
#include "data_structure/segment_tree/persistent_segment_tree.hpp"
#include "data_structure/union_find/detail/potential_union_find_core.hpp"

// A fully persistent potential union find. Version 1 is the initial graph.
template <typename Group>
class PersistentPotentialUnionFind {
	using Value = typename Group::value_type;
	using Operations = union_find_detail::PotentialOperations<Group>;
	using Location = std::pair<int, Value>;
	struct Cell {
		int parent;
		Value relative;
	};
	struct RightmostCell {
		using value_type = Cell;
		static Cell identity() { return {0, Group::identity()}; }
		static Cell combine(const Cell &first, const Cell &second) {
			return second.parent == 0 ? first : second;
		}
	};
	struct Version {
		int root, components;
	};

	PersistentSegmentTree<RightmostCell> data;
	std::vector<Version> versions;

	static std::size_t storage_size(int size) {
		assert(size >= 0);
		return std::size_t(size) + 1;
	}

	const Version &at(int version) const {
		assert(1 <= version && version <= latest_version());
		return versions[version];
	}
	Location locate(int root, int vertex) const {
		auto cell = data.get(root, vertex);
		auto value = Group::identity();
		while (cell.parent > 0) {
			value = Group::combine(cell.relative, value);
			vertex = cell.parent;
			cell = data.get(root, vertex);
		}
		return {vertex, std::move(value)};
	}

       public:
	struct Update {
		int version;
		PotentialMergeResult status;
	};

	explicit PersistentPotentialUnionFind(int size = 0)
	    : data(std::vector<Cell>(storage_size(size), {-1, Group::identity()})),
	      versions{{0, 0}, {data.latest_root(), size}} {}

	int latest_version() const { return int(versions.size() - 1); }
	int count(int version) const { return at(version).components; }
	int find(int version, int vertex) const { return locate(at(version).root, vertex).first; }
	bool same(int version, int first, int second) const { return find(version, first) == find(version, second); }
	int size(int version, int vertex) const {
		const int root = at(version).root;
		return -data.get(root, locate(root, vertex).first).parent;
	}

	std::optional<Value> try_difference(int version, int from, int to) const {
		const int root = at(version).root;
		const auto [first, first_weight] = locate(root, from);
		const auto [second, second_weight] = locate(root, to);
		if (first != second) return std::nullopt;
		return Operations::difference(first_weight, second_weight);
	}
	Value difference(int version, int from, int to) const { return try_difference(version, from, to).value(); }

	// Creates a version for every constraint. A rejected constraint retains the old partition.
	Update constrain(int version, int from, int to, const Value &relation) {
		auto next = at(version);
		const auto [first, first_weight] = locate(next.root, from);
		const auto [second, second_weight] = locate(next.root, to);
		if (first == second) {
			const bool matches = Operations::matches(first_weight, relation, second_weight);
			versions.push_back(next);
			return {latest_version(), matches ? PotentialMergeResult::already_consistent : PotentialMergeResult::contradiction};
		}
		const int first_size = data.get(next.root, first).parent;
		const int second_size = data.get(next.root, second).parent;
		const bool reverse = first_size > second_size;
		const int root = reverse ? second : first;
		const int child = reverse ? first : second;
		const auto relative = Operations::connection(first_weight, relation, second_weight, reverse);
		next.root = data.update(next.root, root, {first_size + second_size, Group::identity()});
		next.root = data.update(next.root, child, {root, relative});
		--next.components;
		versions.push_back(next);
		return {latest_version(), PotentialMergeResult::merged};
	}
};
