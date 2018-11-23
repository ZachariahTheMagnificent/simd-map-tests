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

	float* acceleration_x = static_cast<float*>(_aligned_malloc(total_gameobjects * sizeof(float), sizeof(simd_vector)));
	float* acceleration_y = static_cast<float*>(_aligned_malloc(total_gameobjects * sizeof(float), sizeof(simd_vector)));
	float* acceleration_z = static_cast<float*>(_aligned_malloc(total_gameobjects * sizeof(float), sizeof(simd_vector)));

	float* velocity_x = static_cast<float*>(_aligned_malloc(total_gameobjects * sizeof(float), sizeof(simd_vector)));
	float* velocity_y = static_cast<float*>(_aligned_malloc(total_gameobjects * sizeof(float), sizeof(simd_vector)));
	float* velocity_z = static_cast<float*>(_aligned_malloc(total_gameobjects * sizeof(float), sizeof(simd_vector)));

	float* position_x = static_cast<float*>(_aligned_malloc(total_gameobjects * sizeof(float), sizeof(simd_vector)));
	float* position_y = static_cast<float*>(_aligned_malloc(total_gameobjects * sizeof(float), sizeof(simd_vector)));
	float* position_z = static_cast<float*>(_aligned_malloc(total_gameobjects * sizeof(float), sizeof(simd_vector)));
	
	for(auto index = 0; index < total_gameobjects; ++index)
	{
		acceleration_x[index] = random_float(eng);
		acceleration_y[index] = random_float(eng);
		acceleration_z[index] = random_float(eng);
		velocity_x[index] = random_float(eng);
		velocity_y[index] = random_float(eng);
		velocity_z[index] = random_float(eng);
		position_x[index] = random_float(eng);
		position_y[index] = random_float(eng);
		position_z[index] = random_float(eng);
	}

	const auto acceleration_increase_vector_x = _mm256_set1_ps(acceleration_increase_x);
	const auto acceleration_increase_vector_y = _mm256_set1_ps(acceleration_increase_y);
	const auto acceleration_increase_vector_z = _mm256_set1_ps(acceleration_increase_z);
	const auto delta_time_vector = _mm256_set1_ps(delta_time);

	const auto start_point = std::chrono::steady_clock::now();

	for(auto index = 0; index < total_gameobjects; index += float_vector_size)
	{
		const auto acceleration_x_vector = _mm256_load_ps(acceleration_x + index);
		const auto acceleration_y_vector = _mm256_load_ps(acceleration_y + index);
		const auto acceleration_z_vector = _mm256_load_ps(acceleration_z + index);
		const auto velocity_x_vector = _mm256_load_ps(velocity_x + index);
		const auto velocity_y_vector = _mm256_load_ps(velocity_y + index);
		const auto velocity_z_vector = _mm256_load_ps(velocity_z + index);
		const auto position_x_vector = _mm256_load_ps(position_x + index);
		const auto position_y_vector = _mm256_load_ps(position_y + index);
		const auto position_z_vector = _mm256_load_ps(position_z + index);

		const auto acceleration_increase_x = _mm256_mul_ps(acceleration_increase_vector_x, delta_time_vector);
		const auto acceleration_increase_y = _mm256_mul_ps(acceleration_increase_vector_y, delta_time_vector);
		const auto acceleration_increase_z = _mm256_mul_ps(acceleration_increase_vector_z, delta_time_vector);

		const auto new_acceleration_x_vector = _mm256_add_ps(acceleration_x_vector, acceleration_increase_x);
		const auto new_acceleration_y_vector = _mm256_add_ps(acceleration_y_vector, acceleration_increase_y);
		const auto new_acceleration_z_vector = _mm256_add_ps(acceleration_z_vector, acceleration_increase_z);

		const auto velocity_increase_x = _mm256_mul_ps(new_acceleration_x_vector, delta_time_vector);
		const auto velocity_increase_y = _mm256_mul_ps(new_acceleration_y_vector, delta_time_vector);
		const auto velocity_increase_z = _mm256_mul_ps(new_acceleration_z_vector, delta_time_vector);

		const auto new_velocity_x_vector = _mm256_add_ps(velocity_x_vector, velocity_increase_x);
		const auto new_velocity_y_vector = _mm256_add_ps(velocity_y_vector, velocity_increase_y);
		const auto new_velocity_z_vector = _mm256_add_ps(velocity_z_vector, velocity_increase_z);

		const auto position_increase_x = _mm256_mul_ps(new_velocity_x_vector, delta_time_vector);
		const auto position_increase_y = _mm256_mul_ps(new_velocity_y_vector, delta_time_vector);
		const auto position_increase_z = _mm256_mul_ps(new_velocity_z_vector, delta_time_vector);

		const auto new_position_x_vector = _mm256_add_ps(position_x_vector, position_increase_x);
		const auto new_position_y_vector = _mm256_add_ps(position_y_vector, position_increase_y);
		const auto new_position_z_vector = _mm256_add_ps(position_z_vector, position_increase_z);

		_mm256_store_ps(acceleration_x + index, new_acceleration_x_vector);
		_mm256_store_ps(acceleration_y + index, new_acceleration_y_vector);
		_mm256_store_ps(acceleration_z + index, new_acceleration_z_vector);
		_mm256_store_ps(velocity_x + index, new_velocity_x_vector);
		_mm256_store_ps(velocity_y + index, new_velocity_y_vector);
		_mm256_store_ps(velocity_z + index, new_velocity_z_vector);
		_mm256_store_ps(position_x + index, new_position_x_vector);
		_mm256_store_ps(position_y + index, new_position_y_vector);
		_mm256_store_ps(position_z + index, new_position_z_vector);
	}

	const auto end_point = std::chrono::steady_clock::now();

	_aligned_free(acceleration_x);
	_aligned_free(acceleration_y);
	_aligned_free(acceleration_z);

	_aligned_free(velocity_x);
	_aligned_free(velocity_y);
	_aligned_free(velocity_z);

	_aligned_free(position_x);
	_aligned_free(position_y);
	_aligned_free(position_z);

	const auto duration = std::chrono::duration<double>(end_point - start_point);
	const auto duration_in_nanoseconds = duration.count();

	std::cout << "Time taken: " << duration_in_nanoseconds << "s\n";

	return EXIT_SUCCESS;
}
