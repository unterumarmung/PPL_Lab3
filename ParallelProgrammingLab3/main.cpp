#include <chrono>

#include "functions.hpp"


using namespace std::chrono;


__forceinline milliseconds measure_time(auto func) {
	const auto before = high_resolution_clock::now();

	func();

	const auto after = high_resolution_clock::now();

	return duration_cast<milliseconds>(after - before);
}


template <allowed T>
void run_test(size_t rows, size_t cols) {
	auto** a = make_matrix<T>(rows, cols);
	auto** b = make_matrix<T>(rows, cols);
	auto** c = make_matrix<T>(rows, cols);

	fill_with_random(a, rows, cols);
	fill_with_random(b, rows, cols);

	std::cout << "START TESTING" << std::endl;

	const auto simple_time = measure_time([&] {
		simple_impl(a, b, rows, cols, c);
	});
	std::cout << "Simple implementation: " << simple_time << std::endl;

	const auto intrinsic_time = measure_time([&] {
		intrinsic_impl(a, b, rows, cols, c);
	});
	std::cout << "Intrinsic implementation: " << intrinsic_time << std::endl;

	const auto asm_time = measure_time([&] {
		asm_impl(a, b, rows, cols, c);
	});
	std::cout << "Asm implementation: " << asm_time << std::endl;

	delete_matrix(a, rows, cols);
	delete_matrix(b, rows, cols);
	delete_matrix(c, rows, cols);
}


int main() {
	size_t rows = 5;
	size_t cols = 16;

	std::cin >> rows;
	std::cin >> cols;

	cols -= cols % alignment;

	std::cout << "TEST FOR std::int16_t" << std::endl;
	run_test<int16_t>(rows, cols);
}

