#pragma once

// Symmetries x -> rotation +/- x on a cycle of Order positions.
// combine(later, earlier) applies earlier first.
template <long long Order>
struct Dihedral {
	static_assert(Order > 0);
	struct value_type {
		long long rotation;
		bool reflected;
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {0, false}; }
	static value_type make_rotation(long long offset) { return {normalize(offset), false}; }
	static value_type make_reflection(long long offset = 0) { return {normalize(offset), true}; }

	static value_type combine(value_type later, value_type earlier) {
		const __int128 offset = earlier.rotation;
		return {normalize(__int128(later.rotation) + (later.reflected ? -offset : offset)),
			later.reflected != earlier.reflected};
	}

	static long long evaluate(value_type transform, long long position) {
		const __int128 offset = position;
		return normalize(__int128(transform.rotation) + (transform.reflected ? -offset : offset));
	}

       private:
	static long long normalize(__int128 offset) {
		offset %= Order;
		if (offset < 0) offset += Order;
		return static_cast<long long>(offset);
	}
};
