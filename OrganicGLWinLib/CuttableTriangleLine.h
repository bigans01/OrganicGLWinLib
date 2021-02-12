#pragma once

#ifndef CUTTABLETRIANGLELINE_H
#define CUTTABLETRIANGLELINE_H

#include <glm/glm.hpp>
#include "TwoDIntersectionRecordManager.h"

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
		void printCuttingIntersections()
		{
			if (cuttableIntersectionManager.numberOfRecords() > 0)
			{
				cuttableIntersectionManager.printRecordMap();
			}
		};
};

#endif
