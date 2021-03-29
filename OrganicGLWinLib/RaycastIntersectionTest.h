#pragma once

#ifndef RAYCASTINTERSECTIONTEST_H
#define RAYCASTINTERSECTIONTEST_H

#include "Ray.h"
#include "STriangle.h"
#include "STriangleLine.h"
#include "RayIntersectionResult.h"
#include "QuatUtils.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class RaycastIntersectionTest
{
	public:
		RaycastIntersectionTest(Ray in_rayToCast, STriangle in_sTriangleToTest) :
			rayToCast(in_rayToCast),
			sTriangleToCastAgainst(in_sTriangleToTest)
		{
			STriangleLine lineToTest;
			lineToTest.pointA = rayToCast.raySegmentBeginPoint;
			lineToTest.pointB = rayToCast.raySegmentEndPoint;
			producedResult = runIntersectionTest(sTriangleToCastAgainst, lineToTest);
		}
	private:
		friend class MassZoneShellSlice;
		Ray rayToCast;
		STriangle sTriangleToCastAgainst;
		RayIntersectionResult producedResult;
		PolyLogger rayCastLogger;
		PolyDebugLevel rayCastLoggerLogLevel = PolyDebugLevel::NONE;

		RayIntersectionResult runIntersectionTest(STriangle in_triangle, STriangleLine in_line);
		double doubledot(glm::vec3 in_A, glm::vec3 in_B);
		bool isPointEqualToTrianglePoint(glm::vec3 in_point, STriangle* in_triangleRef);
		

};

#endif
