#pragma once
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector>
#include "mathematics/combinatorics.hpp"

// Batch queries sum_{k=0}^right C(n,k). queries[0] and result[0] are dummy.
// Requires n < prime modulus and odd characteristic. O(maximum*sqrt(q)+q log q).
template <typename Modular>
std::vector<Modular> binomial_coefficient_prefix_sums(const std::vector<std::pair<int, int>>& queries) {
	if (queries.empty() || Modular::get_modulus() <= 2) throw std::invalid_argument("expected dummy slot and odd prime modulus");
	int maximum = 0;
	for (std::size_t i = 1; i < queries.size(); ++i) {
		auto [n, right] = queries[i];
		if (n < 0 || n >= Modular::get_modulus()) throw std::invalid_argument("row outside [0,modulus)");
		maximum = std::max(n, maximum);
	}
	Combinatorics<Modular> combinations(maximum);
	std::vector<Modular> factorial(maximum + 1), inverse_factorial(maximum + 1), powers(maximum + 1);
	powers[0] = 1;
	for (int i = 0; i <= maximum; ++i) {
		factorial[i] = combinations.factorial(i);
		inverse_factorial[i] = combinations.inverse_factorial(i);
		if (i > 0) powers[i] = powers[i - 1] + powers[i - 1];
	}
	auto choose = [&](int n, int k) {
		if (k < 0 || k > n) return Modular(0);
		return factorial[n] * inverse_factorial[k] * inverse_factorial[n - k];
	};

	const int block_size = std::max(1, int(maximum / std::sqrt(double(queries.size()))));
	struct Query {
		int n, right, block;
		std::size_t index;
	};
	std::vector<Query> pending;
	std::vector<Modular> result(queries.size());
	for (std::size_t i = 1; i < queries.size(); ++i) {
		auto [n, right] = queries[i];
		if (right < 0) continue;
		if (right >= n)
			result[i] = powers[n];
		else
			pending.push_back({n, right, n / block_size, i});
	}
	std::sort(pending.begin(), pending.end(), [](const Query& left, const Query& right) {
		if (left.block != right.block) return left.block < right.block;
		return (left.block & 1) ? left.right > right.right : left.right < right.right;
	});
	Modular sum = 1;
	const Modular half = Modular(2).inverse();
	int n = 0, right = 0;
	for (auto query : pending) {
		while (n < query.n) {
			sum = sum + sum - choose(n, right);
			++n;
		}
		while (n > query.n) {
			--n;
			sum = (sum + choose(n, right)) * half;
		}
		while (right < query.right) sum += choose(n, ++right);
		while (right > query.right) sum -= choose(n, right--);
		result[query.index] = sum;
	}
	return result;
}
