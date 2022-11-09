#pragma once

#ifndef QMVOIDBASE_H
#define QMVOIDBASE_H

#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "QuatRotationType.h"
#include "QuatRotationRecord.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class QMVoidBase
{
	public:
		virtual void solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel) = 0;
		PolyLogger qmVoidBaseLogger;
		PolyDebugLevel qmVoidBaseLoggerDebugLevel = PolyDebugLevel::NONE;
		glm::quat createQuaternion(float radians, glm::vec3 in_angle);
};

#endif