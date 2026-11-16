#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

// High-performance Matrix class with 1D flattened contiguous storage and O(1) row access.
template <typename T>
class Matrix {
	int _rows;
	int _cols;
	std::vector<T> _data;

	static std::size_t storage_size(int rows, int cols) {
		assert(rows >= 0 && cols >= 0);
		return std::size_t(rows) * (std::size_t(cols) + 1);
	}

       public:
	using value_type = T;

	Matrix() : _rows(0), _cols(0) {}
	Matrix(int rows, int cols, const T &value = T())
	    : _rows(rows), _cols(cols), _data(storage_size(rows, cols), value) {}
	// Flat input is row-major with a single dummy element at index 0.
	Matrix(int rows, int cols, const std::vector<T> &values) : Matrix(rows, cols) {
		assert(values.size() == std::size_t(rows) * cols + 1);
		for (int r = 1; r <= rows; ++r) {
			for (int c = 1; c <= cols; ++c) {
				(*this)[r][c] = values[std::size_t(r - 1) * cols + c];
			}
		}
	}
	explicit Matrix(const std::vector<std::vector<T>> &values)
	    : _rows(int(values.size()) - 1), _cols(values.size() < 2 ? 0 : int(values[1].size()) - 1), _data(storage_size(_rows, _cols)) {
		assert(!values.empty());
		for (int r = 1; r <= _rows; ++r) {
			assert(int(values[r].size()) == _cols + 1);
			for (int c = 1; c <= _cols; ++c) {
				(*this)[r][c] = values[r][c];
			}
		}
	}

	int rows() const { return _rows; }
	int columns() const { return _cols; }
	int size() const { return _rows; }
	bool empty() const { return _rows == 0 || _cols == 0; }

	T *operator[](int row) {
		assert(1 <= row && row <= _rows);
		return _data.data() + std::size_t(row - 1) * (std::size_t(_cols) + 1);
	}

	const T *operator[](int row) const {
		assert(1 <= row && row <= _rows);
		return _data.data() + std::size_t(row - 1) * (std::size_t(_cols) + 1);
	}

	T &operator()(int row, int col) {
		assert(1 <= col && col <= _cols);
		return (*this)[row][col];
	}

	const T &operator()(int row, int col) const {
		assert(1 <= col && col <= _cols);
		return (*this)[row][col];
	}

	static Matrix identity(int size) {
		assert(size >= 0);
		Matrix res(size, size);
		for (int i = 1; i <= size; ++i) res[i][i] = T(1);
		return res;
	}

	Matrix transposed() const {
		Matrix res(_cols, _rows);
		for (int r = 1; r <= _rows; ++r) {
			for (int c = 1; c <= _cols; ++c) {
				res[c][r] = (*this)[r][c];
			}
		}
		return res;
	}

	Matrix &operator+=(const Matrix &other) {
		assert(_rows == other._rows && _cols == other._cols);
		for (std::size_t i = 0; i < _data.size(); ++i) _data[i] += other._data[i];
		return *this;
	}

	Matrix &operator-=(const Matrix &other) {
		assert(_rows == other._rows && _cols == other._cols);
		for (std::size_t i = 0; i < _data.size(); ++i) _data[i] -= other._data[i];
		return *this;
	}

	Matrix operator+(const Matrix &other) const { return Matrix(*this) += other; }
	Matrix operator-(const Matrix &other) const { return Matrix(*this) -= other; }

	Matrix operator*(const Matrix &other) const {
		assert(_cols == other._rows);
		Matrix res(_rows, other._cols, T(0));
		for (int i = 1; i <= _rows; ++i) {
			for (int k = 1; k <= _cols; ++k) {
				const T r = (*this)[i][k];
				if (r == T(0)) continue;
				for (int j = 1; j <= other._cols; ++j) {
					res[i][j] += r * other[k][j];
				}
			}
		}
		return res;
	}

	std::vector<T> multiply_vector(const std::vector<T> &values) const {
		assert(values.size() == std::size_t(_cols) + 1);
		std::vector<T> result(_rows + 1, T(0));
		for (int row = 1; row <= _rows; ++row) {
			for (int column = 1; column <= _cols; ++column) {
				result[row] += (*this)[row][column] * values[column];
			}
		}
		return result;
	}

	T trace() const {
		assert(_rows == _cols);
		T result = T(0);
		for (int i = 1; i <= _rows; ++i) result += (*this)[i][i];
		return result;
	}

	Matrix power(uint64_t k) const {
		assert(_rows == _cols);
		Matrix res = identity(_rows);
		Matrix base = *this;
		while (k > 0) {
			if (k & 1) res = res * base;
			base = base * base;
			k >>= 1;
		}
		return res;
	}
};
