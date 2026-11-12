#pragma once
#include <optional>

namespace union_find_detail {
template <typename Base>
class ParityConstraints : public Base {
       public:
	using Base::Base;

	bool constrain_same(int first, int second) { return this->merge(first, second, false); }
	bool constrain_opposite(int first, int second) { return this->merge(first, second, true); }

	std::optional<bool> try_same_color(int first, int second) {
		const auto parity = this->try_difference(first, second);
		if (!parity) return std::nullopt;
		return !*parity;
	}
	std::optional<bool> try_same_color(int first, int second) const {
		const auto parity = this->try_difference(first, second);
		if (!parity) return std::nullopt;
		return !*parity;
	}
};
}  // namespace union_find_detail
