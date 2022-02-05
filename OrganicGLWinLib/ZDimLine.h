#pragma once

#ifndef ZDIMLINE_H
#define ZDIMLINE_H

#include "OneDimLineBase.h"

class ZDimLine : public OneDimLineBase
{
public:
	// For an Z dim line, immutableDimA should be a X value, and immutableDimB should be a Y value.

	// required virtual functions
	bool isPointWithinLine(glm::vec3 in_pointToCompare)
	{
		bool isWithin = false;

		// Check 1: the Y and Z values of the point must be equal to the immuable dims; be sure that the point are rounded, if they
		// aren't already.
		if
		(
			(in_pointToCompare.x == immutableDimA)
			&&
			(in_pointToCompare.y == immutableDimB)
		)
		{
			// The X value of the point must be >= 0, AND <= the dimLimit
			if
			(
				(in_pointToCompare.z >= 0.0f)
				&&
				(in_pointToCompare.z <= dimLimit)
			)
			{
				isWithin = true;
			}
		}
		return isWithin;
	}
	void setStaticDims(float in_otherDimImmutableLocA, float in_otherDimImmutableLocB)
	{
		immutableDimA = in_otherDimImmutableLocA;		// A is X.
		immutableDimB = in_otherDimImmutableLocB;		// B is Y.

	}

	void printMetaData()
	{
		std::cout << "Z-Dim | X-value: " << immutableDimA << " | Y-value: " << immutableDimB << std::endl;
	};
private:
};

#endif
