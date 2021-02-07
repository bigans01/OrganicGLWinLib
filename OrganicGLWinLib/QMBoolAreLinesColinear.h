#pragma once

#ifndef QMBOOLARELINESCOLINEAR_H
#define QMBOOLARELINESCOLINEAR_H

#include "QMBoolBase.h"

class QMBoolAreLinesColinear : public QMBoolBase
{
	public:
		bool solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		void rotateLineToYZeroPositiveX(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderStackRef);
		bool runCoplanarCheck(glm::vec3 in_upwardNormalRef, glm::vec3 in_pointToCompareTo);
		float roundRadiansForRightAngleCheck(float in_angleInRadians);
		void rotateAroundZToYZero(glm::vec3* in_upwardNormalRef, QuatRotationPoints* in_quatRotationPointsRef, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef);
};

#endif

