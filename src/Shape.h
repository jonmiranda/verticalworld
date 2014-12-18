//
// sueda
// October, 2014
//

#pragma once
#ifndef _SHAPE_H_
#define _SHAPE_H_

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif
#include "tiny_obj_loader.h"
#include "MatrixStack.h"

class Shape
{
public:
	Shape();
	virtual ~Shape();
	void load();
	void setRotationAngle(float angle);
	void setRotationAxis(glm::vec3 axis);
	void setTranslationVec(glm::vec3 vec);
	void setScaleVec(glm::vec3 vec);
	void setMeshName(std::string name);
	void setColor(glm::vec3 color);
	void init();
	void draw(GLint h_pos, GLint h_nor, GLint h_MV, GLint h_kd, MatrixStack* MV);
	glm::vec3 getTranslationVec();
	// Public so Particle can access
	glm::vec3 x; // position
	glm::vec3 color;

	glm::vec3 translationVec;
	bool hasTexture;
	float rotationAngle;
	int id;

private:
	std::vector<tinyobj::shape_t> shapes;
	glm::vec3 rotationAxis;
	glm::vec3 scaleVec;
	GLuint posBufID;
	GLuint norBufID;
	GLuint indBufID;
	std::string meshName;

	float getRotationAngle();
};

#endif
