#pragma once
#include <utility>
#include "monoid/binary_runs.hpp"

namespace acted_monoid_detail {
struct BinaryRunAggregate : BinaryRuns {
	using Value = value_type;
	static Value assign(bool bit, const Value &value) { return repeated(bit, value.count); }
	static Value flip(Value value) {
		std::swap(value.zero, value.one);
		return value;
	}
};
}  // namespace acted_monoid_detail
