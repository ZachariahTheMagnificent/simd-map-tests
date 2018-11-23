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
	constexpr auto acceleration = 1.f;

	auto eng = std::mt19937_64{524};
	auto random_float = std::uniform_real_distribution<float>{-15, 15};

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

	const auto acceleration_vector = _mm256_set1_ps(acceleration);
	const auto delta_time_vector = _mm256_set1_ps(delta_time);

	const auto start_point = std::chrono::steady_clock::now();

	for(auto index = 0; index < total_vectors; ++index)
	{
		const auto new_acceleration_x = _mm256_fmadd_ps(acceleration_vector, delta_time_vector, acceleration_x[index]);
		const auto new_acceleration_y = _mm256_fmadd_ps(acceleration_vector, delta_time_vector, acceleration_y[index]);
		const auto new_acceleration_z = _mm256_fmadd_ps(acceleration_vector, delta_time_vector, acceleration_z[index]);

		const auto new_velocity_x = _mm256_fmadd_ps(new_acceleration_x, delta_time_vector, velocity_x[index]);
		const auto new_velocity_y = _mm256_fmadd_ps(new_acceleration_y, delta_time_vector, velocity_y[index]);
		const auto new_velocity_z = _mm256_fmadd_ps(new_acceleration_z, delta_time_vector, velocity_z[index]);

		const auto new_position_x = _mm256_fmadd_ps(new_velocity_x, delta_time_vector, position_x[index]);
		const auto new_position_y = _mm256_fmadd_ps(new_velocity_y, delta_time_vector, position_y[index]);
		const auto new_position_z = _mm256_fmadd_ps(new_velocity_z, delta_time_vector, position_z[index]);

		acceleration_x[index] = new_acceleration_x;
		acceleration_y[index] = new_acceleration_y;
		acceleration_z[index] = new_acceleration_z;

		velocity_x[index] = new_velocity_x;
		velocity_y[index] = new_velocity_y;
		velocity_z[index] = new_velocity_z;

		position_x[index] = new_position_x;
		position_y[index] = new_position_y;
		position_z[index] = new_position_z;
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
