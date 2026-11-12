#pragma once
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <functional>
#include <optional>
#include <utility>
#include <limits>
#include <stdexcept>

template <class T, class Cmp = std::less<T>>
class OrderedMultiset {
	using Identifier = unsigned long long;
	static constexpr Identifier first_identifier = 0;
	static constexpr Identifier past_last_identifier = std::numeric_limits<Identifier>::max();
	using Key = std::pair<T, Identifier>;
	struct KeyCmp {
		Cmp cmp;
		bool operator()(const Key& a, const Key& b) const {
			if (cmp(a.first, b.first)) return true;
			if (cmp(b.first, a.first)) return false;
			return a.second < b.second;
		}
	};
	using OST = __gnu_pbds::tree<Key, __gnu_pbds::null_type, KeyCmp, __gnu_pbds::rb_tree_tag,
				     __gnu_pbds::tree_order_statistics_node_update>;
	OST tr;
	Identifier next_identifier = first_identifier;

	const Cmp& comparator() const { return tr.get_cmp_fn().cmp; }
	bool equivalent(const T& first, const T& second) const {
		return !comparator()(first, second) && !comparator()(second, first);
	}

       public:
	OrderedMultiset() = default;
	explicit OrderedMultiset(const Cmp& compare) : tr(KeyCmp{compare}) {}

	int size() const { return (int)tr.size(); }
	bool empty() const { return tr.empty(); }
	void clear() {
		tr.clear();
		next_identifier = first_identifier;
	}
	void insert(const T& x) {
		if (next_identifier == past_last_identifier) throw std::length_error("OrderedMultiset identifier space exhausted");
		tr.insert({x, next_identifier});
		++next_identifier;
	}
	bool erase_one(const T& x) {
		auto it = tr.lower_bound({x, first_identifier});
		if (it == tr.end() || !equivalent(x, it->first)) return false;
		tr.erase(it);
		return true;
	}
	int erase_all(const T& x) {
		auto it = tr.lower_bound({x, first_identifier});
		int erased = 0;
		while (it != tr.end() && equivalent(x, it->first)) {
			it = tr.erase(it);
			erased++;
		}
		return erased;
	}
	std::optional<T> erase_kth(int k) {
		if (k < 1 || k > size()) return std::nullopt;
		auto it = tr.find_by_order(k - 1);
		T val = it->first;
		tr.erase(it);
		return val;
	}
	bool contains(const T& x) const { return count(x) > 0; }
	int count(const T& x) const { return count_less_equal(x) - count_less(x); }
	int order_of_key(const T& x) const { return (int)tr.order_of_key({x, first_identifier}); }
	int order_of_key_next(const T& x) const { return (int)tr.order_of_key({x, past_last_identifier}); }
	std::optional<T> kth(int k) const {
		if (k < 1 || k > size()) return std::nullopt;
		return tr.find_by_order(k - 1)->first;
	}
	std::optional<T> first() const { return kth(1); }
	std::optional<T> last() const { return kth(size()); }
	std::optional<T> min_value() const { return first(); }
	std::optional<T> max_value() const { return last(); }
	std::optional<T> lower_bound(const T& x) const {
		auto it = tr.lower_bound({x, first_identifier});
		if (it == tr.end()) return std::nullopt;
		return it->first;
	}
	std::optional<T> upper_bound(const T& x) const {
		auto it = tr.upper_bound({x, past_last_identifier});
		if (it == tr.end()) return std::nullopt;
		return it->first;
	}
	std::optional<T> max_less(const T& x) const {
		auto it = tr.lower_bound({x, first_identifier});
		if (it == tr.begin()) return std::nullopt;
		--it;
		return it->first;
	}
	std::optional<T> max_less_equal(const T& x) const {
		auto it = tr.upper_bound({x, past_last_identifier});
		if (it == tr.begin()) return std::nullopt;
		--it;
		return it->first;
	}
	std::optional<T> min_greater(const T& x) const {
		return upper_bound(x);
	}
	std::optional<T> min_greater_equal(const T& x) const {
		return lower_bound(x);
	}
	int count_less(const T& x) const {
		return (int)tr.order_of_key({x, first_identifier});
	}
	int count_less_equal(const T& x) const {
		return (int)tr.order_of_key({x, past_last_identifier});
	}
	int count_greater(const T& x) const {
		return size() - count_less_equal(x);
	}
	int count_greater_equal(const T& x) const {
		return size() - count_less(x);
	}
	int count_halfopen(const T& left, const T& right) const {
		if (!comparator()(left, right)) return 0;
		return order_of_key(right) - order_of_key(left);
	}
	int count_range(const T& l, const T& r) const {
		if (comparator()(r, l)) return 0;
		return order_of_key_next(r) - order_of_key(l);
	}
};
