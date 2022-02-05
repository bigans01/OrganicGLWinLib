#pragma once

#ifndef YDIMLINE_H
#define YDIMLINE_H

#include "OneDimLineBase.h"

class YDimLine : public OneDimLineBase
{
public:
	// For an Y dim line, immutableDimA should be a X value, and immutableDimB should be a Z value.

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
			(in_pointToCompare.z == immutableDimB)
		)
		{
			// The Y value of the point must be >= 0, AND <= the dimLimit
			if
			(
				(in_pointToCompare.y >= 0.0f)
				&&
				(in_pointToCompare.y <= dimLimit)
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
		immutableDimB = in_otherDimImmutableLocB;		// B is Z.

	}

	void printMetaData()
	{
		std::cout << "Y-Dim | X-value: " << immutableDimA << " | Z-value: " << immutableDimB << std::endl;
	};

private:
};

#endif