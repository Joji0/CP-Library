#pragma once
#include <vector>
#include <numeric>
#include <cassert>
#include <limits>

struct KruskalReconstructionTree {
	int n, nodes;
	std::vector<int> par, lc, rc;
	std::vector<int> dsu_p;
	KruskalReconstructionTree(int n) : n(n), nodes(n) {
		assert(0 <= n && n <= std::numeric_limits<int>::max() / 2);
		par.assign(n == 0 ? 1 : 2 * n, -1);
		lc.assign(n == 0 ? 1 : 2 * n, -1);
		rc.assign(n == 0 ? 1 : 2 * n, -1);
		dsu_p.resize(n == 0 ? 1 : 2 * n);
		std::iota(dsu_p.begin(), dsu_p.end(), 0);
	}
	int find(int i) {
		assert(1 <= i && i <= nodes);
		int root = i;
		while (root != dsu_p[root]) root = dsu_p[root];
		while (i != root) {
			const int next = dsu_p[i];
			dsu_p[i] = root;
			i = next;
		}
		return root;
	}
	int join(int u, int v) {
		u = find(u), v = find(v);
		if (u == v) return -1;
		int w = ++nodes;
		par[u] = par[v] = w;
		lc[w] = u;
		rc[w] = v;
		dsu_p[u] = dsu_p[v] = w;
		return w;
	}
};
