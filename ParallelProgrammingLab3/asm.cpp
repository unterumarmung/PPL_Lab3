#include "functions.hpp"


template<> void asm_impl<std::int64_t>(std::int64_t** a, std::int64_t** b, size_t rows, size_t cols, std::int64_t** c) {
	for (size_t i = 0; i < rows; ++i) {
		auto* cur_a = a[i];
		auto* cur_b = b[i];
		auto* cur_c = c[i];

		using value_type = decltype(*cur_a);
		const auto iteration_count = cols / get_step<value_type>();
		constexpr auto type_size = sizeof(value_type);
		constexpr auto step = get_step<value_type>() * type_size;

		_asm {
				mov esi, cur_a
				mov edi, cur_b
				mov edx, cur_c
				mov ecx, iteration_count
			vectorized:
				movdqa xmm0, xmmword ptr [esi]
				movdqa xmm1, xmmword ptr [edi]

				vpminsd xmm2, xmm0, xmm1
				vpmaxsd xmm3, xmm0, xmm1

				paddq xmm2, xmm3

				movdqa xmmword ptr [edx], xmm2

				add esi, step
				add edi, step
				add edx, step
				loop vectorized
		}
	}
}

template<> void asm_impl<std::int32_t>(std::int32_t** a, std::int32_t** b, size_t rows, size_t cols, std::int32_t** c) {
	for (size_t i = 0; i < rows; ++i) {
		auto* cur_a = a[i];
		auto* cur_b = b[i];
		auto* cur_c = c[i];

		using value_type = decltype(*cur_a);
		const auto iteration_count = cols / get_step<value_type>();
		constexpr auto type_size = sizeof(value_type);
		constexpr auto step = get_step<value_type>() * type_size;

		_asm {
				mov esi, cur_a
				mov edi, cur_b
				mov edx, cur_c
				mov ecx, iteration_count
			vectorized:
				movdqa xmm0, xmmword ptr [esi]
				movdqa xmm1, xmmword ptr [edi]

				vpminsd xmm2, xmm0, xmm1
				vpmaxsd xmm3, xmm0, xmm1

				paddd xmm2, xmm3

				movdqa xmmword ptr [edx], xmm2

				add esi, step
				add edi, step
				add edx, step
				loop vectorized
		}
	}
}

template<> void asm_impl<std::int16_t>(std::int16_t** a, std::int16_t** b, size_t rows, size_t cols, std::int16_t** c) {
	for (size_t i = 0; i < rows; ++i) {
		auto* cur_a = a[i];
		auto* cur_b = b[i];
		auto* cur_c = c[i];

		using value_type = decltype(*cur_a);
		const auto iteration_count = cols / get_step<value_type>();
		constexpr auto type_size = sizeof(value_type);
		constexpr auto step = get_step<value_type>() * type_size;

		_asm {
				mov esi, cur_a
				mov edi, cur_b
				mov edx, cur_c
				mov ecx, iteration_count
			vectorized:
				movdqa xmm0, xmmword ptr [esi]
				movdqa xmm1, xmmword ptr [edi]

				vpminsw xmm2, xmm0, xmm1
				vpmaxsw xmm3, xmm0, xmm1

				paddw xmm2, xmm3

				movdqa xmmword ptr [edx], xmm2

				add esi, step
				add edi, step
				add edx, step
				loop vectorized
		}
	}
}

template<> void asm_impl<std::int8_t>(std::int8_t** a, std::int8_t** b, size_t rows, size_t cols, std::int8_t** c) {
	for (size_t i = 0; i < rows; ++i) {
		auto* cur_a = a[i];
		auto* cur_b = b[i];
		auto* cur_c = c[i];

		using value_type = decltype(*cur_a);
		const auto iteration_count = cols / get_step<value_type>();
		constexpr auto type_size = sizeof(value_type);
		constexpr auto step = get_step<value_type>() * type_size;

		_asm {
				mov esi, cur_a
				mov edi, cur_b
				mov edx, cur_c
				mov ecx, iteration_count
			vectorized:
				movdqa xmm0, xmmword ptr [esi]
				movdqa xmm1, xmmword ptr [edi]

				vpminsb xmm2, xmm0, xmm1
				vpmaxsb xmm3, xmm0, xmm1

				paddb xmm2, xmm3

				movdqa xmmword ptr [edx], xmm2

				add esi, step
				add edi, step
				add edx, step
				loop vectorized
		}
	}
}

template<> void asm_impl<double>(double** a, double** b, size_t rows, size_t cols, double** c) {
	for (int i = 0; i < rows; ++i) {
		auto* cur_a = a[i];
		auto* cur_b = b[i];
		auto* cur_c = c[i];

		using value_type = decltype(*cur_a);
		const auto iteration_count = cols / get_step<value_type>();
		constexpr auto type_size = sizeof(value_type);
		constexpr auto step = get_step<value_type>() * type_size;

		_asm {
				mov esi, cur_a
				mov edi, cur_b
				mov edx, cur_c
				mov ecx, iteration_count
			vectorized:
				movapd xmm0, xmmword ptr [esi]
				movapd xmm1, xmmword ptr [edi]

				sqrtpd xmm0, xmm0
				sqrtpd xmm1, xmm1

				addpd xmm0, xmm1

				movapd xmmword ptr [edx], xmm0

				add esi, step
				add edi, step
				add edx, step
				loop vectorized
		}
	}
}

template<> void asm_impl<float>(float** a, float** b, size_t rows, size_t cols, float** c) {
	for (size_t i = 0; i < rows; ++i) {
		auto* cur_a = a[i];
		auto* cur_b = b[i];
		auto* cur_c = c[i];

		using value_type = decltype(*cur_a);
		const auto iteration_count = cols / get_step<value_type>();
		constexpr auto type_size = sizeof(value_type);
		constexpr auto step = get_step<value_type>() * type_size;

		_asm {
				mov esi, cur_a
				mov edi, cur_b
				mov edx, cur_c
				mov ecx, iteration_count
			vectorized:
				movaps xmm0, xmmword ptr [esi]
				movaps xmm1, xmmword ptr [edi]

				sqrtps xmm0, xmm0
				sqrtps xmm1, xmm1

				addps xmm0, xmm1

				movaps xmmword ptr [edx], xmm0

				add esi, step
				add edi, step
				add edx, step
				loop vectorized
		}
	}
}
