#include "stdafx.h"
#include "QuatUtils.h"

bool QuatUtils::checkIfPointLiesWithinTrianglePBZ(glm::vec3 in_pointToCheck, glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	QuatRotationPoints points;	// first point = point to check, remaining 3 = points of triangle (see header file for QMBoolPointWithinTrianglePBZ)
	points.insertPointRefs(&in_pointToCheck, &in_point0, &in_point1, &in_point2);
	QMBoolPointWithinTrianglePBZ pointSolver;
	return pointSolver.solve(&points, PolyDebugLevel::NONE);
}