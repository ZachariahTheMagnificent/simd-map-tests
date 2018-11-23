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
		element.acceleration.x += acceleration_increase_x * delta_time;
		element.acceleration.y += acceleration_increase_y * delta_time;
		element.acceleration.z += acceleration_increase_z * delta_time;

		element.velocity.x += element.acceleration.x * delta_time;
		element.velocity.y += element.acceleration.y * delta_time;
		element.velocity.z += element.acceleration.z * delta_time;

		element.position.x += element.velocity.x * delta_time;
		element.position.y += element.velocity.y * delta_time;
		element.position.z += element.velocity.z * delta_time;
	}

	const auto end_point = std::chrono::steady_clock::now();

	const auto duration = std::chrono::duration<double>(end_point - start_point);
	const auto duration_in_nanoseconds = duration.count();

	std::cout << "Time taken: " << duration_in_nanoseconds << "s\n";

	return EXIT_SUCCESS;
}
