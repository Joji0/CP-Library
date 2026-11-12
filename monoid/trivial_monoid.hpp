#pragma once

struct TrivialMonoid {
	struct value_type {
		bool operator==(const value_type &) const = default;
	};
	static value_type identity() { return {}; }
	static value_type combine(value_type, value_type) { return {}; }
};
