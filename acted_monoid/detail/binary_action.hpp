#pragma once
#include <optional>
#include <type_traits>

namespace acted_monoid_detail {
enum class BinaryUpdate { flip,
			  assign,
			  function };

// Each transform is a Boolean truth table, with composition in application order.
template <typename Aggregate, BinaryUpdate Update>
struct BinaryAction : Aggregate {
	using Value = typename Aggregate::Value;
	struct TruthTable {
		bool from_zero, from_one;
		bool operator==(const TruthTable &) const = default;
	};
	using Action = std::conditional_t<Update == BinaryUpdate::flip, bool,
					  std::conditional_t<Update == BinaryUpdate::assign, std::optional<bool>, TruthTable>>;

	static TruthTable make_flip() { return {true, false}; }
	static TruthTable make_assign(bool bit) { return {bit, bit}; }
	static TruthTable truth_table(Action action) {
		if constexpr (Update == BinaryUpdate::flip) return {action, !action};
		if constexpr (Update == BinaryUpdate::assign) return action ? make_assign(*action) : TruthTable{false, true};
		if constexpr (Update == BinaryUpdate::function) return action;
	}
	static Value mapping(Action action, Value value) {
		const auto [from_zero, from_one] = truth_table(action);
		if (from_zero == from_one) return Aggregate::assign(from_zero, value);
		if (from_zero) return Aggregate::flip(value);
		return value;
	}
	static Action composition(Action later, Action earlier) {
		if constexpr (Update == BinaryUpdate::flip) return later != earlier;
		if constexpr (Update == BinaryUpdate::assign) return later ? later : earlier;
		if constexpr (Update == BinaryUpdate::function) {
			return {earlier.from_zero ? later.from_one : later.from_zero, earlier.from_one ? later.from_one : later.from_zero};
		}
	}
	static Action identity_action() {
		if constexpr (Update == BinaryUpdate::flip) return false;
		if constexpr (Update == BinaryUpdate::assign) return std::nullopt;
		if constexpr (Update == BinaryUpdate::function) return {false, true};
	}
};
}  // namespace acted_monoid_detail
