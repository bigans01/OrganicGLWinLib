#pragma once

#ifndef TERRAINTRIANGLE_H
#define TERRAINTRIANGLE_H

#include "TerrainTrianglePoint.h"

class TerrainTriangle
{
	public:
		TerrainTriangle() {};
		TerrainTriangle(TerrainTrianglePoint in_ttPoint0, TerrainTrianglePoint in_ttPoint1, TerrainTrianglePoint in_ttPoint2)
		{
			ttPoints[0] = in_ttPoint0;
			ttPoints[1] = in_ttPoint1;
			ttPoints[2] = in_ttPoint2;
		};

		TerrainTrianglePoint ttPoints[3];
};

#endif
