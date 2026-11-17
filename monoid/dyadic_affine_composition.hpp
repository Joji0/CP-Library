#pragma once
#include <algorithm>
#include <limits>
#include <optional>

// https://atcoder.jp/contests/aalc001/tasks/aalc001_w
// Partial integer maps x -> (offset +/- x) / 2, defined only when the result
// is integral. Inputs and all intermediate results are restricted to [Lower, Upper].
// A product represents a progression of valid inputs with output changing by +/-1.
template <long long Lower, long long Upper>
struct DyadicAffineComposition {
	static_assert(Lower <= Upper && __int128(Upper) - Lower <= std::numeric_limits<long long>::max());

	struct value_type {
		long long first_input = 0, last_input = 0, first_output = 0, stride = 0;
		bool decreasing = false;
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {Lower, Upper, Lower, 1, false}; }

	static value_type make_half(long long offset, bool negate_input = false) {
		const __int128 first_allowed = negate_input ? __int128(offset) - 2 * __int128(Upper) : 2 * __int128(Lower) - offset;
		const __int128 last_allowed = negate_input ? __int128(offset) - 2 * __int128(Lower) : 2 * __int128(Upper) - offset;
		__int128 first = std::max(__int128(Lower), first_allowed);
		__int128 last = std::min(__int128(Upper), last_allowed);
		first += (first - offset) % 2 != 0;
		last -= (last - offset) % 2 != 0;
		if (first > last) return {};
		return make_progression(first, last, (__int128(offset) + (negate_input ? -first : first)) / 2, 2, negate_input);
	}

	static value_type combine(const value_type &left, const value_type &right) {
		if (left.stride == 0 || right.stride == 0) return {};
		// An input left.first_input + left.stride*k reaches
		// left.first_output +/- k before the right map.
		const __int128 maximum_index = (__int128(left.last_input) - left.first_input) / left.stride;
		const __int128 first_allowed = left.decreasing ? __int128(left.first_output) - right.last_input
							       : __int128(right.first_input) - left.first_output;
		const __int128 last_allowed = left.decreasing ? __int128(left.first_output) - right.first_input
							      : __int128(right.last_input) - left.first_output;
		const __int128 residue = left.decreasing ? __int128(left.first_output) - right.first_input
							 : __int128(right.first_input) - left.first_output;
		__int128 first = std::max(__int128(0), first_allowed);
		__int128 last = std::min(maximum_index, last_allowed);
		first += remainder(residue - first, right.stride);
		last -= remainder(last - residue, right.stride);
		if (first > last) return {};
		const __int128 first_input = __int128(left.first_input) + left.stride * first;
		const __int128 last_input = __int128(left.first_input) + left.stride * last;
		const __int128 middle = __int128(left.first_output) + (left.decreasing ? -first : first);
		const __int128 index_in_right = (middle - right.first_input) / right.stride;
		const __int128 output = __int128(right.first_output) + (right.decreasing ? -index_in_right : index_in_right);
		return make_progression(first_input, last_input, output, __int128(left.stride) * right.stride, left.decreasing != right.decreasing);
	}

	static std::optional<long long> evaluate(const value_type &value, long long input) {
		if (value.stride == 0 || input < value.first_input || value.last_input < input) return std::nullopt;
		const long long offset = input - value.first_input;
		if (offset % value.stride != 0) return std::nullopt;
		const long long index = offset / value.stride;
		return static_cast<long long>(__int128(value.first_output) + (value.decreasing ? -index : index));
	}

	static std::optional<long long> fixed_point(const value_type &value) {
		if (value.stride == 0) return std::nullopt;
		const __int128 difference = __int128(value.first_output) - value.first_input;
		const __int128 scale = __int128(value.stride) + (value.decreasing ? 1 : -1);
		if (scale == 0) return difference == 0 ? std::optional<long long>(value.first_input) : std::nullopt;
		if (difference % scale != 0) return std::nullopt;
		const __int128 index = difference / scale;
		if (index < 0 || index > (__int128(value.last_input) - value.first_input) / value.stride) return std::nullopt;
		return static_cast<long long>(__int128(value.first_input) + index * value.stride);
	}

       private:
	static __int128 remainder(__int128 value, long long modulus) {
		const __int128 result = value % modulus;
		return result < 0 ? result + modulus : result;
	}
	static value_type make_progression(__int128 first, __int128 last, __int128 output, __int128 stride, bool decreasing) {
		const bool singleton = first == last;
		return {static_cast<long long>(first), static_cast<long long>(last), static_cast<long long>(output),
			singleton ? 1 : static_cast<long long>(stride), !singleton && decreasing};
	}
};
