#pragma once
#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

// Range monoid updates and point queries. Use composition(later, earlier) when provided;
// otherwise combine(later, earlier) applies earlier first and then later.
template <typename Monoid>
class DualSegmentTree {
	using T = typename Monoid::value_type;

	int _n, _size, _log;
	std::vector<T> _d;

	void all_apply(int k, const T &x) {
		if constexpr (requires { Monoid::composition(x, _d[k]); }) {
			_d[k] = Monoid::composition(x, _d[k]);
		} else {
			_d[k] = Monoid::combine(x, _d[k]);
		}
	}

	void push(int k) {
		all_apply(2 * k, _d[k]);
		all_apply(2 * k + 1, _d[k]);
		_d[k] = Monoid::identity();
	}

       public:
	DualSegmentTree() : DualSegmentTree(0) {}
	explicit DualSegmentTree(int n) : DualSegmentTree(std::vector<T>(n + 1, Monoid::identity())) {}
	explicit DualSegmentTree(const std::vector<T> &v) : _n(int(v.size()) - 1) {
		assert(!v.empty());
		_size = 1;
		_log = 0;
		while (_size < _n) {
			_size <<= 1;
			_log++;
		}
		_d.assign(2 * _size, Monoid::identity());
		for (int i = 0; i < _n; ++i) _d[_size + i] = v[i + 1];
	}

	int size() const { return _n; }
	bool empty() const { return _n == 0; }

	void set(int p, T x) {
		assert(1 <= p && p <= _n);
		p += _size - 1;
		for (int i = _log; i >= 1; --i) push(p >> i);
		_d[p] = std::move(x);
	}

	T get(int p) {
		assert(1 <= p && p <= _n);
		p += _size - 1;
		for (int i = _log; i >= 1; --i) push(p >> i);
		return _d[p];
	}

	T operator[](int p) { return get(p); }

	void apply(int p, T x) {
		assert(1 <= p && p <= _n);
		p += _size - 1;
		for (int i = _log; i >= 1; --i) push(p >> i);
		all_apply(p, x);
	}

	// Apply update on inclusive interval [l, r]
	void apply(int l, int r, T x) {
		if (l > r) return;
		assert(1 <= l && r <= _n);

		l += _size - 1;
		r += _size;
		for (int i = _log; i >= 1; --i) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}

		while (l < r) {
			if (l & 1) all_apply(l++, x);
			if (r & 1) all_apply(--r, x);
			l >>= 1;
			r >>= 1;
		}
	}

	std::vector<T> to_vector() {
		for (int k = 1; k < _size; ++k) push(k);
		std::vector<T> res(1, Monoid::identity());
		res.reserve(_n + 1);
		for (int i = 0; i < _n; ++i) res.push_back(_d[_size + i]);
		return res;
	}
};
