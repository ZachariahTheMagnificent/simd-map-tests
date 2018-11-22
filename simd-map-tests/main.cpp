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

	auto position_x = array<float>{};
	auto position_y = array<float>{};
	auto position_z = array<float>{};

	auto velocity_x = array<float>{};
	auto velocity_y = array<float>{};
	auto velocity_z = array<float>{};

	auto acceleration_x = array<float>{};
	auto acceleration_y = array<float>{};
	auto acceleration_z = array<float>{};

	position_x.resize(total_gameobjects);
	position_y.resize(total_gameobjects);
	position_z.resize(total_gameobjects);

	velocity_x.resize(total_gameobjects);
	velocity_y.resize(total_gameobjects);
	velocity_z.resize(total_gameobjects);

	acceleration_x.resize(total_gameobjects);
	acceleration_y.resize(total_gameobjects);
	acceleration_z.resize(total_gameobjects);

	for(auto index = 0; index < total_gameobjects; ++index)
	{
		position_x[index] = random_float(eng);
		position_y[index] = random_float(eng);
		position_z[index] = random_float(eng);

		velocity_x[index] = random_float(eng);
		velocity_y[index] = random_float(eng);
		velocity_z[index] = random_float(eng);

		acceleration_x[index] = random_float(eng);
		acceleration_y[index] = random_float(eng);
		acceleration_z[index] = random_float(eng);
	}

	const auto start_point = std::chrono::steady_clock::now();

	for(auto index = 0; index < total_gameobjects; ++index)
	{
		acceleration_x[index] += acceleration * delta_time;
		acceleration_y[index] += acceleration * delta_time;
		acceleration_z[index] += acceleration * delta_time;
	}
	for(auto index = 0; index < total_gameobjects; ++index)
	{
		velocity_x[index] += acceleration_x[index] * delta_time;
		velocity_y[index] += acceleration_y[index] * delta_time;
		velocity_z[index] += acceleration_z[index] * delta_time;
	}
	for(auto index = 0; index < total_gameobjects; ++index)
	{
		position_x[index] += velocity_x[index] * delta_time;
		position_y[index] += velocity_y[index] * delta_time;
		position_z[index] += velocity_z[index] * delta_time;
	}

	const auto end_point = std::chrono::steady_clock::now();

	const auto duration = std::chrono::duration<double>(end_point - start_point);
	const auto duration_in_nanoseconds = duration.count();

	std::cout << "Time taken: " << duration_in_nanoseconds << "s\n";

	return EXIT_SUCCESS;
}
