#pragma once
#include <optional>
#include "acted_monoid/sum_add_assign.hpp"
#include "acted_monoid/sum_affine.hpp"
#include "monoid/detail/checked_arithmetic.hpp"

namespace acted_monoid_detail {
enum class NumericUpdate { add,
			   multiply,
			   assign,
			   add_assign,
			   affine };

template <typename T, NumericUpdate Update>
struct NumericAction;

template <typename T>
struct NumericAction<T, NumericUpdate::add> {
	using Action = T;
	static typename SumAffine<T>::Action coefficients(const Action &action) { return {T(1), action}; }
	static Action composition(const Action &later, const Action &earlier) { return monoid_detail::checked_add(later, earlier); }
	static Action identity_action() { return T(0); }
};

template <typename T>
struct NumericAction<T, NumericUpdate::multiply> {
	using Action = T;
	static typename SumAffine<T>::Action coefficients(const Action &action) { return {action, T(0)}; }
	static Action composition(const Action &later, const Action &earlier) { return monoid_detail::checked_multiply(later, earlier); }
	static Action identity_action() { return T(1); }
};

template <typename T>
struct NumericAction<T, NumericUpdate::assign> {
	using Action = std::optional<T>;
	static typename SumAffine<T>::Action coefficients(const Action &action) { return action ? typename SumAffine<T>::Action{T(0), *action} : typename SumAffine<T>::Action{T(1), T(0)}; }
	static Action composition(const Action &later, const Action &earlier) { return later ? later : earlier; }
	static Action identity_action() { return std::nullopt; }
};

template <typename T>
struct NumericAction<T, NumericUpdate::add_assign> {
	using Base = SumAddAssign<T>;
	using Action = typename Base::Action;
	static typename SumAffine<T>::Action coefficients(const Action &action) {
		if (action.set_val) return {T(0), monoid_detail::checked_add(*action.set_val, action.add_val)};
		return {T(1), action.add_val};
	}
	static Action composition(const Action &later, const Action &earlier) { return Base::composition(later, earlier); }
	static Action identity_action() { return Base::identity_action(); }
	static Action make_set(const T &value) { return Base::make_set(value); }
	static Action make_add(const T &value) { return Base::make_add(value); }
};

template <typename T>
struct NumericAction<T, NumericUpdate::affine> {
	using Base = SumAffine<T>;
	using Action = typename Base::Action;
	static Action coefficients(const Action &action) { return action; }
	static Action composition(const Action &later, const Action &earlier) { return Base::composition(later, earlier); }
	static Action identity_action() { return Base::identity_action(); }
};
}  // namespace acted_monoid_detail
