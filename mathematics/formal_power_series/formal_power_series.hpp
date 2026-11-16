#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>
#include "mathematics/modular_arithmetic/static_modular_integer.hpp"
#include "mathematics/convolution/convolution.hpp"

// Coefficient i belongs to x^i. The precision argument is the number of coefficients.
// Logarithm, exponential and power require the relevant integer denominators to be units.
template <typename Mint = StaticModularInteger<998244353>>
class FormalPowerSeries : public std::vector<Mint> {
       public:
	using std::vector<Mint>::vector;
	using Series = FormalPowerSeries<Mint>;

	FormalPowerSeries() = default;
	FormalPowerSeries(const std::vector<Mint> &v) : std::vector<Mint>(v) {}
	FormalPowerSeries(std::vector<Mint> &&v) : std::vector<Mint>(std::move(v)) {}

	Series &shrink() {
		while (!this->empty() && this->back() == Mint(0)) this->pop_back();
		return *this;
	}

	Series prefix(int deg) const {
		assert(deg >= 0);
		Series res(this->begin(), this->begin() + std::min<int>(deg, int(this->size())));
		res.resize(deg, Mint(0));
		return res;
	}

	Series reversed(int sz = -1) const {
		Series res = *this;
		if (sz >= 0) res.resize(sz, Mint(0));
		std::reverse(res.begin(), res.end());
		return res;
	}

	Series &operator+=(const Series &rhs) {
		if (this->size() < rhs.size()) this->resize(rhs.size(), Mint(0));
		for (int i = 0; i < int(rhs.size()); ++i) (*this)[i] += rhs[i];
		return *this;
	}

	Series &operator-=(const Series &rhs) {
		if (this->size() < rhs.size()) this->resize(rhs.size(), Mint(0));
		for (int i = 0; i < int(rhs.size()); ++i) (*this)[i] -= rhs[i];
		return *this;
	}

	Series &operator*=(const Series &rhs) {
		std::vector<Mint> lhs(this->begin(), this->end());
		std::vector<Mint> r(rhs.begin(), rhs.end());
		*this = convolution(lhs, r);
		return *this;
	}

	Series &operator*=(Mint rhs) {
		for (Mint &v : *this) v *= rhs;
		return *this;
	}

	Series &operator/=(Mint rhs) {
		Mint inv_rhs = rhs.inverse();
		for (Mint &v : *this) v *= inv_rhs;
		return *this;
	}

	Series operator+(const Series &rhs) const { return Series(*this) += rhs; }
	Series operator-(const Series &rhs) const { return Series(*this) -= rhs; }
	Series operator*(const Series &rhs) const { return Series(*this) *= rhs; }
	Series operator*(Mint rhs) const { return Series(*this) *= rhs; }
	Series operator/(Mint rhs) const { return Series(*this) /= rhs; }

	Series derivative() const {
		int n = (int)this->size();
		if (n <= 1) return Series{};
		Series res(n - 1);
		for (int i = 1; i < n; ++i) res[i - 1] = (*this)[i] * Mint(i);
		return res;
	}

	Series integral() const {
		int n = (int)this->size();
		Series res(n + 1, Mint(0));
		for (int i = 0; i < n; ++i) res[i + 1] = (*this)[i] / Mint(i + 1);
		return res;
	}

	Series inverse(int deg = -1) const {
		if (deg == -1) deg = int(this->size());
		assert(deg >= 0);
		if (deg == 0) return {};
		assert(!this->empty() && (*this)[0] != Mint(0));
		Series result{(*this)[0].inverse()};
		for (int size = 1; size < deg;) {
			const int next = size + std::min(size, deg - size);
			auto error = (this->prefix(next) * result).prefix(next);
			// The low half is already correct, so convolve only the error above x^size.
			error.erase(error.begin(), error.begin() + size);
			auto correction = (result * error).prefix(next - size);
			result.resize(next);
			for (int i = size; i < next; ++i) result[i] = -correction[i - size];
			size = next;
		}
		return result;
	}

	Series logarithm(int deg = -1) const {
		if (deg == -1) deg = int(this->size());
		assert(deg >= 0);
		if (deg == 0) return {};
		assert(!this->empty() && (*this)[0] == Mint(1));
		if (deg == 1) return Series{Mint(0)};
		return (prefix(deg).derivative() * inverse(deg - 1)).prefix(deg - 1).integral();
	}

	Series exponential(int deg = -1) const {
		assert(this->empty() || (*this)[0] == Mint(0));
		if (deg == -1) deg = (int)this->size();
		assert(deg >= 0);
		if (deg == 0) return {};
		Series result{Mint(1)};
		for (int size = 1; size < deg;) {
			const int next = size + std::min(size, deg - size);
			auto error = this->prefix(next) - result.logarithm(next);
			error.erase(error.begin(), error.begin() + size);
			auto correction = (result * error).prefix(next - size);
			result.insert(result.end(), correction.begin(), correction.end());
			size = next;
		}
		return result;
	}

	Series power(uint64_t k, int deg = -1) const {
		if (deg == -1) deg = (int)this->size();
		assert(deg >= 0);
		if (deg == 0) return {};
		if (k == 0) {
			Series result(deg, Mint(0));
			result[0] = Mint(1);
			return result;
		}
		int n = (int)this->size();
		int shift = 0;
		while (shift < n && (*this)[shift] == Mint(0)) shift++;
		if (shift == n || (shift > 0 && k >= uint64_t((deg - 1) / shift + 1))) {
			return Series(deg, Mint(0));
		}
		const int offset = int(shift * k);
		const int precision = deg - offset;
		const Mint leading = (*this)[shift];
		Series normalized(this->begin() + shift, this->begin() + shift + std::min(n - shift, precision));
		normalized /= leading;
		Series result(deg, Mint(0));
		auto tail = (normalized.logarithm(precision) * Mint(k)).exponential(precision) * leading.power(k);
		std::copy(tail.begin(), tail.end(), result.begin() + offset);
		return result;
	}

	Mint evaluate(Mint x) const {
		Mint result = 0;
		for (auto it = this->rbegin(); it != this->rend(); ++it) result = result * x + *it;
		return result;
	}
};
