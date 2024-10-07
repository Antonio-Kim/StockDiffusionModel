#include <random>

class Air {
	std::mt19937 engine;
	std::normal_distribution<float> normal_dist;
	const float step;
public:
	Air(float step, unsigned int seed = 1) : step(step), engine(seed)
	{}
	float Bump() {
		return step * normal_dist(engine);
	}
};