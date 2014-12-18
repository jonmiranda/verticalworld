#include "Character.h"

Character::Character() :
	isFacingForward(true),
	isJumping(false),
	isFalling(false),
	isFloatingIn(true),
	isFlying(false),
	gravity(0.02f)
{}

Character::~Character() {}

void Character::moveLeft(float delta, float threshold) {
	translationVec.x -= delta;
	translationVec.x = fmax(threshold, translationVec.x);
}

void Character::moveRight(float delta, float threshold) {
	translationVec.x += delta;
	translationVec.x = fmin(threshold, translationVec.x);
}

void Character::handleFloating() {
		if (isFloatingIn) {
			translationVec.z += 0.01;
			if (translationVec.z > 0.3) {
				isFloatingIn = false;
			}
		} else {
			translationVec.z -= 0.01;
			translationVec.z = fmax(0.0, translationVec.z);
			if (translationVec.z <= 0.0) {
				isFloatingIn = true;
			}
		}
}

void Character::faceForward(bool forward) {
	setRotationAngle(forward ? 0.0f : 3.14159f);
}

void Character::fly(bool fly, float ceilThreshold, float floorThreshold) {
	if (fly) {
		translationVec.y += 0.05;
	} else {
		translationVec.y -= 0.05;
	}
	
	translationVec.y = fmax(translationVec.y, floorThreshold);
	translationVec.y = fmin(translationVec.y, ceilThreshold);
}
