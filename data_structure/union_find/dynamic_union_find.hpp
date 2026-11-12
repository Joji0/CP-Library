#pragma once
#include <functional>
#include <limits>
#include <stdexcept>
#include <optional>
#include <unordered_map>
#include <vector>
#include "data_structure/union_find/union_find.hpp"

// Sparse keys are registered on add() or merge(). Read queries do not insert.
template <typename Key = long long, typename Hash = std::hash<Key>>
class DynamicUnionFind {
	UnionFind base;
	std::unordered_map<Key, int, Hash> ids;
	std::vector<Key> keys;

       public:
	bool add(const Key &key) {
		if (base.vertex_count() == std::numeric_limits<int>::max()) throw std::length_error("UnionFind vertex limit");
		const int id = base.vertex_count() + 1;
		auto [position, inserted] = ids.try_emplace(key, id);
		if (!inserted) return false;
		try {
			keys.push_back(key);
			try {
				base.add_vertex();
			} catch (...) {
				keys.pop_back();
				throw;
			}
		} catch (...) {
			ids.erase(position);
			throw;
		}
		return true;
	}

	bool contains(const Key &key) const { return ids.contains(key); }
	int count() const { return base.count(); }
	int vertex_count() const { return base.vertex_count(); }
	bool merge(const Key &first, const Key &second) {
		add(first);
		add(second);
		return base.merge(ids.at(first), ids.at(second));
	}
	bool same(const Key &first, const Key &second) {
		const auto left = ids.find(first), right = ids.find(second);
		return left != ids.end() && right != ids.end() && base.same(left->second, right->second);
	}
	int size(const Key &key) {
		const auto entry = ids.find(key);
		return entry == ids.end() ? 0 : base.size(entry->second);
	}
	std::optional<Key> leader(const Key &key) {
		const auto entry = ids.find(key);
		if (entry == ids.end()) return std::nullopt;
		return keys[base.find(entry->second) - 1];
	}
};
