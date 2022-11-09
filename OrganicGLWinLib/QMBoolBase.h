#pragma once

#ifndef QMBOOLBASE_H
#define QMBOOLBASE_H

#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "QuatRotationType.h"
#include "QuatRotationRecord.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class QMBoolBase	// short for "Quaternion Machine"
{
	public:
		virtual bool solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel) = 0;
		glm::vec3 findTriangleCentroid(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
		glm::quat createQuaternion(float radians, glm::vec3 in_angle);
		void rotatePointsToOriginalPosition(std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef);
		void flipOnXAxis(QuatRotationPoints* in_quatRotationPointsRef);
		PolyLogger qmBoolBaseLogger;
		PolyDebugLevel qmBoolBaseLoggerDebugLevel = PolyDebugLevel::NONE;
};

#endif
