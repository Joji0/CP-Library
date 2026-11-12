#pragma once
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include "data_structure/segment_tree/persistent_segment_tree.hpp"
#include "monoid/minimum.hpp"

// Fully persistent. Version 1 is the initial graph, and every merge creates a version.
class PersistentUnionFind {
	struct Version {
		int root, components;
	};
	PersistentSegmentTree<Minimum<int>> data;
	std::vector<Version> versions;

	static std::size_t storage_size(int size) {
		assert(size >= 0);
		return std::size_t(size) + 1;
	}

	const Version &at(int version) const {
		assert(1 <= version && version <= latest_version());
		return versions[version];
	}

       public:
	explicit PersistentUnionFind(int size = 0) : data(std::vector<int>(storage_size(size), -1)), versions{{0, 0}, {data.latest_root(), size}} {}

	int latest_version() const { return int(versions.size() - 1); }
	int count(int version) const { return at(version).components; }
	int find(int version, int vertex) const {
		const int root = at(version).root;
		int parent = data.get(root, vertex);
		while (parent > 0) {
			vertex = parent;
			parent = data.get(root, vertex);
		}
		return vertex;
	}
	bool same(int version, int first, int second) const {
		return find(version, first) == find(version, second);
	}
	int size(int version, int vertex) const {
		return -data.get(at(version).root, find(version, vertex));
	}

	int merge(int version, int first, int second) {
		auto next = at(version);
		first = find(version, first);
		second = find(version, second);
		if (first != second) {
			int first_size = data.get(next.root, first);
			int second_size = data.get(next.root, second);
			if (first_size > second_size) {
				std::swap(first, second);
				std::swap(first_size, second_size);
			}
			next.root = data.update(next.root, first, first_size + second_size);
			next.root = data.update(next.root, second, first);
			--next.components;
		}
		versions.push_back(next);
		return latest_version();
	}
};
