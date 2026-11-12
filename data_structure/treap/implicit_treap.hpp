#pragma once
#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <random>
#include <utility>
#include <type_traits>
#include <vector>

// Dynamic sequence with 1-based positions and inclusive ranges.
// Point and range operations take expected O(log n). The acted monoid must
// apply its action elementwise and supply nonthrowing hooks for mutation.
template <typename ActedMonoid>
class ImplicitTreap {
	template <typename U>
	static auto value_type_of(int) -> typename U::Value;
	template <typename U>
	static auto value_type_of(...) -> typename U::value_type;
	template <typename U>
	static auto action_type_of(int) -> typename U::Action;
	template <typename U>
	static auto action_type_of(...) -> typename U::operator_type;

       public:
	using value_type = decltype(value_type_of<ActedMonoid>(0));
	using action_type = decltype(action_type_of<ActedMonoid>(0));

       private:
	static value_type identity() {
		if constexpr (requires { ActedMonoid::identity(); })
			return ActedMonoid::identity();
		else
			return ActedMonoid::id();
	}

	static action_type identity_action() {
		if constexpr (requires { ActedMonoid::identity_action(); })
			return ActedMonoid::identity_action();
		else
			return ActedMonoid::op_id();
	}

	static value_type mapping(const action_type &action, const value_type &value) {
		if constexpr (requires { ActedMonoid::mapping(action, value); })
			return ActedMonoid::mapping(action, value);
		else
			return ActedMonoid::mapping(value, action);
	}

	static action_type composition(const action_type &later, const action_type &earlier) {
		if constexpr (requires { ActedMonoid::composition(later, earlier); })
			return ActedMonoid::composition(later, earlier);
		else
			return ActedMonoid::op_comp(later, earlier);
	}

	struct Node;
	using Pointer = std::unique_ptr<Node>;

	struct Node {
		value_type value, product, reversed_product;
		action_type lazy;
		std::uint64_t priority;
		int size = 1;
		bool reversed = false;
		bool has_lazy = false;
		Pointer left, right;

		Node(value_type value, std::uint64_t priority)
		    : value(std::move(value)), product(this->value), reversed_product(this->value), lazy(identity_action()), priority(priority) {}
	};

	Pointer root;

	static std::uint64_t next_priority() {
		static thread_local std::mt19937_64 random(std::random_device{}());
		return random();
	}

	static int subtree_size(const Pointer &node) { return node ? node->size : 0; }
	static value_type product(const Pointer &node) { return node ? node->product : identity(); }
	static value_type reversed_product(const Pointer &node) { return node ? node->reversed_product : identity(); }

	static void pull(Node &node) {
		node.size = 1 + subtree_size(node.left) + subtree_size(node.right);
		node.product = ActedMonoid::combine(ActedMonoid::combine(product(node.left), node.value), product(node.right));
		node.reversed_product = ActedMonoid::combine(
		    ActedMonoid::combine(reversed_product(node.right), node.value), reversed_product(node.left));
	}

	static void apply_node(const Pointer &node, const action_type &action) {
		if (!node) return;
		node->value = mapping(action, node->value);
		node->product = mapping(action, node->product);
		node->reversed_product = mapping(action, node->reversed_product);
		node->lazy = node->has_lazy ? composition(action, node->lazy) : action;
		node->has_lazy = true;
	}

	static void reverse_node(const Pointer &node) {
		if (!node) return;
		std::swap(node->left, node->right);
		std::swap(node->product, node->reversed_product);
		node->reversed = !node->reversed;
	}

	static void push(Node &node) {
		if (node.reversed) {
			reverse_node(node.left);
			reverse_node(node.right);
			node.reversed = false;
		}
		if (node.has_lazy) {
			apply_node(node.left, node.lazy);
			apply_node(node.right, node.lazy);
			node.has_lazy = false;
		}
	}

	static std::pair<Pointer, Pointer> split(Pointer node, int left_size) {
		if (left_size == 0) return {nullptr, std::move(node)};
		if (left_size == node->size) return {std::move(node), nullptr};
		push(*node);
		const int position = subtree_size(node->left) + 1;
		if (left_size < position) {
			auto [left, rest] = split(std::move(node->left), left_size);
			node->left = std::move(rest);
			pull(*node);
			return {std::move(left), std::move(node)};
		}
		auto [rest, right] = split(std::move(node->right), left_size - position);
		node->right = std::move(rest);
		pull(*node);
		return {std::move(node), std::move(right)};
	}

	static Pointer merge(Pointer left, Pointer right) {
		if (!left) return right;
		if (!right) return left;
		if (left->priority > right->priority) {
			push(*left);
			left->right = merge(std::move(left->right), std::move(right));
			pull(*left);
			return left;
		}
		push(*right);
		right->left = merge(std::move(left), std::move(right->left));
		pull(*right);
		return right;
	}

	// Adds a leaf while maintaining the right spine of the Cartesian tree.
	void build_leaf(value_type value, std::vector<Node *> &spine) {
		auto leaf = std::make_unique<Node>(std::move(value), next_priority());
		Node *position = leaf.get();
		while (!spine.empty() && spine.back()->priority <= leaf->priority) {
			pull(*spine.back());
			spine.pop_back();
		}
		Pointer &owner = spine.empty() ? root : spine.back()->right;
		leaf->left = std::move(owner);
		owner = std::move(leaf);
		spine.push_back(position);
	}

	static void finish_build(const std::vector<Node *> &spine) {
		for (auto position = spine.rbegin(); position != spine.rend(); ++position) pull(**position);
	}

	// A deferred ancestor action happens after the node's own action.
	template <typename Visitor>
	static void visit_values(const Pointer &node, bool reversed, const action_type *later, const Visitor &visit) {
		if (!node) return;
		std::optional<action_type> combined;
		const action_type *child_action = later;
		if (node->has_lazy) {
			if (later) {
				combined.emplace(composition(*later, node->lazy));
				child_action = std::addressof(*combined);
			} else {
				child_action = std::addressof(node->lazy);
			}
		}
		const bool reverse_child = reversed != node->reversed;
		visit_values(reversed ? node->right : node->left, reverse_child, child_action, visit);
		if (later)
			visit(mapping(*later, node->value));
		else
			visit(node->value);
		visit_values(reversed ? node->left : node->right, reverse_child, child_action, visit);
	}

	template <typename Operation>
	void modify_range(int left, int right, const Operation &operation) {
		if (left > right) return;
		assert(1 <= left && right <= size());
		auto [prefix, remaining] = split(std::move(root), left - 1);
		auto [middle, suffix] = split(std::move(remaining), right - left + 1);
		operation(middle);
		root = merge(merge(std::move(prefix), std::move(middle)), std::move(suffix));
	}

	Pointer extract(int position) {
		assert(1 <= position && position <= size());
		auto [prefix, remaining] = split(std::move(root), position - 1);
		auto extracted = split(std::move(remaining), 1);
		root = merge(std::move(prefix), std::move(extracted.second));
		return std::move(extracted.first);
	}

	static void set(Node &node, int position, const value_type &value) {
		push(node);
		const int root_position = subtree_size(node.left) + 1;
		if (position < root_position)
			set(*node.left, position, value);
		else if (position > root_position)
			set(*node.right, position - root_position, value);
		else
			node.value = value;
		pull(node);
	}

	static value_type product(Node &node, int left, int right) {
		if (left == 1 && right == node.size) return node.product;
		push(node);
		const int position = subtree_size(node.left) + 1;
		if (right < position) return product(*node.left, left, right);
		if (position < left) return product(*node.right, left - position, right - position);
		value_type result = node.value;
		if (left < position) result = ActedMonoid::combine(product(*node.left, left, position - 1), result);
		if (position < right) result = ActedMonoid::combine(result, product(*node.right, 1, right - position));
		return result;
	}

       public:
	ImplicitTreap() = default;

	// Linear build. Slot 0 is ignored.
	explicit ImplicitTreap(const std::vector<value_type> &values) {
		assert(!values.empty());
		std::vector<Node *> spine;
		for (std::size_t i = 1; i < values.size(); ++i) build_leaf(values[i], spine);
		finish_build(spine);
	}

	template <typename T>
		requires(!std::is_same_v<T, value_type> && requires(const T &value) { ActedMonoid::make_leaf(value); })
	explicit ImplicitTreap(const std::vector<T> &values) : ImplicitTreap(from_values(values)) {}

	// Copies own their nodes and draw new priorities for later concatenation.
	ImplicitTreap(const ImplicitTreap &other) {
		std::vector<Node *> spine;
		visit_values(other.root, false, nullptr, [&](const value_type &value) { build_leaf(value, spine); });
		finish_build(spine);
	}

	ImplicitTreap &operator=(const ImplicitTreap &other) {
		if (this != &other) {
			ImplicitTreap copy(other);
			root = std::move(copy.root);
		}
		return *this;
	}
	ImplicitTreap(ImplicitTreap &&) noexcept = default;
	ImplicitTreap &operator=(ImplicitTreap &&) noexcept = default;

	template <typename T>
		requires requires(const T &value) { ActedMonoid::make_leaf(value); }
	static ImplicitTreap from_values(const std::vector<T> &values) {
		assert(!values.empty());
		ImplicitTreap tree;
		std::vector<Node *> spine;
		for (std::size_t i = 1; i < values.size(); ++i) tree.build_leaf(ActedMonoid::make_leaf(values[i]), spine);
		finish_build(spine);
		return tree;
	}

	int size() const { return subtree_size(root); }
	bool empty() const { return !root; }
	void clear() { root.reset(); }

	void insert(int position, const value_type &value) {
		assert(1 <= position && position <= size() + 1);
		auto leaf = std::make_unique<Node>(value, next_priority());
		auto [left, right] = split(std::move(root), position - 1);
		root = merge(merge(std::move(left), std::move(leaf)), std::move(right));
	}

	template <typename T>
		requires requires(const T &value) { ActedMonoid::make_leaf(value); }
	void insert_value(int position, const T &value) {
		insert(position, ActedMonoid::make_leaf(value));
	}

	void push_front(const value_type &value) { insert(1, value); }
	void push_back(const value_type &value) { insert(size() + 1, value); }
	void erase(int position) { extract(position); }

	std::optional<value_type> pop_front() {
		if (empty()) return std::nullopt;
		return std::move(extract(1)->value);
	}
	std::optional<value_type> pop_back() {
		if (empty()) return std::nullopt;
		return std::move(extract(size())->value);
	}

	value_type get(int position) {
		assert(1 <= position && position <= size());
		return product(*root, position, position);
	}

	void set(int position, const value_type &value) {
		assert(1 <= position && position <= size());
		set(*root, position, value);
	}

	template <typename T>
		requires requires(const T &value) { ActedMonoid::make_leaf(value); }
	void set_value(int position, const T &value) {
		set(position, ActedMonoid::make_leaf(value));
	}

	void reverse(int left, int right) {
		modify_range(left, right, [](const Pointer &node) { reverse_node(node); });
	}
	void apply(int left, int right, const action_type &action) {
		modify_range(left, right, [&](const Pointer &node) { apply_node(node, action); });
	}

	value_type product(int left, int right) {
		if (left > right) return identity();
		assert(1 <= left && right <= size());
		return product(*root, left, right);
	}

	value_type all_product() const { return product(root); }

	// Keeps positions [1, position - 1] and returns the suffix as its own 1-based sequence.
	ImplicitTreap split_off(int position) {
		assert(1 <= position && position <= size() + 1);
		auto [left, right] = split(std::move(root), position - 1);
		root = std::move(left);
		ImplicitTreap suffix;
		suffix.root = std::move(right);
		return suffix;
	}

	// Moves an rvalue suffix in expected O(log n). Passing an lvalue first makes a linear copy.
	void append(ImplicitTreap suffix) { root = merge(std::move(root), std::move(suffix.root)); }

	std::vector<value_type> to_vector() const {
		std::vector<value_type> values;
		values.reserve(size() + 1);
		values.push_back(identity());
		visit_values(root, false, nullptr, [&](const value_type &value) { values.push_back(value); });
		return values;
	}
};
