#pragma once

#ifndef EMPTYNORMALFINDER_H
#define EMPTYNORMALFINDER_H

#include <glm/glm.hpp>
#include <iostream>
#include "QuatRotationRecord.h"
#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "PointTranslationCheck.h"

class EmptyNormalFinder
{
public:

	glm::vec3 point0;	// the points
	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 massReferencePoint;
	glm::vec3 calculatedNormal;
	QuatRotationPoints quatPoints;
	QuatRotationManager rotationManager;
	PointTranslationCheck pointTranslator;

	EmptyNormalFinder(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2, glm::vec3 in_massReferencePoint);
	float getRadiansForXAlignmentS1(glm::vec3 in_vec3);
};

#endif
