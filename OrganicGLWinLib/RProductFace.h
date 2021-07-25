#pragma once

#ifndef RPRODUCTFACE_H
#define RPRODUCTFACE_H

#include "RProductFaceRootPoints.h"
#include "RCollisionPoint.h"

class RProductFace
{
	public:
		RProductFace() {};
		RProductFace(RProductFaceRootPoints in_rootPoints)
		{
			for (int x = 0; x < 4; x++)
			{
				rootPointArray[x] = in_rootPoints.pointArray[x];
			}
		};
	private:
		RCollisionPoint* rootPointArray[4];

};

#endif
