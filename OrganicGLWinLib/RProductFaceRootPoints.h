#pragma once

#ifndef RPRODUCTFACEROOTPOINTS_H
#define RPRODUCTFACEROOTPOINTS_H

#include "RCollisionPoint.h"

class RProductFaceRootPoints
{
	public:
		RProductFaceRootPoints() {};
		RProductFaceRootPoints(RCollisionPoint* in_point0Ref,
			RCollisionPoint* in_point1Ref,
			RCollisionPoint* in_point2Ref,
			RCollisionPoint* in_point3Ref)
		{
			pointArray[0] = in_point0Ref;
			pointArray[1] = in_point1Ref;
			pointArray[2] = in_point2Ref;
			pointArray[3] = in_point3Ref;
		};
		RCollisionPoint* pointArray[4];	// one index per corner (for the 4 corners of the RProductFace)

		void printPointValues()
		{
			for (int x = 0; x < 4; x++)
			{
				std::cout << "Point: " << pointArray[x]->currentValue.x << ", " << pointArray[x]->currentValue.y << ", " << pointArray[x]->currentValue.z << std::endl;
			}
		}
};

#endif
