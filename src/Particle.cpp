#include "Particle.h"

// TODO: Use Shape#randFloat
float randFloat2(float l, float h)
{
	float r = rand() / (float)RAND_MAX;
	return (1.0f - r) * l + r * h;
}

Particle::Particle() :
	m(1.0f),
	d(0.0f),
	v(0.0f, 0.0f, 0.0f),
	lifespan(10.0f),
	tEnd(0.0f),
	show(false)
{
}

Particle::~Particle()
{
}

void Particle::rebirth(glm::vec3 newX, float t)
{
	m = randFloat2(0.1f, 1.0f);
	d = randFloat2(0.0f, 0.19f);
	x = newX;
	v.x = randFloat2(-0.5f, 0.5f);
	v.y = randFloat2(-0.5f, 0.5f);
	v.z = randFloat2(-0.5f, 0.5f);
	tEnd = t + lifespan;
	show = true;
}

// h is the time step (delta t)
void Particle::update(float t, float h)
{
	if (show) {
		glm::vec3 g = glm::vec3(0.0f, -0.01f, 0.0f);

		glm::vec3 fd = -d * v;
	  float C = randFloat2(0.1f, 1.0f);
	 	float r = abs(sqrt((x.x * x.x) + (x.y * x.y) + (x.z * x.z)));
	 	glm::vec3 gravityForce = (m * g);
		glm::vec3 f = gravityForce + fd;
		v = v + (h/m) * f;
		if (t > tEnd) {
			show = false;
		}
		x += h * v;
	}
}
