#pragma once

#ifndef QUATUTILS_H
#define QUATUTILS_H

#include <glm/glm.hpp>
#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "QMBoolPointWithinTrianglePBZ.h"
#include "QMVec3FindOrientatedLineNormal.h"
#include "QMVec3FindCyclingDirectionPoint.h"
#include "QMBoolIsTJunctionCuttable.h"
#include "QMBoolAreLinesColinear.h"
#include "QMBoolDoLinesIntersect.h"
#include <iostream>

class QuatUtils
{
	public:
		static bool checkIfPointLiesWithinTrianglePBZ(glm::vec3 in_pointToCheck, glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
		static glm::vec3 findOrientatedLineNormal(glm::vec3 in_linePointA, glm::vec3 in_linePointB, glm::vec3 in_bearingPoint);
		static glm::vec3 findPointForDeterminingCyclingDirection(
																	glm::vec3 in_lineAPointA,
																	glm::vec3 in_lineAPointB,
																	glm::vec3 in_lineBPointA,
																	glm::vec3 in_lineBPointB,
																	glm::vec3 in_lineAOrientingNormal
																);
		static bool checkTJunctionUsability(
												glm::vec3 in_splitLinePointA,
												glm::vec3 in_splitLinePointB,
												glm::vec3 in_pointToCheck,
												glm::vec3 in_splitLineNormal
											);
		static bool checkIfLinesAreColinear(
												glm::vec3 in_lineAPointA,
												glm::vec3 in_lineAPointB,
												glm::vec3 in_lineBPointA,
												glm::vec3 in_lineBPointB
											);
		static bool areLinesIntersecting(
											glm::vec3* in_lineAPointA,
											glm::vec3* in_lineAPointB,
											glm::vec3* in_lineBPointA,
											glm::vec3* in_lineBPointB,
			                                glm::vec3* in_discoveredIntersectingPointStorage
										);
};

#endif
