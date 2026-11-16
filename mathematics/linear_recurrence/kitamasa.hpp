#pragma once
#include <cassert>
#include <cstdint>
#include <vector>

// Linear recurrence over a ring. Unlike the convolution variant this needs no
// inverses or modular coefficient type. O(d^2 log(index+1)) time.
template <typename Value>
Value kitamasa(const std::vector<Value>& initial, const std::vector<Value>& coefficients, uint64_t index) {
	assert(initial.size() == coefficients.size());
	const int order = int(initial.size());
	if (index < initial.size()) return initial[index];
	if (order == 0) return Value(0);
	auto multiply = [&](const std::vector<Value>& left, const std::vector<Value>& right) {
		std::vector<Value> product(2 * order - 1, Value(0));
		for (int i = 0; i < order; ++i) {
			for (int j = 0; j < order; ++j) product[i + j] += left[i] * right[j];
		}
		for (int i = 2 * order - 2; i >= order; --i) {
			for (int j = 0; j < order; ++j) product[i - j - 1] += product[i] * coefficients[j];
		}
		product.resize(order);
		return product;
	};
	std::vector<Value> power(order, Value(0)), result(order, Value(0));
	result[0] = Value(1);
	if (order == 1)
		power[0] = coefficients[0];
	else
		power[1] = Value(1);
	for (; index != 0; index >>= 1) {
		if (index & 1) result = multiply(result, power);
		if (index > 1) power = multiply(power, power);
	}
	Value answer = 0;
	for (int i = 0; i < order; ++i) answer += result[i] * initial[i];
	return answer;
}
