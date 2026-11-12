#pragma once
#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <random>
#include <utility>

// Ordered multiset with expected O(log n) operations. Ranks start at 1.
template <typename Key>
class Treap {
	struct Node;
	using Pointer = std::unique_ptr<Node>;

	struct Node {
		Key key;
		std::uint64_t priority;
		int count = 1;
		int size = 1;
		Pointer left, right;

		Node(const Key &key, std::uint64_t priority) : key(key), priority(priority) {}
	};

	Pointer root;

	static bool before(const Key &first, const Key &second) {
		if constexpr (requires { first < second; })
			return first < second;
		else
			return !(second <= first);
	}

	static std::uint64_t next_priority() {
		static thread_local std::mt19937_64 random(std::random_device{}());
		return random();
	}

	static int subtree_size(const Pointer &node) { return node ? node->size : 0; }

	static void pull(Node &node) {
		node.size = node.count + subtree_size(node.left) + subtree_size(node.right);
	}

	static Pointer clone(const Pointer &node) {
		if (!node) return nullptr;
		auto copy = std::make_unique<Node>(node->key, node->priority);
		copy->count = node->count;
		copy->size = node->size;
		copy->left = clone(node->left);
		copy->right = clone(node->right);
		return copy;
	}

	static void rotate_left(Pointer &node) {
		auto pivot = std::move(node->right);
		node->right = std::move(pivot->left);
		pull(*node);
		pivot->left = std::move(node);
		node = std::move(pivot);
	}

	static void rotate_right(Pointer &node) {
		auto pivot = std::move(node->left);
		node->left = std::move(pivot->right);
		pull(*node);
		pivot->right = std::move(node);
		node = std::move(pivot);
	}

	static void insert(Pointer &node, const Key &key) {
		if (!node) {
			node = std::make_unique<Node>(key, next_priority());
			return;
		}
		if (before(key, node->key)) {
			insert(node->left, key);
			if (node->left->priority > node->priority) rotate_right(node);
		} else if (before(node->key, key)) {
			insert(node->right, key);
			if (node->right->priority > node->priority) rotate_left(node);
		} else {
			++node->count;
		}
		pull(*node);
	}

	static Pointer merge(Pointer left, Pointer right) {
		if (!left) return right;
		if (!right) return left;
		if (left->priority > right->priority) {
			left->right = merge(std::move(left->right), std::move(right));
			pull(*left);
			return left;
		}
		right->left = merge(std::move(left), std::move(right->left));
		pull(*right);
		return right;
	}

	static int erase(Pointer &node, const Key &key, bool all) {
		if (!node) return 0;
		int erased;
		if (before(key, node->key)) {
			erased = erase(node->left, key, all);
		} else if (before(node->key, key)) {
			erased = erase(node->right, key, all);
		} else {
			erased = all ? node->count : 1;
			node->count -= erased;
			if (node->count == 0) {
				auto left = std::move(node->left), right = std::move(node->right);
				node = merge(std::move(left), std::move(right));
				return erased;
			}
		}
		pull(*node);
		return erased;
	}

	int count_before(const Key &key, bool inclusive) const {
		int result = 0;
		for (const Node *node = root.get(); node;) {
			const bool precedes = inclusive ? !before(key, node->key) : before(node->key, key);
			if (precedes) {
				result += subtree_size(node->left) + node->count;
				node = node->right.get();
			} else {
				node = node->left.get();
			}
		}
		return result;
	}

	const Node *bound(const Key &key, bool strict) const {
		const Node *result = nullptr;
		for (const Node *node = root.get(); node;) {
			const bool qualifies = strict ? before(key, node->key) : !before(node->key, key);
			if (qualifies) {
				result = node;
				node = node->left.get();
			} else {
				node = node->right.get();
			}
		}
		return result;
	}

       public:
	Treap() = default;
	Treap(const Treap &other) : root(clone(other.root)) {}
	Treap &operator=(const Treap &other) {
		if (this != &other) root = clone(other.root);
		return *this;
	}
	Treap(Treap &&) noexcept = default;
	Treap &operator=(Treap &&) noexcept = default;

	int size() const { return subtree_size(root); }
	bool empty() const { return !root; }
	void clear() { root.reset(); }

	void insert(const Key &key) { insert(root, key); }
	bool erase_one(const Key &key) { return erase(root, key, false) != 0; }
	int erase_all(const Key &key) { return erase(root, key, true); }

	int order_of_key(const Key &key) const { return count_before(key, false); }
	int count_less_equal(const Key &key) const { return count_before(key, true); }

	int count(const Key &key) const {
		for (const Node *node = root.get(); node;) {
			if (before(key, node->key))
				node = node->left.get();
			else if (before(node->key, key))
				node = node->right.get();
			else
				return node->count;
		}
		return 0;
	}

	bool contains(const Key &key) const { return count(key) != 0; }
	int count_range(const Key &left, const Key &right) const {
		return before(right, left) ? 0 : count_less_equal(right) - order_of_key(left);
	}

	const Key &find_by_order(int rank) const {
		assert(1 <= rank && rank <= size());
		const Node *node = root.get();
		while (true) {
			const int left_size = subtree_size(node->left);
			if (rank <= left_size) {
				node = node->left.get();
			} else if (rank <= left_size + node->count) {
				return node->key;
			} else {
				rank -= left_size + node->count;
				node = node->right.get();
			}
		}
	}

	std::optional<Key> kth(int rank) const {
		if (rank < 1 || rank > size()) return std::nullopt;
		return find_by_order(rank);
	}

	std::optional<Key> lower_bound(const Key &key) const {
		const Node *node = bound(key, false);
		if (!node) return std::nullopt;
		return node->key;
	}

	std::optional<Key> upper_bound(const Key &key) const {
		const Node *node = bound(key, true);
		if (!node) return std::nullopt;
		return node->key;
	}

	std::optional<Key> max_less_equal(const Key &key) const { return kth(count_less_equal(key)); }
};
