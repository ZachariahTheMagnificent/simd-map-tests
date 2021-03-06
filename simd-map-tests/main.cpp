#include <iostream>
#include <chrono>
#include <vector>
#include <random>

template<typename type>
using array = std::vector<type>;

int main()
{
	using namespace std::chrono_literals;

	constexpr auto total_gameobjects = 8'000'000;
	constexpr auto delta_time = 0.001f;
	constexpr auto acceleration = 1.f;

	auto eng = std::mt19937_64{524};
	auto random_float = std::uniform_real_distribution<float>{-15, 15};

	const auto start_point = std::chrono::steady_clock::now();

	const auto end_point = std::chrono::steady_clock::now();

	const auto duration = std::chrono::duration<double>(end_point - start_point);
	const auto duration_in_nanoseconds = duration.count();

	std::cout << "Time taken: " << duration_in_nanoseconds << "s\n";

	return EXIT_SUCCESS;
}
