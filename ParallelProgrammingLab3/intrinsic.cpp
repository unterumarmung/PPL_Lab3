#include "functions.hpp"

#include <intrin.h>
#include <immintrin.h>
#include <ammintrin.h>

template <allowed T> 
__forceinline auto load(T* foo) {
	if constexpr (std::same_as<T, float>) {
		return _mm_load_ps(foo);
	}
	else if constexpr (std::same_as<T, double>) {
		return _mm_load_pd(foo);
	}
	else {
		return _mm_load_si128(reinterpret_cast<__m128i*>(foo));
	}
}

__forceinline auto sqrt(auto foo) {
	if constexpr (std::same_as<decltype(foo), __m128>) {
		return _mm_sqrt_ps(foo);
	}
	else if constexpr (std::same_as<decltype(foo), __m128d>) {
		return _mm_sqrt_pd(foo);
	}
}

template <typename T>
__forceinline auto add(auto lhs, auto rhs) {
	if constexpr (std::same_as<decltype(lhs), __m128>) {
		return _mm_add_ps(lhs, rhs);
	}
	else if constexpr (std::same_as<decltype(lhs), __m128d>) {
		return _mm_add_pd(lhs, rhs);
	} else if constexpr (std::same_as<T, std::int8_t>) {
		return _mm_add_epi8(lhs, rhs);
	}
	else if constexpr (std::same_as<T, std::int16_t>) {
		return _mm_add_epi16(lhs, rhs);
	}
	else if constexpr (std::same_as<T, std::int32_t>) {
		return _mm_add_epi32(lhs, rhs);
	}
	else if constexpr (std::same_as<T, std::int64_t>) {
		return _mm_add_epi64(lhs, rhs);
	}
}

// _mm_max_epi64 and _mm_min_epi64 are AVX512 instructions 

template <std::integral T>
__forceinline auto min(__m128i lhs, __m128i rhs) {
	if constexpr (std::same_as<T, std::int8_t>) {
		return _mm_min_epi8(lhs, rhs);
	}
	else if constexpr (std::same_as<T, std::int16_t>) {
		return _mm_min_epi16(lhs, rhs);
	}
	else if constexpr (std::same_as<T, std::int32_t>) {
		return _mm_min_epi32(lhs, rhs);
	}
	else if constexpr (std::same_as<T, std::int64_t>) {
		return _mm_min_epi32(lhs, rhs);
	}
}

template <std::integral T>
__forceinline auto max(__m128i lhs, __m128i rhs) {
	if constexpr (std::same_as<T, std::int8_t>) {
		return _mm_max_epi8(lhs, rhs);
	}
	else if constexpr (std::same_as<T, std::int16_t>) {
		return _mm_max_epi16(lhs, rhs);
	}
	else if constexpr (std::same_as<T, std::int32_t>) {
		return _mm_max_epi32(lhs, rhs);
	}
	else if constexpr (std::same_as<T, std::int64_t>) {
		return _mm_max_epi32(lhs, rhs); 
	}
}

template <allowed T>
__forceinline auto store(T* foo, auto value) {
	if constexpr (std::same_as<T, float>) {
		return _mm_store_ps(foo, value);
	}
	else if constexpr (std::same_as<T, double>) {
		return _mm_store_pd(foo, value);
	}
	else {
		return _mm_store_si128(reinterpret_cast<__m128i*>(foo), value);
	}
}

template <allowed T> 
void impl(T** a, T** b, size_t rows, size_t cols, T** c) {
	constexpr auto step = get_step<T>();

	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; j += step) {
			const auto a_ij = load(&a[i][j]);
			const auto b_ij = load(&b[i][j]);

			using result_t = decltype(add<T>(a_ij, b_ij));
			
			result_t lhs;
			result_t rhs;

			if constexpr (std::integral<T>) {
				lhs = max<T>(a_ij, b_ij);
				rhs = min<T>(a_ij, b_ij);
			}
			else {
				lhs = sqrt(a_ij);
				rhs = sqrt(b_ij);
			}

			const auto result = add<T>(lhs, rhs);

			store(&c[i][j], result);
		}
	}
}

template<> void intrinsic_impl(double** a, double** b, size_t rows, size_t cols, double** c) {
	impl(a, b, rows, cols, c);
}

template<> void intrinsic_impl<float>(float** a, float** b, size_t rows, size_t cols, float** c) {
	impl(a, b, rows, cols, c);
}

template<> void intrinsic_impl<std::int8_t>(std::int8_t** a, std::int8_t** b, size_t rows, size_t cols, std::int8_t** c) {
	impl(a, b, rows, cols, c);
}
template<> void intrinsic_impl<std::int16_t>(std::int16_t** a, std::int16_t** b, size_t rows, size_t cols, std::int16_t** c) {
	impl(a, b, rows, cols, c);
}
template<> void intrinsic_impl<std::int32_t>(std::int32_t** a, std::int32_t** b, size_t rows, size_t cols, std::int32_t** c) {
	impl(a, b, rows, cols, c);
}
template<> void intrinsic_impl<std::int64_t>(std::int64_t** a, std::int64_t** b, size_t rows, size_t cols, std::int64_t** c) {
	impl(a, b, rows, cols, c);
}