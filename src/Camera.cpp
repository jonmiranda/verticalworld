//
// sueda
// November, 2014
//

#include "Camera.h"
#include "MatrixStack.h"
#include <iostream>
#define GLM_FORCE_RADIANS
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"

#define PI 3.1415926359f

using namespace std;

Camera::Camera() :
	aspect(1.0f),
	fovy(30.0f / 180.0f * M_PI),
	znear(0.1f),
	zfar(1000.0f),
	position(0.0f, 1.0f, 0.0f),
	mousePrev(-1.0f, -1.0f),
	rotations(0.0f, 0.0f),
	rfactor(0.01f)
{
}

Camera::~Camera()
{
}

void Camera::setWindowSize(float w, float h)
{
	aspect = w/h;
}

glm::vec3 Camera::getPosition() 
{
	return position;
}

void Camera::setPosition(glm::vec3 p, glm::vec3 p2) {
	position.y = max(p.y, p2.y) * 0.70;
	position.z = min(55.0, abs(p2.y - p.y) * 2.5 + 25);
}

void Camera::applyProjectionMatrix(MatrixStack *P) const
{
	P->perspective(fovy, aspect, znear, zfar);
}

void Camera::applyViewMatrix(MatrixStack *MV) const
{
	// Create the translation and rotation matrices
	glm::mat4 T = glm::translate(position);
	glm::mat4 X = glm::rotate(-rotations.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Y = glm::rotate(-rotations.y, glm::vec3(1.0f, 0.0f, 0.0f));

	// The matrix C is the product of these matrices
	glm::mat4 C = T * X * Y; // Also apply rotations here
	// The view matrix is the inverse
	glm::mat4 V = glm::inverse(C);
	// Add to the matrix stack
	MV->multMatrix(V);
}
