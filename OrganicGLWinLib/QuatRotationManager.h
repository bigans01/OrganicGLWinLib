#pragma once

#ifndef QUATROTATIONMANAGER_H
#define QUATROTATIONMANAGER_H

#include "QuatRotationPoints.h"
#include "QuatRotationType.h"
#include "QuatRotationRecord.h"
#include "CyclingDirection.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <stack>

class QuatRotationManager
{
public:
	int debugFlag = 0;			// for debugging purposes 
	QuatRotationPoints* rotationpointsRefVector;
	glm::vec3 focusedLine;							// the line that we will be rotating until it's y and z values are equal (calculations are based around this line)
	glm::vec3* pointARef;
	glm::vec3* pointBRef;
	glm::vec3* pointCRef;
	glm::vec3* triangleNormalRef;					// a reference to the vec3 that is the calculate normal for a triangle
	std::vector<QuatRotationType> rotationOrder;	// stores the types of rotations that need to be executed
	std::stack<QuatRotationRecord> rotationRecords;

	void initializeAndRunForEmptyNormal(QuatRotationPoints* in_quatpointsRefVector);
	void initializeAndRunForZFracture(QuatRotationPoints* in_quatpointsRefVector);
	glm::vec3 initializeAndRunForPlanarSlide(QuatRotationPoints* in_quatpointsRefVector);
	void initializeAndRunForPlanarAlignmentToZ(QuatRotationPoints* in_quatpointsRefVector);
	CyclingDirection initializeAndRunForCyclingDirectionFinder(QuatRotationPoints* in_quatpointsRefVector);
	void initializeAndRunforAligningNeighboringCleaveSequencesToPosY(QuatRotationPoints* in_quatpointsRefVector);
	void initializeAndRunForFindingBorderLine(QuatRotationPoints* in_quatpointsRefVector);
	void initializeAndRunForFindingBorderLineEmptyNormal(QuatRotationPoints* in_quatpointsRefVector);

	void calculateEmptyNormal();					// find the empty normal (should only be run when all points of triangle are on y
	void executeRotationsForEmptyNormal();
	void executeRotationsForZFracture();
	void executeRotationsForCyclingDirectionFinder();
	void executeRotationsForPlanarSlide();
	void executeRotationsForFindingBorderLine();
	void executeRotationsForFindingBorderLineEmptyNormal();

	void rotateAroundYAndPushIntoStack();
	void rotateAroundYToPosZForPlanarSlideAndPushIntoStack();
	void rotateAroundXToYZeroForPlanarSlideAndPushIntoStack();
	void rotateAroundZAndPushIntoStack();
	void rotateAroundZToFindBorderLineEmptyNormalAndPushIntoStack();
	void rotateAroundZAndPushIntoStack(glm::vec3* in_point);
	void rotateAroundXToYZeroAndPushIntoStack();
	void rotateAroundXForZFractureAndPushIntoStack();
	void rotateAroundXForPositiveYBorderLineAndPushIntoStack();

	void rotateEmptyNormalToPosY(glm::vec3* in_normal);
	void rotateAroundZForPosYNormalAndPushIntoStack(glm::vec3 in_normal);

	float findRotationRadiansForZFracture(glm::vec3 in_vec3);
	float findRotationRadiansForGettingToPosYThroughZ(glm::vec3 in_vec3);
	float findRotationRadainsForGettingToPosXThroughY(glm::vec3 in_vec3);

	glm::quat createQuaternion(float radians, glm::vec3 in_angle);
	void rotateToOriginalPosition();
	void setDebugFlag(int in_debugFlag);
	glm::vec3 checkForEmptyNormalCorrection(glm::vec3 in_mrpCopy, glm::vec3 in_normalCopy);
	glm::vec3 getEmptyNormal();						// returns the calculated empty normal
};

#endif
