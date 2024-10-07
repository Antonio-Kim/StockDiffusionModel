#include <limits>
class Particle {
private:
	float x;
	float y;
	const float edge;
	const float max_x;
	const float max_y;
	bool done;
	const bool breakout;

public:
	Particle(float x = 0, float y = 0, float edge = 0, float max_x = std::numeric_limits<float>::max(), float max_y = std::numeric_limits<float>::max(), bool breakout =false) :
		x(x), y(y), edge(edge), max_x(max_x), max_y(max_y), done(false), breakout(breakout)
	{}

	void Move(float x_step, float y_step) {
		if (done) return;
		x += x_step;
		y += y_step;

		if (y < edge / 4) {
			done = true;
			return;
		}

		if (y > max_y) y = max_y;
		if (!breakout) {
			if (x < edge / 2) x = edge / 2;
			if (x > max_x) x = max_x;
		}
	}
	float X() const { return x; }
	float Y() const { return y; }
};