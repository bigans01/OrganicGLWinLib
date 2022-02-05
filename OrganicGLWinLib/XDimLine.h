#pragma once

#ifndef XDIMLINE_H
#define XDIMLINE_H

#include "OneDimLineBase.h"
#include <iostream>

class XDimLine : public OneDimLineBase
{
	public:
		// For an X dim line, immutableDimA should be a Y value, and immutableDimB should be a Z value.

		// required virtual functions
		bool isPointWithinLine(glm::vec3 in_pointToCompare)
		{
			bool isWithin = false;

			// Check 1: the Y and Z values of the point must be equal to the immuable dims; be sure that the point are rounded, if they
			// aren't already.
			if
			(
				(in_pointToCompare.y == immutableDimA)
				&&
				(in_pointToCompare.z == immutableDimB)
			)
			{
				// The X value of the point must be >= 0, AND <= the dimLimit
				if
				(
					(in_pointToCompare.x >= 0.0f)
					&&
					(in_pointToCompare.x <= dimLimit)
				)
				{
					isWithin = true;
				}
			}
			return isWithin;
		}
		void setStaticDims(float in_otherDimImmutableLocA, float in_otherDimImmutableLocB)
		{
			immutableDimA = in_otherDimImmutableLocA;		// A is Y.
			immutableDimB = in_otherDimImmutableLocB;		// B is Z.

		}

		void printMetaData()
		{
			std::cout << "X-Dim | Y-value: " << immutableDimA << " | Z-value: " << immutableDimB << std::endl;
		};

	private:
};

#endif