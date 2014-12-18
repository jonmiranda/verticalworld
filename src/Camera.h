//
// sueda
// November, 2014
//

#pragma  once
#ifndef __Camera__
#define __Camera__

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

class MatrixStack;

class Camera
{
public:
	Camera();
	virtual ~Camera();
	void setWindowSize(float w, float h);
	void update(const bool *keys, const glm::vec2 &mouse);
	void applyProjectionMatrix(MatrixStack *P) const;
	void applyViewMatrix(MatrixStack *MV) const;
	glm::vec3 getPosition();
	void setPosition(glm::vec3 p, glm::vec3 p2);
	
private:
	float aspect;
	float fovy;
	float znear;
	float zfar;
	glm::vec2 mousePrev;
	float angleX;
	float angleY;
	glm::vec3 position;
	
	float rfactor;
	glm::vec2 rotations;
};

#endif
