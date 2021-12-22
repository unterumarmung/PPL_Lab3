#pragma once

#include <concepts>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <malloc.h>

template <typename T>
concept allowed = std::same_as<T, std::int8_t>
|| std::same_as<T, std::int16_t>
|| std::same_as<T, std::int32_t>
|| std::same_as<T, std::int64_t>
|| std::same_as<T, float>
|| std::same_as<T, double>;


inline constexpr auto alignment = 16;

template <allowed T>
void fill_with_random(T** matrix, size_t rows, size_t cols) {
	std::minstd_rand0 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count()); // Standard mersenne_twister_engine seeded with current time

	using generator_t = std::conditional_t<std::integral<T>, std::uniform_int_distribution<std::int64_t>, std::uniform_real_distribution<T>>;

	// generator_t rand(0, 99);

	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			matrix[i][j] = std::rand();
		}
	}
}


template <allowed T>
void print(T** matrix, size_t rows, size_t cols) {
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			if constexpr (std::integral<T>) {
				std::cout << std::setw(5) << static_cast<int64_t>(matrix[i][j]);
			}
			else {
				std::cout << std::setprecision(3) << std::setw(6) << matrix[i][j];
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


template <allowed T>
T** make_matrix(size_t rows, size_t cols) {
	auto** matrix = new T * [rows];
	for (size_t i = 0; i < rows; ++i) {
		matrix[i] = static_cast<T*>(_aligned_malloc(sizeof(T) * cols, alignment));
	}
	return matrix;
}

template <allowed T>
void delete_matrix(T** matrix, size_t rows, size_t cols) {
	for (size_t i = 0; i < rows; ++i) {
		_aligned_free(matrix[i]);
	}
	delete[] matrix;
}

template <allowed T>
void simple_impl(T** a, T** b, size_t rows, size_t cols, T** c) {
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			if constexpr (std::integral<T>) {
				c[i][j] = std::min(a[i][j], b[i][j]) + std::max(a[i][j], b[i][j]);
			}
			else {
				c[i][j] = std::sqrt(a[i][j]) + std::sqrt(b[i][j]);
			}
		}
	}
}

template <typename T>
void intrinsic_impl(T** a, T** b, size_t rows, size_t cols, T** c) = delete;

template<> void intrinsic_impl<double>(double** a, double** b, size_t rows, size_t cols, double** c);
template<> void intrinsic_impl<float>(float** a, float** b, size_t rows, size_t cols, float** c);
template<> void intrinsic_impl<std::int8_t>(std::int8_t** a, std::int8_t** b, size_t rows, size_t cols, std::int8_t** c);
template<> void intrinsic_impl<std::int16_t>(std::int16_t** a, std::int16_t** b, size_t rows, size_t cols, std::int16_t** c);
template<> void intrinsic_impl<std::int32_t>(std::int32_t** a, std::int32_t** b, size_t rows, size_t cols, std::int32_t** c);
template<> void intrinsic_impl<std::int64_t>(std::int64_t** a, std::int64_t** b, size_t rows, size_t cols, std::int64_t** c);

template <typename T>
void asm_impl(T** a, T** b, size_t rows, size_t cols, T** c) = delete;

template<> void asm_impl<double>(double** a, double** b, size_t rows, size_t cols, double** c);
template<> void asm_impl<float>(float** a, float** b, size_t rows, size_t cols, float** c);
template<> void asm_impl<std::int8_t>(std::int8_t** a, std::int8_t** b, size_t rows, size_t cols, std::int8_t** c);
template<> void asm_impl<std::int16_t>(std::int16_t** a, std::int16_t** b, size_t rows, size_t cols, std::int16_t** c);
template<> void asm_impl<std::int32_t>(std::int32_t** a, std::int32_t** b, size_t rows, size_t cols, std::int32_t** c);
template<> void asm_impl<std::int64_t>(std::int64_t** a, std::int64_t** b, size_t rows, size_t cols, std::int64_t** c);

template <typename T>
consteval size_t get_step() {
	if (std::same_as<T, double>) {
		return 2;
	}
	else if (std::same_as<T, float>) {
		return 4;
	}
	else {
		return 16 / sizeof(T);
	}
}
