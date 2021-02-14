#pragma once

#ifndef QMBOOLISTJUNCTIONCUTTABLE_H
#define QMBOOLISTJUNCTIONCUTTABLE_H

#include "QMBoolBase.h"

class QMBoolIsTJunctionCuttable : public QMBoolBase
{
	public:
		bool solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		void rotateLineToYZeroPositiveX(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef);
};

#endif
