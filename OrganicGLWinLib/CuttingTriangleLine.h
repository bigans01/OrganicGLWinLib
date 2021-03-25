#pragma once

#ifndef CUTTINGTRIANGLELINE_H
#define CUTTINGTRIANGLELINE_H

#include <glm/glm.hpp>
#include "TwoDIntersectionRecordManager.h"
#include "CyclingDirection.h"

class CuttingTriangleLine
{
	private:
		friend class CuttableTriangle;
		friend class CuttingTriangle;
		friend class CutLineWelder;
		CuttingTriangleLine() {};
		CuttingTriangleLine(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_outwardFacingNormal) :
			pointA(in_pointA),
			pointB(in_pointB),
			outwardFacingNormal(in_outwardFacingNormal)
		{}
		glm::vec3 pointA;
		glm::vec3 pointB;
		glm::vec3 outwardFacingNormal;
		TwoDIntersectionRecordManager cuttingIntersectionManager;
		void printCuttableIntersections()
		{
			if (cuttingIntersectionManager.numberOfRecords() > 0)
			{
				cuttingIntersectionManager.printRecordMap();
			}
		};

		CyclingDirection determineCyclingDirectionToUseFromPoint(glm::vec3 in_point)	
		{
			CyclingDirection returnDirection;
			if (in_point == pointA)
			{
				returnDirection = CyclingDirection::REVERSE;
			}
			else if (in_point == pointB)
			{
				returnDirection = CyclingDirection::FORWARD;
			}
			return returnDirection;
		}

		CyclingDirection determineCyclingDirectionToUseFromFailedAttempt(glm::vec3 in_point)	// created for CUTLINEWELDER_TYPICAL-0
		{
			CyclingDirection returnDirection;
			if (in_point == pointA)
			{
				returnDirection = CyclingDirection::FORWARD;
			}
			else if (in_point == pointB)
			{
				returnDirection = CyclingDirection::REVERSE;
			}
			return returnDirection;
		}

		void resetRecordManager()
		{
			cuttingIntersectionManager.clearRecordMap();
		}
};

#endif
