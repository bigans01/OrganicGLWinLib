#pragma once

#ifndef CUTTABLETRIANGLELINE_H
#define CUTTABLETRIANGLELINE_H

#include <glm/glm.hpp>
#include "TwoDIntersectionRecordManager.h"
#include <map>
#include "CyclingDirection.h"
#include "IntersectionFindingAttempt.h"

class CuttableTriangleLine
{
	private:
		friend class CuttableTriangle;
		friend class CutLineWelder;
		CuttableTriangleLine() {};
		CuttableTriangleLine(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_centroidFacingNormal) :
			pointA(in_pointA),
			pointB(in_pointB),
			cuttableTriangleCentroidFacingNormal(in_centroidFacingNormal)
		{}

		glm::vec3 pointA;
		glm::vec3 pointB;
		glm::vec3 cuttableTriangleCentroidFacingNormal;
		TwoDIntersectionRecordManager cuttableIntersectionManager;
		std::map<int, glm::vec3> cuttingLineNonIntersectingPoints;
	
		void printCuttingIntersections();
		void insertNonIntersectingCuttingLinePoint(int in_cuttingLineID, glm::vec3 in_point);
		IntersectionFindingAttempt getNonIntersectingPountForCuttingLine(int in_cuttingLineID, glm::vec3 in_currentLeadingPoint);
		glm::vec3 getNonIntersectingPountForCuttingLine(int in_cuttingLineID);
		glm::vec3 fetchNextPointBasedOnCyclingDirection(CyclingDirection in_cyclingDirection);
		glm::vec3 getOtherEndpoint(glm::vec3 in_currentEndpoint);

};

#endif
