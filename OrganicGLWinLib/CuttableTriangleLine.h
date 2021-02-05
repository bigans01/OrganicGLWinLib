#pragma once

#ifndef CUTTABLETRIANGLELINE_H
#define CUTTABLETRIANGLELINE_H

#include "STriangleLine.h"

class CuttableTriangleLine
{
	//public:
	private:
		friend class CuttableTriangle;
		CuttableTriangleLine() {};
		CuttableTriangleLine(STriangleLine in_sTriangleLine) :
			pointA(in_sTriangleLine.pointA),
			pointB(in_sTriangleLine.pointB)
		{}
		glm::vec3 pointA;
		glm::vec3 pointB;
		glm::vec3 cuttableTriangleCentroidFacingNormal;
};

#endif
