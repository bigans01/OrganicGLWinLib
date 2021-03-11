#pragma once

#ifndef QMBOOLDOLINESINTERSECT_H
#define QMBOOLDOLINESINTERSECT_H

#include "QMBoolBase.h"
#include <iomanip>

class QMBoolDoLinesIntersect : public QMBoolBase
{
	public:
		bool solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		void rotateLineToYZeroPositiveX(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef);
		void roundVec3YByTenThousandths(glm::vec3* in_vec3Ref);
		bool checkForExactPointMatch(QuatRotationPoints* in_quatRotationPointsRef);

};

#endif