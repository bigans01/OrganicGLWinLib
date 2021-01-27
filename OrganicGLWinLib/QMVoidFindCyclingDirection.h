#pragma once

#ifndef QMVOIDFINDCYCLINGDIRECTION_H
#define QMVOIDFINDCYCLINGDIRECTION_H

#include "QMVoidBase.h"
#include <stack>
#include "QuatRotationType.h"

class QMVoidFindCyclingDirection : public QMVoidBase
{
	public:
		void solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		
		QuatRotationPoints* rotationpointsRefVector = nullptr;
		glm::vec3* triangleNormalRef = nullptr;
		std::vector<QuatRotationType> rotationOrder;
		std::stack<QuatRotationRecord> rotationRecords;

		
		void runRotations(std::vector<QuatRotationType>* in_rotationOrderVecRef);

		void rotateEmptyNormalAroundYToPosZ();
		void rotateEmptyNormalAroundXToPosY();
		void rotateEmptyNormalAroundZToPosY();

		float findRotationRadiansForGettingToPosYThroughX(glm::vec3 in_vec3);
		float findRotationRadiansForGettingToPosYThroughZ(glm::vec3 in_vec3);
		
};

#endif
