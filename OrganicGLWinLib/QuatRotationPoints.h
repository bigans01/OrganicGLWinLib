#pragma once

#ifndef QUATROTATIONPOINTS_H
#define QUATROTATIONPOINTS_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <iostream>
//#include "OrganicGLWinUtils.h"

class OrganicGLWinUtils;
class QuatRotationPoints
{
public:
	template<typename FirstOption, typename ...RemainingOptions> void insertPointRefs(FirstOption && firstOption, RemainingOptions && ...optionParams)
	{
		if constexpr
		(
			std::is_same<FirstOption, glm::vec3*>::value
		)
		{
			pointsRefVector.push_back(std::forward<FirstOption>(firstOption));
			insertPointRefs(std::forward<RemainingOptions>(optionParams)...);
		}
	}
	void insertPointRefs() {};

	std::vector<glm::vec3*> pointsRefVector;	// contains the points to cycle through
	void applyQuaternion(glm::quat in_quat);
	void applyTranslation(glm::vec3 in_translation);
	void applyTranslationToIndexRange(glm::vec3 in_translation, int in_indexStart, int in_indexEnd);
	void printTrianglePoints();
	void applyNormalization(float in_normalizationValue);
	void applyDimensionCorrections();
	void printPoints();
	void clearPoints();
	void roundAllPointsToHundredths();
	glm::vec3 getFirstPoint();
	glm::vec3 getLastPoint();
	glm::vec3 getSecondPoint();
	glm::vec3* getFirstPointRef();
	glm::vec3* getSecondPointRef();
	glm::vec3* getThirdPointRef();
	glm::vec3* getMRPRef();
	glm::vec3* getNormalRef();
	glm::vec3 getPointByIndex(int in_index);
	glm::vec3* getPointRefByIndex(int in_index);
};

#endif