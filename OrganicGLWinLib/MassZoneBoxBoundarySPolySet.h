#pragma once

#ifndef MASSZONEBOXBOUNDARYSPOLYSET_H
#define MASSZONEBOXBOUNDARYSPOLYSET_H

#include "SPoly.h"
#include "STriangle.h"
#include "IntersectionLineGroup.h"
#include "IntersectionResult.h"
#include "IntersectionLine.h"
#include "CategorizedLineColinearTester.h"
#include "PolyLogger.h"


class MassZoneBoxBoundarySPolySet
{
	public:
		void setBoundarySPolyRef(SPoly* in_sPolyRef);
		void compareSPolySubZoneSPolyToBoundarySPoly(SPoly* in_sPolyRef);
		void insertCategorizedLinesFromNonboundarySPoly(SPoly* in_sPolyRef);
	private:
		PolyLogger boxBoundaryLogger;
		SPoly* boundarySPolyRef = nullptr;	// the SPoly in the boundary that we will be inserting categorized lines into.
		int currentComparableSPolyIndex = 1;	// this index should always start at 1.
		IntersectionResult checkIfLineIntersectsTriangle(STriangle in_triangle, STriangleLine in_line);
		IntersectionResult checkIfRayIntersectsTriangle(STriangle in_triangle, STriangleLine in_triangleLine);
		glm::vec3 roundPointToHundredths(glm::vec3 in_point);
		double doubledot(glm::vec3 in_A, glm::vec3 in_B);
		CategorizedLine determineCategorizedLineThroughHostTriangleContext(IntersectionLine in_hostLine, IntersectionLine in_guestLine, int in_groupID, glm::vec3 in_polyBEmptyNormal);
		bool checkForSamePointCondition(IntersectionLine in_lineA, IntersectionLine in_lineB);
		glm::vec3 findSecondPointForLine(glm::vec3 in_beginPoint, glm::vec3 in_candidate1, glm::vec3 in_candidate2);
		int checkIfPointsMatch(glm::vec3 in_pointA, glm::vec3 in_pointB);
};

#endif
