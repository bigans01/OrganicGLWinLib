#pragma once

#ifndef CUTTABLETRIANGLELINE_H
#define CUTTABLETRIANGLELINE_H

#include <glm/glm.hpp>
#include "TwoDIntersectionRecordManager.h"
#include <map>
#include "CyclingDirection.h"

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
	
		void printCuttingIntersections()
		{
			if (cuttableIntersectionManager.numberOfRecords() > 0)
			{
				cuttableIntersectionManager.printRecordMap();
			}
		};
		void insertNonIntersectingCuttingLinePoint(int in_cuttingLineID, glm::vec3 in_point)
		{
			cuttingLineNonIntersectingPoints[in_cuttingLineID] = in_point;
		};
		glm::vec3 getNonIntersectingPountForCuttingLine(int in_cuttingLineID)
		{
			return cuttingLineNonIntersectingPoints[in_cuttingLineID];
		}
		glm::vec3 fetchNextPointBasedOnCyclingDirection(CyclingDirection in_cyclingDirection)
		{
			if (in_cyclingDirection == CyclingDirection::FORWARD)
			{
				return pointB;
			}
			else if (in_cyclingDirection == CyclingDirection::REVERSE)
			{
				return pointA;
			}
		}

		glm::vec3 pointA;
		glm::vec3 pointB;
		glm::vec3 cuttableTriangleCentroidFacingNormal;
		TwoDIntersectionRecordManager cuttableIntersectionManager;
		std::map<int, glm::vec3> cuttingLineNonIntersectingPoints;
};

#endif
