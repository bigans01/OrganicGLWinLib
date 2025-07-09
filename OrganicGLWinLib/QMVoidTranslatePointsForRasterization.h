#pragma once

#ifndef QMVOIDTRANSLATEPOINTSFORRASTERIZATION_H
#define QMVOIDTRANSLATEPOINTSFORRASTERIZATION_H

#include "QMVoidBase.h"
#include "QuatRotationType.h"

class QMVoidTranslatePointsForRasterization	: public QMVoidBase
{
	public:
		void solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		QuatRotationPoints* rotationpointsRefVector = nullptr;
		//glm::vec3* emptyNormalRef = nullptr;
		glm::vec3 emptyNormalRef;
		std::vector<QuatRotationType> rotationOrder;
		std::stack<QuatRotationRecord> rotationRecords;

		void rotateEmptyNormalAroundXToYZero();
		void findRotationRadiansForGettingToYZeroThroughX(glm::vec3 in_vec3);

		void rotateEmptyNormalAroundYToPosZ();
		float findRotationRadiansForGettingToPosZThroughY(glm::vec3 in_vec3);

		void runRotations();
};

#endif
