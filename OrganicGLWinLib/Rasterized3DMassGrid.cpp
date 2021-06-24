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
		rPolysBegin->second.traceTriangleAreaIntoGrid(&dataArray);
	}
}

void Rasterized3DMassGrid::fillMass()
{
	dataArray.executeDownfills();
}

bool Rasterized3DMassGrid::wasMassFoundInAreaScan(MassGridArrayCellScanArea in_scanArea)
{
	return dataArray.wereCellsDiscoveredInArea(in_scanArea);
}