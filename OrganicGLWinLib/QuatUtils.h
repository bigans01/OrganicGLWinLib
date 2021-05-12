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
#include "QMBoolIsColinearLineAContainedWithinB.h"
#include "QMBoolIsPointWithinPBZMassSide.h"
#include <iostream>
#include "Vec3Result.h"
#include "PolyDebugLevel.h"
#include "PointToMassRelationshipType.h"
#include "QMFindPointToMassRelationship.h"
#include "SPolyScalingOps.h"
#include "QMVoidAlignTriangleToXYPlane.h"
#include "QMVoidTranslatePointsForRasterization.h"

class QuatUtils
{
	public:
		static bool checkIfPointLiesWithinTrianglePBZ(glm::vec3 in_pointToCheck, glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
		static bool checkIfPointLiesWithinTrianglePBZDebug(glm::vec3 in_pointToCheck, glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2, PolyDebugLevel in_polyDebugLevel);
		static glm::vec3 findOrientatedLineNormal(glm::vec3 in_linePointA, glm::vec3 in_linePointB, glm::vec3 in_bearingPoint);
		static Vec3Result findPointForDeterminingCyclingDirection(
																	glm::vec3 in_lineAPointA,
																	glm::vec3 in_lineAPointB,
																	glm::vec3 in_lineBPointA,
																	glm::vec3 in_lineBPointB,
																	glm::vec3 in_lineAOrientingNormal,
																	PolyDebugLevel in_polyDebugLevel
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
												glm::vec3 in_lineBPointB,
												PolyDebugLevel in_polyDebugLevel
											);
		static bool areLinesIntersecting(
											glm::vec3* in_lineAPointA,
											glm::vec3* in_lineAPointB,
											glm::vec3* in_lineBPointA,
											glm::vec3* in_lineBPointB,
			                                glm::vec3* in_discoveredIntersectingPointStorage,
											PolyDebugLevel in_polyDebugLevel
										);
		static bool isLineAContainedWithinB(
											glm::vec3 in_lineAPointA,
											glm::vec3 in_lineAPointB,
											glm::vec3 in_lineBPointA,
											glm::vec3 in_lineBPointB,
											PolyDebugLevel in_polyDebugLevel
										);
		static bool isPointWithinMassOfPBZ(
											glm::vec3 in_pointToCompare,
											glm::vec3 in_trianglePoint0,
											glm::vec3 in_trianglePoint1,
											glm::vec3 in_trianglePoint2,
											glm::vec3 in_triangleEmptyNormal,
											PolyDebugLevel in_polyDebugLevel
										);
		static PointToMassRelationshipType findPointToMassRelationship(
											glm::vec3 in_pointToCompare,
											glm::vec3 in_trianglePoint0,
											glm::vec3 in_trianglePoint1,
											glm::vec3 in_trianglePoint2,
											glm::vec3 in_triangleEmptyNormal,
											PolyDebugLevel in_polyDebugLevel
										);
		static void rotateSPolyToXYPlanAndRecord(SPolyScalingOps* in_scalingOpsRef,
												 QuatRotationPoints* in_quatRotationPointsRef);
		static void rotatePointsForRasterization(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
};

#endif
