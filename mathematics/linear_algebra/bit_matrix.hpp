#pragma once
#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

// BitMatrix (GF(2) matrix) packed in 64-bit words for 64x speedup in CP.
class BitMatrix {
	int _rows;
	int _cols;
	int _blocks;
	std::vector<uint64_t> _data;

	static int block_count(int cols) {
		assert(cols >= 0);
		return cols / 64 + (cols % 64 != 0);
	}

       public:
	BitMatrix() : _rows(0), _cols(0), _blocks(0) {}
	BitMatrix(int rows, int cols)
	    : _rows(rows), _cols(cols), _blocks(block_count(cols)), _data((std::size_t(rows) + 1) * std::size_t(_blocks), 0) {}

	int rows() const { return _rows; }
	int columns() const { return _cols; }
	int blocks() const { return _blocks; }
	bool empty() const { return _rows == 0 || _cols == 0; }

	bool get(int r, int c) const {
		assert(1 <= r && r <= _rows && 1 <= c && c <= _cols);
		return (_data[std::size_t(r) * _blocks + ((c - 1) / 64)] >> ((c - 1) % 64)) & 1;
	}

	void set(int r, int c, bool val) {
		assert(1 <= r && r <= _rows && 1 <= c && c <= _cols);
		std::size_t idx = std::size_t(r) * _blocks + ((c - 1) / 64);
		uint64_t mask = 1ULL << ((c - 1) % 64);
		if (val)
			_data[idx] |= mask;
		else
			_data[idx] &= ~mask;
	}

	void flip(int r, int c) {
		assert(1 <= r && r <= _rows && 1 <= c && c <= _cols);
		_data[std::size_t(r) * _blocks + ((c - 1) / 64)] ^= (1ULL << ((c - 1) % 64));
	}

	static BitMatrix identity(int size) {
		BitMatrix res(size, size);
		for (int i = 1; i <= size; ++i) res.set(i, i, true);
		return res;
	}

	BitMatrix operator*(const BitMatrix &other) const {
		assert(_cols == other._rows);
		BitMatrix res(_rows, other._cols);
		for (int i = 1; i <= _rows; ++i) {
			for (int k = 1; k <= _cols; ++k) {
				if (!get(i, k)) continue;
				for (int b = 0; b < other._blocks; ++b) {
					res._data[std::size_t(i) * res._blocks + b] ^= other._data[std::size_t(k) * other._blocks + b];
				}
			}
		}
		return res;
	}

	// Reduced row echelon form over F2, with packed 64-bit row operations
	int gaussian_elimination() {
		int rank = 1;
		for (int col = 1; col <= _cols && rank <= _rows; ++col) {
			int pivot = rank;
			for (int r = rank; r <= _rows; ++r) {
				if (get(r, col)) {
					pivot = r;
					break;
				}
			}

			if (!get(pivot, col)) continue;

			if (pivot != rank) {
				for (int b = 0; b < _blocks; ++b) {
					std::swap(_data[std::size_t(rank) * _blocks + b], _data[std::size_t(pivot) * _blocks + b]);
				}
			}

			for (int r = 1; r <= _rows; ++r) {
				if (r != rank && get(r, col)) {
					for (int b = 0; b < _blocks; ++b) {
						_data[std::size_t(r) * _blocks + b] ^= _data[std::size_t(rank) * _blocks + b];
					}
				}
			}
			rank++;
		}
		return rank - 1;
	}

	int rank() const {
		BitMatrix copy = *this;
		return copy.gaussian_elimination();
	}

	// Solve linear system Ax = b over GF(2)
	static std::optional<std::vector<bool>> solve(BitMatrix a, std::vector<bool> b) {
		int n = a.rows();
		int m = a.columns();
		assert(int(b.size()) == n + 1);

		BitMatrix aug(n, m + 1);
		for (int r = 1; r <= n; ++r) {
			for (int c = 1; c <= m; ++c) {
				if (a.get(r, c)) aug.set(r, c, true);
			}
			if (b[r]) aug.set(r, m + 1, true);
		}

		aug.gaussian_elimination();

		std::vector<bool> x(m + 1, false);
		std::vector<int> pivot_col(n + 1, -1);

		for (int r = 1; r <= n; ++r) {
			for (int c = 1; c <= m; ++c) {
				if (aug.get(r, c)) {
					pivot_col[r] = c;
					break;
				}
			}
			if (pivot_col[r] == -1 && aug.get(r, m + 1)) {
				return std::nullopt;  // Inconsistent
			}
		}

		for (int r = 1; r <= n; ++r) {
			if (pivot_col[r] != -1) {
				x[pivot_col[r]] = aug.get(r, m + 1);
			}
		}
		return x;
	}
};
