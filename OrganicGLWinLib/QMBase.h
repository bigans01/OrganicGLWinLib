#pragma once

#ifndef QMBASE_H
#define QMBASE_H

#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"
#include <glm/glm.hpp>

class QMBase	// short for "Quaternion Machine"
{
	public:
		virtual bool solve(QuatRotationPoints* in_quatRotationPointsRef) = 0;
		glm::vec3 findTriangleCentroid(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
};

#endif
