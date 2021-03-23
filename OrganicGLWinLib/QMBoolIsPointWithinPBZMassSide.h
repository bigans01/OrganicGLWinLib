#pragma once

#ifndef QMBOOLISPOINTWITHINPBZMASSSIDE_H
#define QMBOOLISPOINTWITHINPBZMASSSIDE_H

#include "QMBoolBase.h"

class QMBoolIsPointWithinPBZMassSide : public QMBoolBase
{
	public:
		bool solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		void findAxisRotationsToGetFirstLineToXYPlane(QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef);
		void runRotationsToGetLineToXYPlane(QuatRotationPoints* in_quatRotationPointsRef, 
											std::vector<QuatRotationType>* in_rotationOrderVectorRef,
											std::stack<QuatRotationRecord>* in_rotationRecordStack);
		void rotateFirstTriangleLineAroundYAndPushIntoStack(QuatRotationPoints* in_quatRotationPointsRef, 
															std::stack<QuatRotationRecord>* in_rotationRecordStack);
		void rotateFirstTriangleLineAroundZAndPushIntoStack(QuatRotationPoints* in_quatRotationPointsRef,
															std::stack<QuatRotationRecord>* in_rotationRecordStack);
		void rotateTriangleAroundX(QuatRotationPoints* in_quatRotationPointsRef,
									std::stack<QuatRotationRecord>* in_rotationRecordStack);
		float findRadiansForRoateTriangleAroundX(glm::vec3 in_vec3);
		float roundZValueToTenThousandths(float in_z);
};

#endif
