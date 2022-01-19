#include "stdafx.h"
#include "RCollisionPoint.h"

void RCollisionPoint::incrementUsageCount()
{
	usageCount++;
	if (usageCount == 8)
	{
		pointState = RCollisionPointState::IMMUTABLE;
	}
};