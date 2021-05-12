#pragma once

#ifndef QMVOIDTRANSLATEPOINTSFORRASTERIZATION_H
#define QMVOIDTRANSLATEPOINTSFORRASTERIZATION_H

#include "QMVoidBase.h"
#include <stack>
#include "QuatRotationType.h"

class QMVoidTranslatePointsForRasterization	: public QMVoidBase
{
	public:
		void solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		QuatRotationPoints* rotationpointsRefVector = nullptr;
		glm::vec3* emptyNormalRef = nullptr;
		std::vector<QuatRotationType> rotationOrder;
		std::stack<QuatRotationRecord> rotationRecords;

		void rotateEmptyNormalAroundXToYZero();
		float findRotationRadiansForGettingToYZeroThroughX(glm::vec3 in_vec3);

		void rotateEmptyNormalAroundYToPosZ();
		float findRotationRadiansForGettingToPosZThroughY(glm::vec3 in_vec3);

		void runRotations();
};

#endif
