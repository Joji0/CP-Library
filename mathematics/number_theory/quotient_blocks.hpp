#pragma once
#include <cstdint>
#include <vector>

struct QuotientBlock {
	uint64_t left;
	uint64_t right;
	uint64_t quotient;
};

// Visit maximal inclusive ranges [left,right] in [1,n] with constant floor(n/i).
// O(sqrt(n)) time, O(1) auxiliary memory.
template <typename Callback>
void for_each_quotient_block(uint64_t n, Callback&& callback) {
	for (uint64_t left = 1; left <= n;) {
		const uint64_t quotient = n / left;
		const uint64_t right = n / quotient;
		callback(QuotientBlock{left, right, quotient});
		if (right == n) break;
		left = right + 1;
	}
}

inline std::vector<QuotientBlock> quotient_blocks(uint64_t n) {
	std::vector<QuotientBlock> result;
	for_each_quotient_block(n, [&](QuotientBlock block) { result.push_back(block); });
	return result;
}
