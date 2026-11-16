#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <vector>
#include "mathematics/number_theory/integer_kth_root.hpp"

class SegmentedSieve {
	long long n;
	std::vector<int> small_primes;
	static constexpr int BLOCK = 1 << 15;

	void sieve_block(long long lo, long long hi, std::vector<char> &buf) const {
		assert(0 <= lo && lo <= hi && hi <= n);
		assert(hi - lo < std::numeric_limits<int>::max());
		int len = (int)(hi - lo + 1);
		assert((int)buf.size() >= len);
		std::memset(buf.data(), 1, len);
		if (lo == 0) buf[0] = 0;
		if (lo <= 1 && 1 <= hi) buf[(int)(1 - lo)] = 0;
		for (int p : small_primes) {
			__int128_t first = (__int128_t)(lo / p + (lo % p != 0)) * p;
			long long start = std::max((long long)p * p, first > hi ? hi + 1 : (long long)first);
			for (long long j = start; j <= hi; j += p) {
				buf[(int)(j - lo)] = 0;
			}
		}
	}

       public:
	SegmentedSieve(long long n = 0) { initialize(n); }

	void initialize(long long n) {
		assert(n >= 0);
		assert(n <= (long long)(std::numeric_limits<int>::max() - 1) * (std::numeric_limits<int>::max() - 1));
		this->n = n;
		small_primes.clear();
		const int sq = int(integer_kth_root(n, 2));
		std::vector<char> is_p(sq + 1, 1);
		for (int i = 2; i <= sq; i++) {
			if (is_p[i]) {
				small_primes.push_back(i);
				for (long long j = (long long)i * i; j <= sq; j += i) is_p[j] = 0;
			}
		}
	}

	bool is_prime(long long x) const {
		if (x < 2) return false;
		assert(x <= n);
		for (int p : small_primes) {
			if ((long long)p * p > x) break;
			if (x % p == 0) return x == p;
		}
		return true;
	}

	std::vector<char> sieve_range(long long lo, long long hi) const {
		assert(0 <= lo && lo <= hi && hi <= n);
		std::vector<char> buf(std::size_t(hi - lo) + 1);
		std::vector<char> block(BLOCK);
		for (long long start = lo; start <= hi; start += BLOCK) {
			const long long end = std::min(hi, start + BLOCK - 1);
			sieve_block(start, end, block);
			std::copy_n(block.begin(), end - start + 1, buf.begin() + (start - lo));
		}
		return buf;
	}

	std::vector<long long> primes_in_range(long long lo, long long hi) const {
		auto is_p = sieve_range(lo, hi);
		std::vector<long long> res;
		for (std::size_t i = 0; i < is_p.size(); ++i) {
			if (is_p[i]) res.push_back(lo + static_cast<long long>(i));
		}
		return res;
	}

	long long count_primes(long long n) const {
		if (n < 2) return 0;
		assert(n <= this->n);
		long long cnt = 0;
		std::vector<char> buf(BLOCK);
		for (long long lo = 2; lo <= n; lo += BLOCK) {
			long long hi = std::min(n, lo + BLOCK - 1);
			sieve_block(lo, hi, buf);
			for (long long i = lo; i <= hi; i++) {
				if (buf[(int)(i - lo)]) cnt++;
			}
		}
		return cnt;
	}
};
