#include "Shape.h"

class Particle : public Shape {
	public:
		Particle();
		virtual ~Particle();
		void rebirth(glm::vec3 newX, float t);
		void update(float t, float h);
		bool show;

	private:
		float m; // mass
		float d; // viscous damping
		glm::vec3 v; // velocity
		float lifespan; // how long this particle lives
		float tEnd;     // time this particle dies
};
