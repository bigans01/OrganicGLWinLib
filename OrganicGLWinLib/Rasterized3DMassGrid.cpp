#include "stdafx.h"
#include "Rasterized3DMassGrid.h"

void Rasterized3DMassGrid::addGridRPoly(SPoly in_sPolyToResolve)
{
	RPoly polyToResolve(in_sPolyToResolve, 
						numberOfTilesPerDimension,
						dimensionLimit, 
						rCubeDimensionalLength, 
						tileDimWeightToHundredthFloatRatio, 
						massGridLogLevel);
	rPolyMap[gridRPolyCount++] = polyToResolve;
}

void Rasterized3DMassGrid::appendMatterFromOtherArray(MassGridArray* in_otherDataArrayRef)
{

	int dataArraySize = dataArray.getArraySize();

	//std::cout << "~~~~ dataArraySize: " << dataArraySize << std::endl;

	int otherDataArraySize = in_otherDataArrayRef->getArraySize();
	if 
	(
		(dataArraySize == otherDataArraySize) // will only append if the arrays are equal in size.
		&&
		(dataArraySize > 0)					  // array must be initialized; initialized arrays won't have this value == 0.
	)
	{
		MassGridArrayCell* thisInstanceArrayRef = dataArray.getArrayRef();
		MassGridArrayCell* otherInstanceArrayRef = in_otherDataArrayRef->getArrayRef();
		int updateCount = 0;
		for (int x = 0; x < dataArraySize; x++)
		{
			if (otherInstanceArrayRef[x].getNumberOfFlagsSet() > 0)
			{
				thisInstanceArrayRef[x] = otherInstanceArrayRef[x];
				updateCount++;
			}
		}
		//std::cout << "Total number of appends: " << updateCount << std::endl;
	}
}

void Rasterized3DMassGrid::appendAntiMatterFromOtherArray(MassGridArray* in_otherDataArrayRef)
{
	int dataArraySize = dataArray.getArraySize();
	//std::cout << "~~~~ dataArraySize: " << dataArraySize << std::endl;

	int otherDataArraySize = in_otherDataArrayRef->getArraySize();
	if
	(
		(dataArraySize == otherDataArraySize) // will only append if the arrays are equal in size.
		&&
		(dataArraySize > 0)					  // array must be initialized; initialized arrays won't have this value == 0.
	)
	{
		MassGridArrayCell* thisInstanceArrayRef = dataArray.getArrayRef();
		MassGridArrayCell* otherInstanceArrayRef = in_otherDataArrayRef->getArrayRef();
		int updateCount = 0;
		for (int x = 0; x < dataArraySize; x++)
		{
			if (otherInstanceArrayRef[x].getNumberOfFlagsSet() > 0)
			{
				thisInstanceArrayRef[x].resetCell();	// anti-matter wipes out any matter
				updateCount++;
			}
		}
		//std::cout << "Total number of appends: " << updateCount << std::endl;
	}
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

void Rasterized3DMassGrid::setGridParameters(int in_tilesPerDimension, float in_dimensionLimit)
{
	dimensionLimit = in_dimensionLimit;
	numberOfTilesPerDimension = in_tilesPerDimension;
	rCubeDimensionalLength = dimensionLimit / numberOfTilesPerDimension;	//  i.e., for Enclave resolution, this would be:
																		// dimensionLimit = 4.0f
																		// numberOfTilesPerDiemnsion = 400
																		// tileGridWith = 0.01f	
	//std::cout << "Rcube dimensional length: " << rCubeDimensionalLength << std::endl;
	massGridLogger.log("(Rasterized3DMassGrid): Rcube dimensional length: ", rCubeDimensionalLength, "\n");

	tileDimWeightToHundredthFloatRatio = 100.0f / (numberOfTilesPerDimension / dimensionLimit);
	//std::cout << "tile weight to hundredth float ratio: " << tileDimWeightToHundredthFloatRatio << std::endl;
	massGridLogger.log("(Rasterized3DMassGrid): tile weight to hundredth float ratio: ", tileDimWeightToHundredthFloatRatio, "\n");

	dataArray.buildArray(in_tilesPerDimension);
}

void Rasterized3DMassGrid::setLoggerDebugLevel(PolyDebugLevel in_logLevel)
{
	massGridLogger.setDebugLevel(in_logLevel);
	massGridLogLevel = in_logLevel;
}

bool Rasterized3DMassGrid::wasMassFoundInAreaScan(MassGridArrayCellScanArea in_scanArea)
{
	return dataArray.wereCellsDiscoveredInArea(in_scanArea);
}

MassGridSearchResult Rasterized3DMassGrid::fetchCell(EnclaveKeyDef::EnclaveKey in_key)
{
	return dataArray.searchForCell(in_key.x, in_key.y, in_key.z);
}

MassGridArray* Rasterized3DMassGrid::fetchDataArrayRef()
{
	return &dataArray;
}