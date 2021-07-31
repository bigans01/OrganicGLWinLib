#pragma once

#ifndef PTRIANGLERCPOINTTOINDEXLINK_H
#define PTRIANGLERCPOINTTOINDEXLINK_H

#include "RCollisionPoint.h"

class PTriangleRCPointToIndexLink
{
	public:
		PTriangleRCPointToIndexLink() {};
		PTriangleRCPointToIndexLink(RCollisionPoint* in_linkPointRef, int in_indexOfPointInPTriangle) :
			linkPointRef(in_linkPointRef),
			indexOfPointInPTriangle(in_indexOfPointInPTriangle)
		{};

		RCollisionPoint* linkPointRef = nullptr;
		int indexOfPointInPTriangle = 0;
};

#endif

