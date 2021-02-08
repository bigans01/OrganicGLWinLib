#pragma once

#ifndef CUTTINGTRIANGLELINE_H
#define CUTTINGTRIANGLELINE_H

#include <glm/glm.hpp>
#include "TwoDIntersectionRecordManager.h"

class CuttingTriangleLine
{
	private:
		friend class CuttableTriangle;
		friend class CuttingTriangle;
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
};

#endif
