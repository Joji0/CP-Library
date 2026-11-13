#pragma once
#include "acted_monoid/binary_inversions_flip.hpp"
#include "monoid/detail/checked_arithmetic.hpp"

namespace acted_monoid_detail {
struct BinaryInversionAggregate : BinaryInversions {
	using Value = value_type;
	static Value assign(bool bit, const Value &value) {
		const long long count = monoid_detail::checked_add(value.zeros, value.ones);
		return bit ? Value{0, count, 0} : Value{count, 0, 0};
	}
	static Value flip(Value value) { return BinaryInversionsFlip::mapping(true, value); }
};
}  // namespace acted_monoid_detail
