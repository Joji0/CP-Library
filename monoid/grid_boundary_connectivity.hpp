#pragma once
#include <array>
#include <cassert>
#include <cstddef>
#include <string_view>

// https://atcoder.jp/contests/aalc001/tasks/aalc001_r
// Four-neighbor connectivity of equal symbols within a rectangular row block.
// Stores the colors and partition of the top and bottom boundaries, O(width)
// space per aggregate. Leaf strings and queried columns are 1-based.
template <std::size_t MaximumWidth>
struct GridBoundaryConnectivity {
	static_assert(0 < MaximumWidth && MaximumWidth <= 255 / 2);
	struct value_type {
		int width = 0;
		std::array<char, MaximumWidth + 1> top{}, bottom{};
		std::array<unsigned char, 2 * MaximumWidth + 1> labels{};
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {}; }

	static value_type make_leaf(std::string_view row) {
		assert(1 < row.size() && row.size() <= MaximumWidth + 1);
		value_type result;
		result.width = static_cast<int>(row.size()) - 1;
		int first = 1;
		for (int column = 1; column <= result.width; ++column) {
			if (column == 1 || row[column - 1] != row[column]) first = column;
			result.top[column] = result.bottom[column] = row[column];
			result.labels[column] = result.labels[result.width + column] = static_cast<unsigned char>(first);
		}
		return result;
	}

	static value_type combine(const value_type &upper, const value_type &lower) {
		if (upper.width == 0) return lower;
		if (lower.width == 0) return upper;
		const int width = upper.width;
		assert(width == lower.width);
		BoundaryPartition partition(4 * width);
		add_block(partition, upper, 0);
		add_block(partition, lower, 2 * width);
		connect_blocks(partition, upper, 0, lower, 2 * width);
		value_type result;
		result.width = width;
		result.top = upper.top;
		result.bottom = lower.bottom;
		std::array<unsigned char, 4 * MaximumWidth + 1> representative{};
		for (int index = 1; index <= 2 * width; ++index) {
			const int cell = index <= width ? index : index + 2 * width;
			auto &label = representative[partition.find(cell)];
			if (label == 0) label = static_cast<unsigned char>(index);
			result.labels[index] = label;
		}
		return result;
	}

	// Middle contains both queried rows as its top and bottom boundary.
	// Above and below are the disjoint adjacent blocks, or identity at an edge.
	static bool connected_with_context(const value_type &above, const value_type &middle, const value_type &below,
					   int top_column, int bottom_column) {
		const int width = middle.width;
		assert(1 <= top_column && top_column <= width && 1 <= bottom_column && bottom_column <= width);
		assert((above.width == 0 || above.width == width) && (below.width == 0 || below.width == width));
		BoundaryPartition partition(6 * width);
		add_block(partition, middle, 2 * width);
		if (above.width != 0) {
			add_block(partition, above, 0);
			connect_blocks(partition, above, 0, middle, 2 * width);
		}
		if (below.width != 0) {
			add_block(partition, below, 4 * width);
			connect_blocks(partition, middle, 2 * width, below, 4 * width);
		}
		return partition.find(2 * width + top_column) == partition.find(3 * width + bottom_column);
	}

       private:
	struct BoundaryPartition {
		std::array<int, 6 * MaximumWidth + 1> parent;
		explicit BoundaryPartition(int size) {
			for (int i = 1; i <= size; ++i) parent[i] = -1;
		}
		int find(int vertex) {
			if (parent[vertex] < 0) return vertex;
			return parent[vertex] = find(parent[vertex]);
		}
		void merge(int first, int second) {
			first = find(first);
			second = find(second);
			if (first == second) return;
			if (parent[first] > parent[second]) {
				const int temporary = first;
				first = second;
				second = temporary;
			}
			parent[first] += parent[second];
			parent[second] = first;
		}
	};

	static void add_block(BoundaryPartition &partition, const value_type &block, int offset) {
		for (int cell = 1; cell <= 2 * block.width; ++cell) {
			if (cell != block.labels[cell]) partition.merge(offset + cell, offset + block.labels[cell]);
		}
	}
	static void connect_blocks(BoundaryPartition &partition, const value_type &upper, int upper_offset,
				   const value_type &lower, int lower_offset) {
		for (int column = 1; column <= upper.width; ++column) {
			if (upper.bottom[column] == lower.top[column]) partition.merge(upper_offset + upper.width + column, lower_offset + column);
		}
	}
};
