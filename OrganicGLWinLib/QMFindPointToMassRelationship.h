#pragma once

#ifndef QMFINDPOINTTOMASSRELATIONSHIP_H
#define QMFINDPOINTTOMASSRELATIONSHIP_H

#include "PointToMassRelationshipType.h"
#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "QuatRotationType.h"
#include "QuatRotationRecord.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class QMFindPointToMassRelationship
{
	public:
		PointToMassRelationshipType solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		PolyLogger qmRelationshipLogger;
		PolyDebugLevel qmRelationshipLoggerDebugLevel = PolyDebugLevel::NONE;

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
		float findRadiansForRotateTriangleAroundX(glm::vec3 in_vec3);
		glm::quat createQuaternion(float radians, glm::vec3 in_angle);
		float roundZValueToThousandths(float in_z);
};

#endif
