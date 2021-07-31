#pragma once

#ifndef PTRIANGLEMAPPERUPDATE_H
#define PTRIANGLEMAPPERUPDATE_H

#include "RCollisionPoint.h"

class PTriangleMapperUpdate
{
	public:
		PTriangleMapperUpdate() {};	
		PTriangleMapperUpdate(RCollisionPoint* in_collisionPoint, int in_targetPTriangleID, int in_targetPointInPTriangle) :
			mapperUpdateCollisionPointRef(in_collisionPoint),
			targetPTriangleID(in_targetPTriangleID),
			targetPointInPTriangle(in_targetPointInPTriangle)
		{};
		RCollisionPoint* mapperUpdateCollisionPointRef = nullptr;
		int targetPTriangleID = 0;
		int targetPointInPTriangle = 0;
};

#endif
