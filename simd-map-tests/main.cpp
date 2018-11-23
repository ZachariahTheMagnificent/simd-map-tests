#include <iostream>
#include <chrono>
#include <vector>
#include <random>

template<typename type>
using array = std::vector<type>;

struct vector3
{
	float x;
	float y;
	float z;
};

struct object
{
	vector3 position;
	vector3 velocity;
	vector3 acceleration;
};

int main()
{
	using namespace std::chrono_literals;

	constexpr auto total_gameobjects = 8'000'000;
	constexpr auto delta_time = 0.001f;

	auto eng = std::mt19937_64{524};
	auto random_float = std::uniform_real_distribution<float>{-15, 15};
	auto random_acceleration = std::uniform_real_distribution<float>{-1, 1};

	const auto acceleration_increase_x = random_acceleration(eng);
	const auto acceleration_increase_y = random_acceleration(eng);
	const auto acceleration_increase_z = random_acceleration(eng);

	auto objects = array<object>{};
	objects.resize(total_gameobjects);

	for(auto& element : objects)
	{
		element.position.x = random_float(eng);
		element.position.y = random_float(eng);
		element.position.z = random_float(eng);

		element.velocity.x = random_float(eng);
		element.velocity.y = random_float(eng);
		element.velocity.z = random_float(eng);

		element.acceleration.x = random_float(eng);
		element.acceleration.y = random_float(eng);
		element.acceleration.z = random_float(eng);
	}

	const auto start_point = std::chrono::steady_clock::now();

	for(auto& element : objects)
	{
		const auto acceleration_x = element.acceleration.x + acceleration_increase_x * delta_time;
		const auto acceleration_y = element.acceleration.y + acceleration_increase_y * delta_time;
		const auto acceleration_z = element.acceleration.z + acceleration_increase_z * delta_time;

		const auto velocity_x = element.velocity.x + acceleration_x * delta_time;
		const auto velocity_y = element.velocity.y + acceleration_y * delta_time;
		const auto velocity_z = element.velocity.z + acceleration_z * delta_time;

		const auto position_x = element.position.x + velocity_x * delta_time;
		const auto position_y = element.position.y + velocity_y * delta_time;
		const auto position_z = element.position.z + velocity_z * delta_time;

		element.acceleration.x = acceleration_x;
		element.acceleration.y = acceleration_y;
		element.acceleration.z = acceleration_z;

		element.acceleration.x = velocity_x;
		element.acceleration.y = velocity_y;
		element.acceleration.z = velocity_z;

		element.acceleration.x = position_x;
		element.acceleration.y = position_y;
		element.acceleration.z = position_z;
	}

	const auto end_point = std::chrono::steady_clock::now();

	const auto duration = std::chrono::duration<double>(end_point - start_point);
	const auto duration_in_nanoseconds = duration.count();

	std::cout << "Time taken: " << duration_in_nanoseconds << "s\n";

	return EXIT_SUCCESS;
}
