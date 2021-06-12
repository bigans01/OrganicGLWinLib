#include "stdafx.h"
#include "Rasterized3DMassGrid.h"

void Rasterized3DMassGrid::addGridRPoly(SPoly in_sPolyToResolve)
{
	RPoly polyToResolve(in_sPolyToResolve, numberOfTilesPerDimension, dimensionLimit, rCubeDimensionalLength, tileDimWeightToHundredthFloatRatio);
	rPolyMap[gridRPolyCount++] = polyToResolve;
}

void Rasterized3DMassGrid::buildShell()
{
	// cycle through each RTriangle in each RPoly; trace the interior for each, 
	// and put the shell data into the appropriate value in the MassGridArray instance (dataArray)
	auto rPolysBegin = rPolyMap.begin();
	auto rPolysEnd = rPolyMap.end();
	for (; rPolysBegin != rPolysEnd; rPolysBegin++)
	{
		auto rTrianglesBegin = rPolysBegin->second.rTriangleMap.begin();
		auto rTrianglesEnd = rPolysBegin->second.rTriangleMap.end();
		for (; rTrianglesBegin != rTrianglesEnd; rTrianglesBegin++)
		{
			rTrianglesBegin->second.traceRasterLinesIntoGrid(&dataArray);
		}
	}
}

void Rasterized3DMassGrid::fillMass()
{

}