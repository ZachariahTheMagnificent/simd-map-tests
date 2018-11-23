#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <immintrin.h>

template<typename type>
using array = std::vector<type>;

int main()
{
	using namespace std::chrono_literals;
	using simd_vector = __m256;

	constexpr auto total_gameobjects = 8'000'000;
	constexpr auto float_vector_size = sizeof(simd_vector) / sizeof(float);
	constexpr auto total_vectors = total_gameobjects / float_vector_size;
	constexpr auto delta_time = 0.001f;

	auto eng = std::mt19937_64{524};
	auto random_float = std::uniform_real_distribution<float>{-15, 15};
	auto random_acceleration = std::uniform_real_distribution<float>{-1, 1};

	const auto acceleration_increase_x = random_acceleration(eng);
	const auto acceleration_increase_y = random_acceleration(eng);
	const auto acceleration_increase_z = random_acceleration(eng);

	__m256* acceleration_x = new __m256[total_vectors];
	__m256* acceleration_y = new __m256[total_vectors];
	__m256* acceleration_z = new __m256[total_vectors];
	__m256* velocity_x = new __m256[total_vectors];
	__m256* velocity_y = new __m256[total_vectors];
	__m256* velocity_z = new __m256[total_vectors];
	__m256* position_x = new __m256[total_vectors];
	__m256* position_y = new __m256[total_vectors];
	__m256* position_z = new __m256[total_vectors];

	for(auto index = 0; index < total_vectors; ++index)
	{
		for(auto inner_index = 0; index < float_vector_size; ++index)
		{
			acceleration_x[index].m256_f32[inner_index] = random_float(eng);
			acceleration_y[index].m256_f32[inner_index] = random_float(eng);
			acceleration_z[index].m256_f32[inner_index] = random_float(eng);
			velocity_x[index].m256_f32[inner_index] = random_float(eng);
			velocity_y[index].m256_f32[inner_index] = random_float(eng);
			velocity_z[index].m256_f32[inner_index] = random_float(eng);
			position_x[index].m256_f32[inner_index] = random_float(eng);
			position_y[index].m256_f32[inner_index] = random_float(eng);
			position_z[index].m256_f32[inner_index] = random_float(eng);
		}
	}

	const auto acceleration_increase_x_vector = _mm256_set1_ps(acceleration_increase_x);
	const auto acceleration_increase_y_vector = _mm256_set1_ps(acceleration_increase_y);
	const auto acceleration_increase_z_vector = _mm256_set1_ps(acceleration_increase_z);
	const auto delta_time_vector = _mm256_set1_ps(delta_time);

	const auto start_point = std::chrono::steady_clock::now();

	for(auto index = 0; index < total_vectors; ++index)
	{
		acceleration_x[index] = _mm256_fmadd_ps(acceleration_increase_x_vector, delta_time_vector, acceleration_x[index]);
	}
	for(auto index = 0; index < total_vectors; ++index)
	{
		acceleration_y[index] = _mm256_fmadd_ps(acceleration_increase_y_vector, delta_time_vector, acceleration_y[index]);
	}
	for(auto index = 0; index < total_vectors; ++index)
	{
		acceleration_z[index] = _mm256_fmadd_ps(acceleration_increase_z_vector, delta_time_vector, acceleration_z[index]);
	}
	for(auto index = 0; index < total_vectors; ++index)
	{
		velocity_x[index] = _mm256_fmadd_ps(acceleration_x[index], delta_time_vector, velocity_x[index]);
	}
	for(auto index = 0; index < total_vectors; ++index)
	{
		velocity_y[index] = _mm256_fmadd_ps(acceleration_x[index], delta_time_vector, velocity_y[index]);
	}
	for(auto index = 0; index < total_vectors; ++index)
	{
		velocity_z[index] = _mm256_fmadd_ps(acceleration_x[index], delta_time_vector, velocity_z[index]);
	}
	for(auto index = 0; index < total_vectors; ++index)
	{
		position_x[index] = _mm256_fmadd_ps(velocity_x[index], delta_time_vector, position_x[index]);
	}
	for(auto index = 0; index < total_vectors; ++index)
	{
		position_y[index] = _mm256_fmadd_ps(velocity_y[index], delta_time_vector, position_y[index]);
	}
	for(auto index = 0; index < total_vectors; ++index)
	{
		position_z[index] = _mm256_fmadd_ps(velocity_z[index], delta_time_vector, position_z[index]);
	}

	const auto end_point = std::chrono::steady_clock::now();
	
	delete[] acceleration_x;
	delete[] acceleration_y;
	delete[] acceleration_z;
	delete[] velocity_x;
	delete[] velocity_y;
	delete[] velocity_z;
	delete[] position_x;
	delete[] position_y;
	delete[] position_z;

	const auto duration = std::chrono::duration<double>(end_point - start_point);
	const auto duration_in_nanoseconds = duration.count();

	std::cout << "Time taken: " << duration_in_nanoseconds << "s\n";

	return EXIT_SUCCESS;
}
