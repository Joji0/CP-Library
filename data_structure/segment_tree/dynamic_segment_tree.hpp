#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <numeric>
#include <utility>
#include <vector>

// Dynamic (Sparse) Segment Tree over an arbitrary index range [left, right].
// Nodes are created on-the-fly in O(log(range)).
template <typename Monoid, typename Index = long long>
class DynamicSegmentTree {
	using T = typename Monoid::value_type;

	struct Node {
		T val;
		int left;
		int right;

		Node() : val(Monoid::identity()), left(-1), right(-1) {}
		explicit Node(T v) : val(std::move(v)), left(-1), right(-1) {}
	};

	Index _l, _r;
	int _root;
	std::vector<Node> _nodes;

	int new_node() {
		_nodes.emplace_back();
		return int(_nodes.size()) - 1;
	}

	T node_val(int t) const {
		return t == -1 ? Monoid::identity() : _nodes[t].val;
	}

	void update_node(int t) {
		_nodes[t].val = Monoid::combine(node_val(_nodes[t].left), node_val(_nodes[t].right));
	}

	int set_impl(int t, Index l, Index r, Index pos, const T &v) {
		if (t == -1) t = new_node();
		if (l == r) {
			_nodes[t].val = v;
			return t;
		}
		Index m = std::midpoint(l, r);
		if (pos <= m) {
			int nxt = set_impl(_nodes[t].left, l, m, pos, v);
			_nodes[t].left = nxt;
		} else {
			int nxt = set_impl(_nodes[t].right, m + 1, r, pos, v);
			_nodes[t].right = nxt;
		}
		update_node(t);
		return t;
	}

	int apply_impl(int t, Index l, Index r, Index pos, const T &v) {
		if (t == -1) t = new_node();
		if (l == r) {
			_nodes[t].val = Monoid::combine(_nodes[t].val, v);
			return t;
		}
		Index m = std::midpoint(l, r);
		if (pos <= m) {
			int nxt = apply_impl(_nodes[t].left, l, m, pos, v);
			_nodes[t].left = nxt;
		} else {
			int nxt = apply_impl(_nodes[t].right, m + 1, r, pos, v);
			_nodes[t].right = nxt;
		}
		update_node(t);
		return t;
	}

	T prod_impl(int t, Index l, Index r, Index ql, Index qr) const {
		if (t == -1 || qr < l || r < ql) return Monoid::identity();
		if (ql <= l && r <= qr) return _nodes[t].val;
		Index m = std::midpoint(l, r);
		return Monoid::combine(prod_impl(_nodes[t].left, l, m, ql, qr),
				       prod_impl(_nodes[t].right, m + 1, r, ql, qr));
	}

       public:
	DynamicSegmentTree() : _l(1), _r(0), _root(-1) {}
	DynamicSegmentTree(Index l, Index r) : _l(l), _r(r), _root(-1) {
		assert(l <= r);
	}

	Index left_bound() const { return _l; }
	Index right_bound() const { return _r; }
	std::size_t node_count() const { return _nodes.size(); }

	void clear() {
		_root = -1;
		_nodes.clear();
	}

	void set(Index pos, const T &val) {
		assert(_l <= pos && pos <= _r);
		_root = set_impl(_root, _l, _r, pos, val);
	}

	void apply(Index pos, const T &val) {
		assert(_l <= pos && pos <= _r);
		_root = apply_impl(_root, _l, _r, pos, val);
	}

	T get(Index pos) const {
		return product(pos, pos);
	}

	// Range query on inclusive interval [ql, qr]
	T product(Index ql, Index qr) const {
		if (ql > qr) return Monoid::identity();
		assert(_l <= ql && qr <= _r);
		return prod_impl(_root, _l, _r, ql, qr);
	}

	// Range query on inclusive interval [l, r]
	T query(Index l, Index r) const {
		return product(l, r);
	}

	T all_product() const {
		return node_val(_root);
	}
};
