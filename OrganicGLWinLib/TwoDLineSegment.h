#pragma once
#ifndef TwoDLineSegment_H
#define TwoDLineSegment_H

#include "TwoDLineSegmentIntersectResult.h"
#include "TwoDPoint.h"
#include "STriangle.h"
#include "STriangleLine.h"
#include <vector>
#include "TwoDSPolyIntersectionRecord.h"
#include "TwoDSPolyIntersectionType.h"
#include "CategorizedLine.h"
#include "OrganicGLWinUtils.h"

class TwoDLineSegment
{
	public:
		TwoDLineSegment() {};
		TwoDLineSegment(float in_Ax, float in_Ay, float in_Bx, float in_By) 
		{
			a.x = in_Ax;
			a.y = in_Ay;
			b.x = in_Bx;
			b.y = in_By;
		};
		TwoDLineSegment(TwoDPoint in_twoDPointA, TwoDPoint in_twoDPointB)
		{
			a.x = in_twoDPointA.x;
			a.y = in_twoDPointA.y;
			b.x = in_twoDPointB.x;
			b.y = in_twoDPointB.y;
		}
		TwoDPoint a;
		TwoDPoint b;
		bool containsCategorizedLine = false;	// set to true if/when a valid categorized line is determined.
		bool isColinearToAnotherLine = false;	// set to true if this line segment is colinear to any other line in the host triangle it's compared to

		void attemptIntersectionInsert(TwoDLineSegmentIntersectResult in_result, STriangleLine* in_triangleLineRef);
		CategorizedLine attemptCategorizedLineConstruction(glm::vec3 in_guestTriangleCentroid, STriangle* in_hostSTrianglePtr);
		glm::vec3 determineCoplanarCategorizedLineEmptyNormal(glm::vec3 in_guestTriangleCentroid, glm::vec3 in_pointA, glm::vec3 in_pointB);
	private:
		std::vector<TwoDSPolyIntersectionRecord> intersectionRecords;
		int numberOfIntersectedBorderLines = 0;
		int numberOfIntersectedNonBorderLines = 0;
		void insertIntersectionRecord(TwoDSPolyIntersectionType in_intersectionType, bool in_isOnBorderLine, unsigned char in_intersectdBorderLineID, TwoDPoint in_intersectedPoint);
		bool checkIf2dPointsMatch(TwoDPoint in_pointA, TwoDPoint in_pointB);
};

#endif
