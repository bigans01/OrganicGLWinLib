#include "stdafx.h"
#include "QuatUtils.h"

bool QuatUtils::checkIfPointLiesWithinTrianglePBZ(glm::vec3 in_pointToCheck, glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	QuatRotationPoints points;	// first point = point to check, remaining 3 = points of triangle (see header file for QMBoolPointWithinTrianglePBZ)
	points.insertPointRefs(&in_pointToCheck, &in_point0, &in_point1, &in_point2);
	QMBoolPointWithinTrianglePBZ pointSolver;
	return pointSolver.solve(&points, PolyDebugLevel::NONE);
}

glm::vec3 QuatUtils::findOrientatedLineNormal(glm::vec3 in_linePointA, glm::vec3 in_linePointB, glm::vec3 in_bearingPoint)
{
	QuatRotationPoints points;
	points.insertPointRefs(&in_linePointA, &in_linePointB, &in_bearingPoint);
	QMVec3FindOrientatedLineNormal orientatedSolver;
	return orientatedSolver.solve(&points, PolyDebugLevel::NONE);
}

glm::vec3 QuatUtils::findPointForDeterminingCyclingDirection
(
	glm::vec3 in_lineAPointA,
	glm::vec3 in_lineAPointB,
	glm::vec3 in_lineBPointA,
	glm::vec3 in_lineBPointB,
	glm::vec3 in_lineAOrientingNormal
)
{
	QuatRotationPoints points;
	points.insertPointRefs(
								&in_lineAPointA,
								&in_lineAPointB,
								&in_lineBPointA,
								&in_lineBPointB,
								&in_lineAOrientingNormal
							);
	QMVec3FindCyclingDirectionPoint cyclingPointSolver;
	return cyclingPointSolver.solve(&points, PolyDebugLevel::NONE);
}

bool QuatUtils::checkTJunctionUsability
(
	glm::vec3 in_splitLinePointA,
	glm::vec3 in_splitLinePointB,
	glm::vec3 in_pointToCheck,
	glm::vec3 in_splitLineNormal
)
{
	QuatRotationPoints points;
	points.insertPointRefs(
								&in_splitLinePointA,
								&in_splitLinePointB,
								&in_pointToCheck,
								&in_splitLineNormal
							);
	QMBoolIsTJunctionCuttable tJunctionCuttableSolver;
	return tJunctionCuttableSolver.solve(&points, PolyDebugLevel::NONE);
}

bool QuatUtils::checkIfLinesAreColinear
(
	glm::vec3 in_lineAPointA,
	glm::vec3 in_lineAPointB,
	glm::vec3 in_lineBPointA,
	glm::vec3 in_lineBPointB
)
{
	QuatRotationPoints points;
	points.insertPointRefs(
								&in_lineAPointA,
								&in_lineAPointB,
								&in_lineBPointA,
								&in_lineBPointB
							);
	QMBoolAreLinesColinear areLinesColinear;
	return areLinesColinear.solve(&points, PolyDebugLevel::NONE);
}

bool QuatUtils::areLinesIntersecting(
	glm::vec3* in_lineAPointA,
	glm::vec3* in_lineAPointB,
	glm::vec3* in_lineBPointA,
	glm::vec3* in_lineBPointB,
	glm::vec3* in_discoveredIntersectingPointStorage
)
{
	QuatRotationPoints points;
	points.insertPointRefs(
								in_lineAPointA,
								in_lineAPointB,
								in_lineBPointA,
								in_lineBPointB,
		                        in_discoveredIntersectingPointStorage
							);
	QMBoolDoLinesIntersect doLinesIntersect;
	return doLinesIntersect.solve(&points, PolyDebugLevel::NONE);
}
