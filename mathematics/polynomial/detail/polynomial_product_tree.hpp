#pragma once
#include <cassert>
#include <utility>
#include <vector>
#include "mathematics/polynomial/polynomial_division.hpp"

namespace polynomial_detail {

template <typename Modular>
class PolynomialProductTree {
	using Series = FormalPowerSeries<Modular>;
	int count;
	std::vector<Series> product;

	const Series& build(int node, int left, int right, const std::vector<Modular>& points) {
		if (left == right) {
			product[node] = Series{-points[left], Modular(1)};
		} else {
			const int middle = left + (right - left) / 2;
			build(node * 2, left, middle, points);
			build(node * 2 + 1, middle + 1, right, points);
			product[node] = product[node * 2] * product[node * 2 + 1];
		}
		return product[node];
	}

	void evaluate(int node, int left, int right, const Series& coefficients, std::vector<Modular>& values) const {
		if (coefficients.empty()) return;
		if (left == right) {
			values[left] = coefficients.evaluate(-product[node][0]);
			return;
		}
		const int middle = left + (right - left) / 2;
		evaluate(node * 2, left, middle, polynomial_division(coefficients, product[node * 2]).second, values);
		evaluate(node * 2 + 1, middle + 1, right, polynomial_division(coefficients, product[node * 2 + 1]).second, values);
	}

	Series interpolate(int node, int left, int right, const std::vector<Modular>& weights) const {
		if (left == right) return Series{weights[left]};
		const int middle = left + (right - left) / 2;
		return interpolate(node * 2, left, middle, weights) * product[node * 2 + 1] +
		       interpolate(node * 2 + 1, middle + 1, right, weights) * product[node * 2];
	}

       public:
	explicit PolynomialProductTree(const std::vector<Modular>& points)
	    : count(int(points.size()) - 1), product(4 * points.size()) {
		assert(!points.empty());
		if (count != 0) build(1, 1, count, points);
	}

	std::vector<Modular> values(const Series& coefficients) const {
		std::vector<Modular> result(count + 1);
		if (count != 0) evaluate(1, 1, count, polynomial_division(coefficients, product[1]).second, result);
		return result;
	}

	Series interpolate(const std::vector<Modular>& values_at_points) const {
		assert(int(values_at_points.size()) == count + 1);
		if (count == 0) return {};
		auto derivatives = values(product[1].derivative());
		for (int i = 1; i <= count; ++i) derivatives[i] = values_at_points[i] / derivatives[i];
		return interpolate(1, 1, count, derivatives);
	}
};

}  // namespace polynomial_detail
