#pragma once PRICE_SIMULATION

#include<random>
#include<chrono>

namespace Diffuse {
	class PriceSimulation {
	private:
		double price;
		double drift;
		double vol;
		double dt;
		double jump;

		std::mt19937 engine;
		std::normal_distribution<> normal_dist;
		std::poisson_distribution<> poisson_dist;

	public:
		PriceSimulation(
			double price,
			double drift,
			double vol,
			double dt,
			unsigned int seed = std::chrono::high_resolution_clock::now().time_since_epoch().count(),
			double jump = 0.0,
			double mean_jump_per_unit_time = 0.1
		);
		double Next();
	};

	double get_value(int argc, const char** argv, int pos, double value);
}
