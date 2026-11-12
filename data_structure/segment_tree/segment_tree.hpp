#pragma once
#include <algorithm>
#include <cassert>
#include <vector>

template <typename Monoid>
class SegmentTree {
	using T = typename Monoid::value_type;
	int n;
	std::vector<T> t;
	void build(const std::vector<T> &A, int v, int tl, int tr) {
		if (tl == tr) {
			t[v] = A[tl];
		} else {
			int tm = (tl + tr) / 2;
			build(A, v * 2, tl, tm);
			build(A, v * 2 + 1, tm + 1, tr);
			t[v] = Monoid::combine(t[v * 2], t[v * 2 + 1]);
		}
	}
	void update(int v, int tl, int tr, int pos, const T &new_val) {
		if (tl == tr) {
			t[v] = new_val;
		} else {
			int tm = (tl + tr) / 2;
			if (pos <= tm) {
				update(v * 2, tl, tm, pos, new_val);
			} else {
				update(v * 2 + 1, tm + 1, tr, pos, new_val);
			}
			t[v] = Monoid::combine(t[v * 2], t[v * 2 + 1]);
		}
	}
	T query(int v, int tl, int tr, int l, int r) const {
		if (l > r) {
			return Monoid::identity();
		}
		if (l == tl && r == tr) {
			return t[v];
		}
		int tm = (tl + tr) / 2;
		return Monoid::combine(query(v * 2, tl, tm, l, std::min(r, tm)),
				       query(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r));
	}
	template <class Pred>
	int max_right_dfs(int v, int tl, int tr, int l, Pred pred, T &acc) const {
		if (tr < l) return l;
		if (tl >= l) {
			T nxt = Monoid::combine(acc, t[v]);
			if (pred(nxt)) {
				acc = nxt;
				return tr + 1;
			}
			if (tl == tr) return tl;
		}
		int tm = (tl + tr) / 2;
		int res = max_right_dfs(v * 2, tl, tm, l, pred, acc);
		if (res <= tm) return res;
		return max_right_dfs(v * 2 + 1, tm + 1, tr, l, pred, acc);
	}
	template <class Pred>
	int min_left_dfs(int v, int tl, int tr, int r, Pred pred, T &acc) const {
		if (tl >= r) return tl - 1;
		if (tr < r) {
			T nxt = Monoid::combine(t[v], acc);
			if (pred(nxt)) {
				acc = nxt;
				return tl - 1;
			}
			if (tl == tr) return tl + 1;
		}
		int tm = (tl + tr) / 2;
		int res = min_left_dfs(v * 2 + 1, tm + 1, tr, r, pred, acc);
		if (res >= tm + 1) return res;
		return min_left_dfs(v * 2, tl, tm, r, pred, acc);
	}
	template <class Pred>
	int find_first_dfs(int v, int tl, int tr, int l, int r, Pred check) const {
		if (l > r || !check(t[v])) return -1;
		if (tl == tr) return tl;
		int tm = (tl + tr) / 2;
		int res = find_first_dfs(v * 2, tl, tm, l, std::min(r, tm), check);
		if (res != -1) return res;
		return find_first_dfs(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, check);
	}
	template <class Pred>
	int find_last_dfs(int v, int tl, int tr, int l, int r, Pred check) const {
		if (l > r || !check(t[v])) return -1;
		if (tl == tr) return tl;
		int tm = (tl + tr) / 2;
		int res = find_last_dfs(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, check);
		if (res != -1) return res;
		return find_last_dfs(v * 2, tl, tm, l, std::min(r, tm), check);
	}

       public:
	SegmentTree() : n(0) {}
	SegmentTree(int n) : n(n) { t.resize(4 * n, Monoid::identity()); }
	SegmentTree(const std::vector<T> &A) : n(int(A.size()) - 1) {
		assert(!A.empty());
		t.resize(4 * n, Monoid::identity());
		if (n > 0) build(A, 1, 1, n);
	}
	int size() const { return n; }
	bool empty() const { return n == 0; }
	T all_product() const { return empty() ? Monoid::identity() : t[1]; }

	void set(int pos, const T &new_val) { update(pos, new_val); }
	void update(int pos, const T &new_val) {
		assert(1 <= pos && pos <= n);
		update(1, 1, n, pos, new_val);
	}
	T query(int l, int r) const {
		if (l > r) return Monoid::identity();
		assert(1 <= l && r <= n);
		return query(1, 1, n, l, r);
	}
	T get(int pos) const { return query(pos, pos); }
	template <class Pred>
	int max_right(int l, Pred pred) const {
		assert(1 <= l && l <= n + 1);
		assert(pred(Monoid::identity()));
		T acc = Monoid::identity();
		return max_right_dfs(1, 1, n, l, pred, acc) - 1;
	}
	template <class Pred>
	int min_left(int r, Pred pred) const {
		assert(0 <= r && r <= n);
		assert(pred(Monoid::identity()));
		T acc = Monoid::identity();
		int res = min_left_dfs(1, 1, n, r + 1, pred, acc);
		return std::max(1, res);
	}
	template <class Pred>
	int find_first(int l, int r, Pred check) const {
		if (l > r) return -1;
		assert(1 <= l && r <= n);
		return find_first_dfs(1, 1, n, l, r, check);
	}
	template <class Pred>
	int find_last(int l, int r, Pred check) const {
		if (l > r) return -1;
		assert(1 <= l && r <= n);
		return find_last_dfs(1, 1, n, l, r, check);
	}
};
