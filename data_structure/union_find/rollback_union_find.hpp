#pragma once
#include <algorithm>
#include <cassert>
#include <vector>

struct RollbackUnionFind {
	int n;
	std::vector<int> psiz;
	struct History {
		int u, v;
		int size_u, parent_v;
	};
	std::vector<History> history;
	RollbackUnionFind() : n(0) {}
	RollbackUnionFind(int n) : n(n) {
		assert(n >= 0);
		psiz.assign(std::size_t(n) + 1, -1);
	}
	int find(int i) const {
		assert(1 <= i && i <= n);
		while (psiz[i] >= 0) {
			i = psiz[i];
		}
		return i;
	}
	bool join(int u, int v) {
		int root_u = find(u);
		int root_v = find(v);
		if (root_u == root_v) return false;
		if (-psiz[root_u] < -psiz[root_v]) {
			std::swap(root_u, root_v);
		}
		history.push_back({root_u, root_v, psiz[root_u], psiz[root_v]});
		psiz[root_u] += psiz[root_v];
		psiz[root_v] = root_u;
		return true;
	}
	bool merge(int u, int v) { return join(u, v); }
	int count() const { return n - snapshot(); }
	bool same(int u, int v) const { return find(u) == find(v); }
	int size(int i) const { return -psiz[find(i)]; }
	int snapshot() const { return (int)history.size(); }
	void rollback(int snap) {
		assert(0 <= snap && snap <= snapshot());
		while ((int)history.size() > snap) {
			History h = history.back();
			history.pop_back();
			psiz[h.u] = h.size_u;
			psiz[h.v] = h.parent_v;
		}
	}
	void undo() {
		if (!history.empty()) rollback(snapshot() - 1);
	}
};
