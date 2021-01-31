#pragma once

#ifndef POINTTOSTRIANGLERELATIONSHIP_H
#define POINTTOSTRIANGLERELATIONSHIP_H

#include "STriangle.h"

class PointToSTriangleRelationship
{
	public:
		PointToSTriangleRelationship() {};
		PointToSTriangleRelationship(int in_sTriangleIndex, STriangle* in_sTriangleRef) :
			sTriangleIndex(in_sTriangleIndex),
			sTriangleRef(in_sTriangleRef)
		{};
		int sTriangleIndex = 0;
		STriangle* sTriangleRef = nullptr;
};

#endif
