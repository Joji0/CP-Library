#pragma once
#include <cstddef>
#include <limits>
#include <optional>
#include <type_traits>
#include "monoid/bitwise_function.hpp"
#include "monoid/detail/checked_arithmetic.hpp"

namespace acted_monoid_detail {
template <typename T>
inline constexpr std::size_t bit_width = std::numeric_limits<std::make_unsigned_t<T>>::digits;

template <typename T, std::size_t Bits>
struct BitwiseDomain {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool> && sizeof(T) <= sizeof(long long));
	static_assert(0 < Bits && Bits <= bit_width<T>);
	using Unsigned = std::make_unsigned_t<T>;
	static constexpr Unsigned mask = std::numeric_limits<Unsigned>::max() >> (bit_width<T> - Bits);

	// A reduced width represents nonnegative inputs. Update masks use their low Bits bits.
	static Unsigned encode(T value) {
		const Unsigned bits = static_cast<Unsigned>(value);
		if (bits > mask) monoid_detail::arithmetic_failure();
		return bits;
	}
};

enum class BitwiseUpdate { bitwise_and,
			   bitwise_or,
			   exclusive_or,
			   assign,
			   bitwise_not,
			   function };

template <typename T, std::size_t Bits, BitwiseUpdate Update>
struct BitwiseAction {
	using Domain = BitwiseDomain<T, Bits>;
	using Unsigned = typename Domain::Unsigned;
	using Function = BitwiseFunction<Unsigned>;
	using TruthTable = typename Function::value_type;
	using Action = std::conditional_t<Update == BitwiseUpdate::assign, std::optional<T>,
					  std::conditional_t<Update == BitwiseUpdate::bitwise_not, bool,
							     std::conditional_t<Update == BitwiseUpdate::function, TruthTable, Unsigned>>>;
	static constexpr Unsigned mask = Domain::mask;

	static TruthTable make_and(Unsigned bits) { return {0, Unsigned(bits & mask)}; }
	static TruthTable make_or(Unsigned bits) { return {Unsigned(bits & mask), mask}; }
	static TruthTable make_exclusive_or(Unsigned bits) { return {Unsigned(bits & mask), Unsigned(mask & ~bits)}; }
	static TruthTable make_assign(T value) {
		const Unsigned bits = Domain::encode(value);
		return {bits, bits};
	}
	static TruthTable make_not() { return {mask, 0}; }

	static TruthTable truth_table(Action action) {
		if constexpr (Update == BitwiseUpdate::bitwise_and) return make_and(action);
		if constexpr (Update == BitwiseUpdate::bitwise_or) return make_or(action);
		if constexpr (Update == BitwiseUpdate::exclusive_or) return make_exclusive_or(action);
		if constexpr (Update == BitwiseUpdate::assign) return action ? make_assign(*action) : TruthTable{0, mask};
		if constexpr (Update == BitwiseUpdate::bitwise_not) return action ? make_not() : TruthTable{0, mask};
		if constexpr (Update == BitwiseUpdate::function) return {Unsigned(action.from_zero & mask), Unsigned(action.from_one & mask)};
	}

	static Action composition(Action later, Action earlier) {
		if constexpr (Update == BitwiseUpdate::bitwise_and) return later & earlier;
		if constexpr (Update == BitwiseUpdate::bitwise_or) return later | earlier;
		if constexpr (Update == BitwiseUpdate::exclusive_or) return later ^ earlier;
		if constexpr (Update == BitwiseUpdate::assign) return later ? later : earlier;
		if constexpr (Update == BitwiseUpdate::bitwise_not) return later != earlier;
		if constexpr (Update == BitwiseUpdate::function) return Function::combine(later, earlier);
	}
	static Action identity_action() {
		if constexpr (Update == BitwiseUpdate::bitwise_and) return Function::identity().from_one;
		if constexpr (Update == BitwiseUpdate::assign) return std::nullopt;
		if constexpr (Update == BitwiseUpdate::function) return Function::identity();
		return {};
	}
};
}  // namespace acted_monoid_detail
