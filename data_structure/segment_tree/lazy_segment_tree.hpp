#pragma once
#include <algorithm>
#include <cassert>
#include <type_traits>
#include <vector>

// ActedMonoid requirement:
// using Value = ...; (Node Type)
// using Action = ...; (Lazy Tag Type)
// static Value combine(Value a, Value b);
// static Value identity();
// static Value mapping(Action f, Value x);
// static Action composition(Action f, Action g); (f applied after g)
// static Action identity_action();
// Mapping distributes over combine and preserves the empty aggregate.
// Optional make_leaf(value) converts a raw value to a leaf; make_leaf() supplies a default leaf.

template <typename ActedMonoid>
class LazySegmentTree {
	using S = typename ActedMonoid::Value;
	using F = typename ActedMonoid::Action;
	int n;
	std::vector<S> t;
	std::vector<F> lazy;
	template <typename MakeLeaf>
	void build(const MakeLeaf &make_leaf, int v, int tl, int tr) {
		if (tl == tr) {
			t[v] = make_leaf(tl);
		} else {
			int tm = (tl + tr) / 2;
			build(make_leaf, v * 2, tl, tm);
			build(make_leaf, v * 2 + 1, tm + 1, tr);
			t[v] = ActedMonoid::combine(t[v * 2], t[v * 2 + 1]);
		}
	}
	void apply(int v, int tl, int tr, const F &f) {
		t[v] = ActedMonoid::mapping(f, t[v]);
		if (tl != tr) {
			lazy[v] = ActedMonoid::composition(f, lazy[v]);
		}
	}
	void push(int v, int tl, int tr) {
		int tm = (tl + tr) / 2;
		apply(v * 2, tl, tm, lazy[v]);
		apply(v * 2 + 1, tm + 1, tr, lazy[v]);
		lazy[v] = ActedMonoid::identity_action();
	}
	void update(int v, int tl, int tr, int l, int r, const F &f) {
		if (l > r) return;
		if (l == tl && r == tr) {
			apply(v, tl, tr, f);
		} else {
			push(v, tl, tr);
			int tm = (tl + tr) / 2;
			update(v * 2, tl, tm, l, std::min(r, tm), f);
			update(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, f);
			t[v] = ActedMonoid::combine(t[v * 2], t[v * 2 + 1]);
		}
	}
	void set(int v, int tl, int tr, int pos, const S &new_val) {
		if (tl == tr) {
			t[v] = new_val;
			lazy[v] = ActedMonoid::identity_action();
		} else {
			push(v, tl, tr);
			int tm = (tl + tr) / 2;
			if (pos <= tm)
				set(v * 2, tl, tm, pos, new_val);
			else
				set(v * 2 + 1, tm + 1, tr, pos, new_val);
			t[v] = ActedMonoid::combine(t[v * 2], t[v * 2 + 1]);
		}
	}
	S query(int v, int tl, int tr, int l, int r) {
		if (l > r) return ActedMonoid::identity();
		if (l == tl && r == tr) return t[v];
		push(v, tl, tr);
		int tm = (tl + tr) / 2;
		return ActedMonoid::combine(query(v * 2, tl, tm, l, std::min(r, tm)),
					    query(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r));
	}
	template <class Pred>
	int max_right_dfs(int v, int tl, int tr, int l, Pred pred, S &acc) {
		if (tr < l) return l;
		if (tl >= l) {
			S nxt = ActedMonoid::combine(acc, t[v]);
			if (pred(nxt)) {
				acc = nxt;
				return tr + 1;
			}
			if (tl == tr) return tl;
		}
		push(v, tl, tr);
		int tm = (tl + tr) / 2;
		int res = max_right_dfs(v * 2, tl, tm, l, pred, acc);
		if (res <= tm) return res;
		return max_right_dfs(v * 2 + 1, tm + 1, tr, l, pred, acc);
	}
	template <class Pred>
	int min_left_dfs(int v, int tl, int tr, int r, Pred pred, S &acc) {
		if (tl >= r) return tl - 1;
		if (tr < r) {
			S nxt = ActedMonoid::combine(t[v], acc);
			if (pred(nxt)) {
				acc = nxt;
				return tl - 1;
			}
			if (tl == tr) return tl + 1;
		}
		push(v, tl, tr);
		int tm = (tl + tr) / 2;
		int res = min_left_dfs(v * 2 + 1, tm + 1, tr, r, pred, acc);
		if (res >= tm + 1) return res;
		return min_left_dfs(v * 2, tl, tm, r, pred, acc);
	}

	void initialize_storage(int size) {
		assert(size >= 0);
		n = size;
		t.assign(std::size_t(n) * 4, ActedMonoid::identity());
		lazy.assign(std::size_t(n) * 4, ActedMonoid::identity_action());
	}

	struct BuildFromLeaves {};

	template <typename MakeLeaf>
	LazySegmentTree(int size, const MakeLeaf &make_leaf, BuildFromLeaves) {
		initialize_storage(size);
		if (n > 0) build(make_leaf, 1, 1, n);
	}

       public:
	LazySegmentTree() : n(0) {}
	LazySegmentTree(int size) {
		initialize_storage(size);
		if constexpr (requires { ActedMonoid::make_leaf(); }) {
			if (n > 0) {
				const S leaf = ActedMonoid::make_leaf();
				build([&](int) -> const S & { return leaf; }, 1, 1, n);
			}
		}
	}
	LazySegmentTree(int size, const S &initial_leaf)
	    : LazySegmentTree(size, [&](int) -> const S & { return initial_leaf; }, BuildFromLeaves{}) {}
	LazySegmentTree(const std::vector<S> &values)
	    : LazySegmentTree(int(values.size()) - 1, [&](int position) -> const S & { return values[position]; }, BuildFromLeaves{}) {}

	template <typename T>
		requires(!std::is_same_v<T, S> && requires(const T &value) { ActedMonoid::make_leaf(value); })
	explicit LazySegmentTree(const std::vector<T> &values) : LazySegmentTree(from_values(values)) {}

	template <typename T>
		requires requires(const T &value) { ActedMonoid::make_leaf(value); }
	static LazySegmentTree from_values(const std::vector<T> &values) {
		return LazySegmentTree(int(values.size()) - 1, [&](int position) { return ActedMonoid::make_leaf(values[position]); }, BuildFromLeaves{});
	}

	int size() const { return n; }
	bool empty() const { return n == 0; }
	S all_product() const { return empty() ? ActedMonoid::identity() : t[1]; }

	void update(int l, int r, const F &f) {
		if (l > r) return;
		assert(1 <= l && r <= n);
		update(1, 1, n, l, r, f);
	}
	void update(int pos, const F &f) { update(pos, pos, f); }
	void set(int pos, const S &new_val) {
		assert(1 <= pos && pos <= n);
		set(1, 1, n, pos, new_val);
	}
	template <typename T>
		requires requires(const T &value) { ActedMonoid::make_leaf(value); }
	void set_value(int position, const T &value) {
		set(position, ActedMonoid::make_leaf(value));
	}

	S query(int l, int r) {
		if (l > r) return ActedMonoid::identity();
		assert(1 <= l && r <= n);
		return query(1, 1, n, l, r);
	}
	S get(int pos) { return query(pos, pos); }
	template <class Pred>
	int max_right(int l, Pred pred) {
		assert(1 <= l && l <= n + 1);
		assert(pred(ActedMonoid::identity()));
		S acc = ActedMonoid::identity();
		return max_right_dfs(1, 1, n, l, pred, acc) - 1;
	}
	template <class Pred>
	int min_left(int r, Pred pred) {
		assert(0 <= r && r <= n);
		assert(pred(ActedMonoid::identity()));
		S acc = ActedMonoid::identity();
		int res = min_left_dfs(1, 1, n, r + 1, pred, acc);
		return std::max(1, res);
	}
};
