#pragma once

#ifndef QMBOOLDOLINESINTERSECT_H
#define QMBOOLDOLINESINTERSECT_H

#include "QMBoolBase.h"

class QMBoolDoLinesIntersect : public QMBoolBase
{
	public:
		bool solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		void rotateLineToYZeroPositiveX(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef);

};

#endif