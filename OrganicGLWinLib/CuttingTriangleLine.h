#pragma once

#ifndef CUTTINGTRIANGLELINE_H
#define CUTTINGTRIANGLELINE_H

#include "STriangleLine.h"

class CuttingTriangleLine
{
	friend class CuttingTriangle;
	CuttingTriangleLine() {};
	CuttingTriangleLine(STriangleLine in_sTriangleLine) :
		pointA(in_sTriangleLine.pointA),
		pointB(in_sTriangleLine.pointB)
	{}
	glm::vec3 pointA;
	glm::vec3 pointB;
};

#endif
