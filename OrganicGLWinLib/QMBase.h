#pragma once

#ifndef QMBASE_H
#define QMBASE_H

#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "QuatRotationType.h"
#include "QuatRotationRecord.h"

class QMBase	// short for "Quaternion Machine"
{
	public:
		virtual bool solve(QuatRotationPoints* in_quatRotationPointsRef) = 0;
		glm::vec3 findTriangleCentroid(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
		//std::vector<QuatRotationType> rotationOrder;	// stores the types of rotations that need to be executed
		//std::stack<QuatRotationRecord> rotationRecords;
		glm::quat createQuaternion(float radians, glm::vec3 in_angle);
		void rotatePointsToOriginalPosition(std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef);
		void flipOnXAxis(QuatRotationPoints* in_quatRotationPointsRef);
};

#endif
