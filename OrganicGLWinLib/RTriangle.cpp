#include "stdafx.h"
#include "RTriangle.h"

RTriangle::RTriangle() 
{

};

RTriangle::RTriangle(RTriangleLine in_line0, RTriangleLine in_line1, RTriangleLine in_line2)
{
	rLines[0] = in_line0;
	rLines[1] = in_line1;
	rLines[2] = in_line2;

	// once lines are set up, initialize the dim registers (xDimRegister, yDimRegister, zDimRegister)
	buildRegisters();
};

void RTriangle::buildRegisters()
{
	EnclaveKeyDef::EnclaveKey pointAKey = rLines[0].pointACubeKey;
	EnclaveKeyDef::EnclaveKey pointBKey = rLines[1].pointACubeKey;
	EnclaveKeyDef::EnclaveKey pointCKey = rLines[2].pointACubeKey;

	// find the greatest difference in x,y,z
	xScanMeta = determineScanMeta(pointAKey.x, pointBKey.x, pointCKey.x);
	yScanMeta = determineScanMeta(pointAKey.y, pointBKey.y, pointCKey.y);
	zScanMeta = determineScanMeta(pointAKey.z, pointBKey.z, pointCKey.z);

	std::cout << ">>>> X scan, greatest dist: " << xScanMeta.numberOfScans << std::endl;
	std::cout << ">>>> X scan, scan coords -- begin: " << xScanMeta.dimStartValue << " | end: " << xScanMeta.dimEndValue << std::endl;

	std::cout << ">>>> Y scan, greatest dist: " << yScanMeta.numberOfScans << std::endl;
	std::cout << ">>>> Y scan, scan coords -- begin: " << yScanMeta.dimStartValue << " | end: " << yScanMeta.dimEndValue << std::endl;

	std::cout << ">>>> Z scan, greatest dist: " << zScanMeta.numberOfScans << std::endl;
	std::cout << ">>>> Z scan, scan coords -- begin: " << zScanMeta.dimStartValue << " | end: " << zScanMeta.dimEndValue << std::endl;

	// set the unique_ptrs for the dim registers;
	xDimRegister.reset(new LookupByDimRegister[xScanMeta.numberOfScans]);
	yDimRegister.reset(new LookupByDimRegister[yScanMeta.numberOfScans]);
	zDimRegister.reset(new LookupByDimRegister[zScanMeta.numberOfScans]);

	// set the unique_ptr arrays-being-set flag
	areRegistersSet = true;

	int buildWait = 3;
	std::cin >> buildWait;
}

RTriangle::DimScanMeta RTriangle::determineScanMeta(int in_pointADimValue, int in_pointBDimValue, int in_pointCDimValue)
{
	DimScanMeta returnMeta;

	// build the candidate for point A
	bool a_match_to_b = false;
	bool a_match_to_c = false;
	int a_begin_scan_coord, a_end_scan_coord;
	int a_a_to_b = abs(in_pointBDimValue - in_pointADimValue);	// should be b - a 
	int a_a_to_c = abs(in_pointCDimValue - in_pointADimValue);	// "" c - a
	int a_greatestDist = std::max(a_a_to_b, a_a_to_c);
	int a_count = a_greatestDist + 1;
	if (a_greatestDist == a_a_to_b)
	{
		a_match_to_b = true;
		a_begin_scan_coord = in_pointADimValue;		// was rLines[0].pointACubeKey.x;
		a_end_scan_coord = in_pointBDimValue;		// was rLines[1].pointACubeKey.x;
	}
	else if (a_greatestDist == a_a_to_c)
	{
		a_match_to_c = true;
		a_begin_scan_coord = in_pointADimValue;	// was rLines[0].pointACubeKey.x;
		a_end_scan_coord = in_pointCDimValue;	// was rLines[2].pointACubeKey.x;
	}
	DimScanMeta a_candidate(a_count, a_begin_scan_coord, a_end_scan_coord);
	a_candidate.swapToMin();


	// build the candidate for point B
	bool b_match_to_a = false;
	bool b_match_to_c = false;
	int b_begin_scan_coord, b_end_scan_coord;
	int b_b_to_a = abs(in_pointADimValue - in_pointBDimValue); // "" a - b
	int b_b_to_c = abs(in_pointCDimValue - in_pointBDimValue); // "" a - c
	int b_greatestDist = std::max(b_b_to_a, b_b_to_c);
	int b_count = b_greatestDist + 1;
	if (b_greatestDist == b_b_to_a)
	{
		b_match_to_a = true;
		b_begin_scan_coord = in_pointBDimValue;		// was rLines[1].pointACubeKey.x
		b_end_scan_coord = in_pointADimValue;		// was rLines[0].pointACubeKey.x
	}
	else if (b_greatestDist == b_b_to_c)
	{
		b_match_to_c = true;
		b_begin_scan_coord = in_pointBDimValue;	// was rLines[1].pointACubeKey.x;
		b_end_scan_coord = in_pointCDimValue;	// was rLines[2].pointACubeKey.x;
	}
	DimScanMeta b_candidate(b_count, b_begin_scan_coord, b_end_scan_coord);
	b_candidate.swapToMin();


	// build the candidate for point C
	bool c_match_to_a = false;
	bool c_match_to_b = false;
	int c_begin_scan_coord, c_end_scan_coord;
	int c_c_to_a = abs(in_pointADimValue - in_pointCDimValue);
	int c_c_to_b = abs(in_pointBDimValue - in_pointCDimValue);
	int c_greatestDist = std::max(c_c_to_a, c_c_to_b);
	int c_count = c_greatestDist + 1;
	if (c_greatestDist == c_c_to_a)
	{
		c_match_to_a = true;
		c_begin_scan_coord = in_pointCDimValue;		// was rLines[2].pointACubeKey.x
		c_end_scan_coord = in_pointADimValue;		// was rLines[0].pointACubeKey.x
	}
	else if (c_greatestDist == c_c_to_b)
	{
		c_match_to_a = true;
		c_begin_scan_coord = in_pointCDimValue;	// was rLines[2].pointACubeKey.x
		c_end_scan_coord = in_pointBDimValue;   // was rLines[1].pointACubeKey.x
	}
	DimScanMeta c_candidate(c_count, c_begin_scan_coord, c_end_scan_coord);
	c_candidate.swapToMin();

	// find whichever candidate had the greatest distance.
	int greatestOfAAndB = std::max(a_candidate.numberOfScans, b_candidate.numberOfScans);
	int greatestOfAll = std::max(greatestOfAAndB, c_candidate.numberOfScans);

	if (greatestOfAll == a_candidate.numberOfScans)
	{
		returnMeta = a_candidate;
	}
	else if (greatestOfAll == b_candidate.numberOfScans)
	{
		returnMeta = b_candidate;
	}
	else if (greatestOfAll == c_candidate.numberOfScans)
	{
		returnMeta = c_candidate;
	}
	return returnMeta;
}

RTriangle::RTriangle(const RTriangle& in_triangleB)
{
	for (int x = 0; x < 3; x++)
	{
		rLines[x] = in_triangleB.rLines[x];
	}

	areRegistersSet = in_triangleB.areRegistersSet;

	if (areRegistersSet == true)
	{
		
		xScanMeta.numberOfScans = in_triangleB.xScanMeta.numberOfScans;
		yScanMeta.numberOfScans = in_triangleB.yScanMeta.numberOfScans;
		zScanMeta.numberOfScans = in_triangleB.zScanMeta.numberOfScans;

		// copy data for the X dim
		xDimRegister.reset(new LookupByDimRegister[xScanMeta.numberOfScans]);
		for (int x = 0; x < xScanMeta.numberOfScans; x++)
		{
			xDimRegister[x] = in_triangleB.xDimRegister[x];
		}

		// "" Y dim 
		yDimRegister.reset(new LookupByDimRegister[yScanMeta.numberOfScans]);
		for (int x = 0; x < yScanMeta.numberOfScans; x++)
		{
			yDimRegister[x] = in_triangleB.yDimRegister[x];
		}

		// "" Z dim
		zDimRegister.reset(new LookupByDimRegister[zScanMeta.numberOfScans]);
		for (int x = 0; x < zScanMeta.numberOfScans; x++)
		{
			//zDimRegister[x] = in_triangleB.zDimRegister[x];
		}
		
	}
}

void RTriangle::printRPoints()
{
	std::cout << "!! Printing points of RTriangle. " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		std::cout << "point " << x << ": " << rLines[x].rLinePointA.x << ", " << rLines[x].rLinePointA.y << ", " << rLines[x].rLinePointA.z << std::endl;
	}
}

void RTriangle::traceRasterLines()
{
	for (int x = 0; x < 3; x++)
	{
		rLines[x].runRasterTrace();
	}
}

void RTriangle::traceRasterLinesIntoGrid(MassGridArray* in_massGridArrayRef, 
	                                     glm::vec3 in_triangleEmptyNormal,
	                                     float in_rPolyRCubeDimLength,
	                                     float in_rPolyTilesPerDim,
	                                     float in_rPolyTileWeightToHundredthFloatRatio)
{
	// step 1: determine the value of the DOWNFILL_CRUST bit
	int downFillCrustBit = 0;
	if (in_triangleEmptyNormal.y > 0.0f)
	{
		downFillCrustBit = 1;
	}


	// step 2: trace the lines of the triangle into the grid.
	for (int x = 0; x < 3; x++)
	{
		rLines[x].runRasterTraceIntoGrid(in_massGridArrayRef, downFillCrustBit);
	}

	// step 3: use the rasterizedBlocks from each line, to create line pairs that are used to trace the interior of the RTriangle
	// into the MassGridArray. Each line pair should do X, Y, and Z scans.
	initializeXYZDimRegisters();
	runXDimRegisterScan(in_massGridArrayRef, in_rPolyRCubeDimLength, in_rPolyTilesPerDim, in_rPolyTileWeightToHundredthFloatRatio);
	runYDimRegisterScan(in_massGridArrayRef, in_rPolyRCubeDimLength, in_rPolyTilesPerDim, in_rPolyTileWeightToHundredthFloatRatio);
	runZDimRegisterScan(in_massGridArrayRef, in_rPolyRCubeDimLength, in_rPolyTilesPerDim, in_rPolyTileWeightToHundredthFloatRatio);
}

void RTriangle::initializeXYZDimRegisters()
{
	int xScanMetaBeginValue = xScanMeta.dimStartValue;
	for (int x = 0; x < xScanMeta.numberOfScans; x++)
	{
		LookupByDimRegister newXRegister(xScanMetaBeginValue++);
		xDimRegister[x] = newXRegister;
	}

	int yScanMetaBeginValue = yScanMeta.dimStartValue;
	for (int y = 0; y < yScanMeta.numberOfScans; y++)
	{
		LookupByDimRegister newYRegister(yScanMetaBeginValue++);
		yDimRegister[y] = newYRegister;
	}

	int zScanMetaBeginValue = zScanMeta.dimStartValue;
	for (int z = 0; z < zScanMeta.numberOfScans; z++)
	{
		LookupByDimRegister newZRegister(zScanMetaBeginValue++);
		zDimRegister[z] = newZRegister;
	}
}

void RTriangle::runXDimRegisterScan(MassGridArray* in_massGridArrayRef, 
									float in_rPolyRCubeDimLength,
									float in_rPolyTilesPerDim,
									float in_rPolyTileWeightToHundredthFloatRatio)
{
	for (int x = 0; x < xScanMeta.numberOfScans; x++)
	{
		// check all lines for entries
		int currentXValue = xDimRegister[x].dimValue;
		for (int currentLine = 0; currentLine < 3; currentLine++)
		{
			auto currentLineTouchedBlocks = rLines[currentLine].findBlocksAtX(currentXValue);
			if (!currentLineTouchedBlocks.empty())
			{
				/*
				std::cout << "!! Found entries at line " << currentLine << ", for " << currentXValue << ": " << std::endl;
				auto line0PrintablesBegin = rLines[currentLine].rasterizedBlocks.xLookup.lookup[currentXValue].begin();
				auto line0PrintablesEnd = rLines[currentLine].rasterizedBlocks.xLookup.lookup[currentXValue].end();
				for (; line0PrintablesBegin != line0PrintablesEnd; line0PrintablesBegin++)
				{
					std::cout << "(" << currentXValue << ", " << line0PrintablesBegin->a << ", " << line0PrintablesBegin->b << ") " << std::endl;
				}
				*/
				xDimRegister[x].insertLineSetRef(currentLine, &rLines[currentLine].rasterizedBlocks.xLookup.lookup[currentXValue]);
			}
		}
		//int scanWait = 3; 
		//std::cin >> scanWait;

		// build the scans in each register, then execute them
		xDimRegister[x].buildScanRuns();
		xDimRegister[x].executeScanRuns(in_massGridArrayRef,
			                            in_rPolyRCubeDimLength,
			                            in_rPolyTilesPerDim,
			                            in_rPolyTileWeightToHundredthFloatRatio);
	}
}

void RTriangle::runYDimRegisterScan(MassGridArray* in_massGridArrayRef,
									float in_rPolyRCubeDimLength,
									float in_rPolyTilesPerDim,
									float in_rPolyTileWeightToHundredthFloatRatio)
{
	for (int y = 0; y < yScanMeta.numberOfScans; y++)
	{
		// check all lines for entries
		int currentYValue = yDimRegister[y].dimValue;
		for (int currentLine = 0; currentLine < 3; currentLine++)
		{
			auto currentLineTouchedBlocks = rLines[currentLine].findBlocksAtY(currentYValue);
			if (!currentLineTouchedBlocks.empty())
			{
				yDimRegister[y].insertLineSetRef(currentLine, &rLines[currentLine].rasterizedBlocks.yLookup.lookup[currentYValue]);
			}
		}

		// build the scans in each register, then execute them
		yDimRegister[y].buildScanRuns();
		yDimRegister[y].executeScanRuns(in_massGridArrayRef,
										in_rPolyRCubeDimLength,
										in_rPolyTilesPerDim,
										in_rPolyTileWeightToHundredthFloatRatio);
	}
}

void RTriangle::runZDimRegisterScan(MassGridArray* in_massGridArrayRef,
									float in_rPolyRCubeDimLength,
									float in_rPolyTilesPerDim,
									float in_rPolyTileWeightToHundredthFloatRatio)
{
	for (int z = 0; z < zScanMeta.numberOfScans; z++)
	{
		// check all lines for entries
		int currentZValue = zDimRegister[z].dimValue;
		for (int currentLine = 0; currentLine < 3; currentLine++)
		{
			auto currentLineTouchedBlocks = rLines[currentLine].findBlocksAtZ(currentZValue);
			if (!currentLineTouchedBlocks.empty())
			{
				zDimRegister[z].insertLineSetRef(currentLine, &rLines[currentLine].rasterizedBlocks.zLookup.lookup[currentZValue]);
			}
		}

		// build the scans in each register then execute them
		zDimRegister[z].buildScanRuns();
		zDimRegister[z].executeScanRuns(in_massGridArrayRef,
										in_rPolyRCubeDimLength,
										in_rPolyTilesPerDim,
										in_rPolyTileWeightToHundredthFloatRatio);
	}
}