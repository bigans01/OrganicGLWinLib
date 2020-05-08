#pragma once

#ifndef QUATROTATIONPOINTS_H
#define QUATROTATIONPOINTS_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <iostream>

class QuatRotationPoints
{
public:
	std::vector<glm::vec3*> pointsRef;	// contains the points to cycle through
	void applyQuaternion(glm::quat in_quat);
	void applyTranslation(glm::vec3 in_translation);
	void printPoints();
	glm::vec3 getFirstPoint();
	glm::vec3 getSecondPoint();
	glm::vec3* getFirstPointRef();
	glm::vec3* getSecondPointRef();
	glm::vec3* getThirdPointRef();
	glm::vec3* getMRPRef();
	glm::vec3* getNormalRef();
};

#endif