#include "Particle.h"

class Particle;
class Character : public Shape {
	public:
		Character();
		virtual ~Character();
		bool isJumping;
		bool isFalling;
		void moveLeft(float delta, float threshold);
		void moveRight(float delta, float threshold);
		void handleFloating();
		void faceForward(bool forward);
		void fly(bool fly, float ceilThreshold, float floorThreshold);

	private:
		std::vector<Particle> particles;
		bool isFacingForward;
		bool isFloatingIn;
		float gravity;
		bool isFlying;
};
