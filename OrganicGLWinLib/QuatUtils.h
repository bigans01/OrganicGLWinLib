#pragma once

#ifndef QUATUTILS_H
#define QUATUTILS_H

#include <glm/glm.hpp>
#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "QMBoolPointWithinTrianglePBZ.h"

class QuatUtils
{
	public:
		static bool checkIfPointLiesWithinTrianglePBZ(glm::vec3 in_pointToCheck, glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
};

#endif
