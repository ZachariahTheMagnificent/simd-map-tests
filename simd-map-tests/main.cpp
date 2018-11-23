#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <immintrin.h>

template<typename type>
using array = std::vector<type>;

struct simd_vector
{
	using underlying_type = __m256;
	using element_type = float;

	static constexpr auto size = sizeof(underlying_type) / sizeof(element_type);

	alignas(sizeof(underlying_type)) element_type elements[size];

	constexpr operator float*() noexcept
	{
		return elements;
	}
	constexpr operator const float*() const noexcept
	{
		return elements;
	}

	template<typename type>
	element_type& operator[](const type index) noexcept
	{
		return elements[index];
	}
	template<typename type>
	const element_type& operator[](const type index) const noexcept
	{
		return elements[index];
	}
};

struct vector3
{
	simd_vector x;
	simd_vector y;
	simd_vector z;
};

struct object
{
	vector3 acceleration;
	vector3 velocity;
	vector3 position;
};

int main()
{
	using namespace std::chrono_literals;

	constexpr auto total_gameobjects = 8'000'000;
	constexpr auto total_vectors = total_gameobjects / simd_vector::size;
	constexpr auto delta_time = 0.001f;

	auto eng = std::mt19937_64{524};
	auto random_float = std::uniform_real_distribution<float>{-15, 15};
	auto random_acceleration = std::uniform_real_distribution<float>{-1, 1};

	const auto acceleration_increase_x = random_acceleration(eng);
	const auto acceleration_increase_y = random_acceleration(eng);
	const auto acceleration_increase_z = random_acceleration(eng);

	auto objects = array<object>{};
	objects.resize(total_vectors);

	for(auto& element : objects)
	{
		for(auto index = 0; index < simd_vector::size; ++index)
		{
			element.acceleration.x[index] = random_float(eng);
			element.acceleration.y[index] = random_float(eng);
			element.acceleration.z[index] = random_float(eng);

			element.velocity.x[index] = random_float(eng);
			element.velocity.y[index] = random_float(eng);
			element.velocity.z[index] = random_float(eng);

			element.position.x[index] = random_float(eng);
			element.position.y[index] = random_float(eng);
			element.position.z[index] = random_float(eng);
		}
	}

	const auto start_point = std::chrono::steady_clock::now();

	for(auto& element : objects)
	{
		const auto acceleration_vector_x = _mm256_load_ps(element.acceleration.x);
		const auto acceleration_vector_y = _mm256_load_ps(element.acceleration.y);
		const auto acceleration_vector_z = _mm256_load_ps(element.acceleration.z);

		const auto velocity_vector_x = _mm256_load_ps(element.velocity.x);
		const auto velocity_vector_y = _mm256_load_ps(element.velocity.y);
		const auto velocity_vector_z = _mm256_load_ps(element.velocity.z);

		const auto position_vector_x = _mm256_load_ps(element.position.x);
		const auto position_vector_y = _mm256_load_ps(element.position.y);
		const auto position_vector_z = _mm256_load_ps(element.position.z);


	}

	const auto end_point = std::chrono::steady_clock::now();

	const auto duration = std::chrono::duration<double>(end_point - start_point);
	const auto duration_in_nanoseconds = duration.count();

	std::cout << "Time taken: " << duration_in_nanoseconds << "s\n";

	return EXIT_SUCCESS;
}
