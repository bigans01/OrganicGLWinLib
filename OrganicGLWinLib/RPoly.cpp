#include "stdafx.h"
#include "RPoly.h"

void RPoly::addRTriangle(RTriangle in_rTriangleToAdd)
{
	rTriangleMap[numberOfRTriangles++] = in_rTriangleToAdd;
}

void RPoly::traceTriangleAreaIntoGrid(MassGridArray* in_massGridArrayRef, bool in_debugFlag)
{
	auto rTrianglesBegin = rTriangleMap.begin();
	auto rTrianglesEnd = rTriangleMap.end();
	for (; rTrianglesBegin != rTrianglesEnd; rTrianglesBegin++)
	{
		rTrianglesBegin->second.traceRasterLinesIntoGrid(in_massGridArrayRef, 
														 rPolyEmptyNormal,
			                                             rPolyRCubeDimLength,
			                                             rPolyTilesPerDim,
			                                             rPolyTileWeightToHundredthFloatRatio,
														 in_debugFlag);
	}
}

void RPoly::printRPoints()
{
	for (int x = 0; x < numberOfRBorderLines; x++)
	{
		std::cout << "RPoint " << x << ": " << rBorderLines[x].pointA.x << ", " << rBorderLines[x].pointA.y << ", " << rBorderLines[x].pointA.z << std::endl;
	}
}