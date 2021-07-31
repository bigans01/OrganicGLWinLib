#pragma once

#ifndef RCOLLISIONPOINTTOPTRIANGLEMAPPER_H
#define RCOLLISIONPOINTTOPTRIANGLEMAPPER_H

#include "RCollisionPoint.h"
#include <map>

class RCollisionPointToPTriangleMapper
{
	public:
		RCollisionPointToPTriangleMapper() {};
		RCollisionPointToPTriangleMapper(RCollisionPoint* in_collisionPointRef) :
			collisionPointRef(in_collisionPointRef)
		{};

		void insertPTriangleToPointMapping(int in_pTriangleID, int in_pTrianglePointIndex)
		{
			pTrianglePointMapper[in_pTriangleID] = in_pTrianglePointIndex;
		}

		RCollisionPoint* fetchMapperCollisinPoint()
		{
			return collisionPointRef;
		}

	private:
		friend class RCollisionPointToPTriangleMapContainer;	// for debug printing.
		RCollisionPoint* collisionPointRef = nullptr;
		std::map<int, int> pTrianglePointMapper;	// key = the ID of the PTriangle, value = the ID of the point in the keyed PTriangle
};

#endif
