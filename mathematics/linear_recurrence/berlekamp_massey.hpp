#pragma once
#include <vector>

// Shortest linear recurrence explaining the given observations over a field.
// Terms are indexed from a[0]; result[j] multiplies a[n-j-1]. O(n^2).
template <typename Field>
std::vector<Field> berlekamp_massey(const std::vector<Field>& sequence) {
	std::vector<Field> active{Field(1)}, previous{Field(1)};
	Field last_discrepancy = 1;
	int length = 0, shift = 1;
	for (int i = 0; i < int(sequence.size()); ++i, ++shift) {
		Field discrepancy = sequence[i];
		for (int j = 1; j <= length; ++j) discrepancy += active[j] * sequence[i - j];
		if (discrepancy == Field(0)) continue;
		const Field multiplier = discrepancy / last_discrepancy;
		const bool increase = length <= i - length;
		std::vector<Field> old;
		if (increase) old = active;
		if (active.size() < previous.size() + shift) active.resize(previous.size() + shift, Field(0));
		for (int j = 0; j < int(previous.size()); ++j) active[j + shift] -= multiplier * previous[j];
		if (increase) {
			length = i + 1 - length;
			previous.swap(old);
			last_discrepancy = discrepancy;
			shift = 0;
		}
	}
	std::vector<Field> result(length);
	for (int j = 0; j < length; ++j) result[j] = -active[j + 1];
	return result;
}
