#pragma once

#ifndef QMVEC3RESULTBASE_H
#define QMVEC3RESULTBASE_H

#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "QuatRotationType.h"
#include "QuatRotationRecord.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "Vec3Result.h"

class QMVec3ResultBase
{
	public:
		virtual Vec3Result solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel) = 0;
		glm::quat createQuaternion(float radians, glm::vec3 in_angle);
		void rotatePointsToOriginalPosition(std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef);
		void flipOnXAxis(QuatRotationPoints* in_quatRotationPointsRef);
		PolyLogger qmVec3BaseLogger;
		PolyDebugLevel qmVec3BaseLoggerDebugLevel = PolyDebugLevel::NONE;
};

#endif
