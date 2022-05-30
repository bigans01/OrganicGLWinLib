#include "stdafx.h"
#include "CoplanarAreaRasterizer.h"

void CoplanarAreaRasterizer::buildGrid(int in_numberOfTilesPerDimension, float in_dimensionLimit)
{
	numberOfTilesPerDimension = in_numberOfTilesPerDimension;
	dimensionLimit = in_dimensionLimit;
	rasterizationTileGrid.reset(new RasterizationTile[numberOfTilesPerDimension * numberOfTilesPerDimension]);
	cuttableHorizontalScanLineRegister.reset(new RasterizedScanLineEndpoints[numberOfTilesPerDimension]);
	cuttingHorizontalScanLineRegister.reset(new RasterizedScanLineEndpoints[numberOfTilesPerDimension]);

	tileGridWidth = dimensionLimit / numberOfTilesPerDimension;
}

void CoplanarAreaRasterizer::insertCuttableTriangleMass(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	TileLocation point0Location = determinePointTileLocation(in_point0);
	TileLocation point1Location = determinePointTileLocation(in_point1);
	TileLocation point2Location = determinePointTileLocation(in_point2);

	//std::cout << "(CoplanarAreaRasterizer): cuttable triangle mass, point0Location: (" << point0Location.x << ", " << point0Location.y << ") " << std::endl;
	//std::cout << "(CoplanarAreaRasterizer): cuttable triangle mass, point1Location: (" << point1Location.x << ", " << point1Location.y << ") " << std::endl;
	//std::cout << "(CoplanarAreaRasterizer): cuttable triangle mass, point2Location: (" << point2Location.x << ", " << point2Location.y << ") " << std::endl;

	BrasenhamLineData brasenhamLines[3];
	brasenhamLines[0] = BrasenhamLineData(point0Location, point1Location);
	brasenhamLines[1] = BrasenhamLineData(point1Location, point2Location);
	brasenhamLines[2] = BrasenhamLineData(point2Location, point0Location);

	// see Wikipedia article on Brasenham's algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	// analyze each line to determine which function to call
	for (int x = 0; x < 3; x++)
	{
		BrasenhamLineData currentLine = brasenhamLines[x];
		runLinePlotting(currentLine, RasterizedMassType::CUTTABLE);
	}

	//std::cout << "HARDCODED TEST: printing out line data at x = 2: " << std::endl;
	//printCuttableLineScanAtIndex(2);

	//printRemainingCuttableTiles();
}

void CoplanarAreaRasterizer::printCuttableLineScanAtIndex(int in_index)
{
	std::cout << " Current cuttable scan line, at elevation: " << in_index << std::endl;
	std::cout << "A: (" << cuttableHorizontalScanLineRegister[in_index].locationA.x << ", " << cuttableHorizontalScanLineRegister[in_index].locationA.y << ") " << std::endl;
	std::cout << "B: (" << cuttableHorizontalScanLineRegister[in_index].locationB.x << ", " << cuttableHorizontalScanLineRegister[in_index].locationB.y << ") " << std::endl;

}

void CoplanarAreaRasterizer::printCuttingLineScanAtIndex(int in_index)
{
	std::cout << " Current cutting scan line, at elevation: " << in_index << std::endl;
	std::cout << "A: (" << cuttingHorizontalScanLineRegister[in_index].locationA.x << ", " << cuttingHorizontalScanLineRegister[in_index].locationA.y << ") " << std::endl;
	std::cout << "B: (" << cuttingHorizontalScanLineRegister[in_index].locationB.x << ", " << cuttingHorizontalScanLineRegister[in_index].locationB.y << ") " << std::endl;
}

void CoplanarAreaRasterizer::runScanWithCurrentCuttableTriangle()
{
	//std::cout << "!!! Running scan with cuttable X register lines..." << std::endl;
	for (int y = 0; y < (numberOfTilesPerDimension); y++)
	{
		if (cuttableHorizontalScanLineRegister[y].wasSecondLocationInserted == true)
		{
			// only do it if the x values aren't the same.
			if (cuttableHorizontalScanLineRegister[y].locationA.x != cuttableHorizontalScanLineRegister[y].locationB.x)
			{
				int minVal = std::min(cuttableHorizontalScanLineRegister[y].locationA.x, cuttableHorizontalScanLineRegister[y].locationB.x);
				int maxVal = std::max(cuttableHorizontalScanLineRegister[y].locationA.x, cuttableHorizontalScanLineRegister[y].locationB.x);
				for (int c = minVal; c < maxVal + 1; c++)
				{
					TileLocation currentTile(c, y);
					int currentIndex = determineTileLocationArrayIndex(currentTile);
					rasterizationTileGrid[currentIndex].cuttableMassFlag = true;
				}

			}
		}
		cuttableHorizontalScanLineRegister[y].reset();
	}
}

void CoplanarAreaRasterizer::insertCuttingTriangleMass(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	TileLocation point0Location = determinePointTileLocation(in_point0);
	TileLocation point1Location = determinePointTileLocation(in_point1);
	TileLocation point2Location = determinePointTileLocation(in_point2);

	BrasenhamLineData brasenhamLines[3];
	brasenhamLines[0] = BrasenhamLineData(point0Location, point1Location);
	brasenhamLines[1] = BrasenhamLineData(point1Location, point2Location);
	brasenhamLines[2] = BrasenhamLineData(point2Location, point0Location);

	// see Wikipedia article on Brasenham's algorithm: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	// analyze each line to determine which function to call
	for (int x = 0; x < 3; x++)
	{
		//std::cout << "...............Running plot for line: " << x << std::endl;
		BrasenhamLineData currentLine = brasenhamLines[x];
		//std::cout << "...............Ended Running plot for line: " << x << std::endl;
		runLinePlotting(currentLine, RasterizedMassType::CUTTING);
	}
}

void CoplanarAreaRasterizer::runScanWithCurrentCuttingTriangle()
{
	for (int y = 0; y < (numberOfTilesPerDimension); y++)
	{
		if (cuttingHorizontalScanLineRegister[y].wasSecondLocationInserted == true)
		{
			// only do it if the x values aren't the same.
			if (cuttingHorizontalScanLineRegister[y].locationA.x != cuttingHorizontalScanLineRegister[y].locationB.x)
			{
				int minVal = std::min(cuttingHorizontalScanLineRegister[y].locationA.x, cuttingHorizontalScanLineRegister[y].locationB.x);
				int maxVal = std::max(cuttingHorizontalScanLineRegister[y].locationA.x, cuttingHorizontalScanLineRegister[y].locationB.x);
				for (int c = minVal; c < maxVal + 1; c++)
				{
					TileLocation currentTile(c, y);
					int currentIndex = determineTileLocationArrayIndex(currentTile);
					rasterizationTileGrid[currentIndex].cuttingMassFlag = true;
				}

			}
		}
		cuttingHorizontalScanLineRegister[y].reset();
	}
}

float CoplanarAreaRasterizer::getRemainingCuttableAreaPercentage()
{
	bool wasConsumed = false;

	// first pass, set cuttableMassFlag to false if cuttingMassFlag was true
	for (int x = 0; x < numberOfTilesPerDimension*numberOfTilesPerDimension; x++)
	{
		if (rasterizationTileGrid[x].cuttingMassFlag == true)
		{
			rasterizationTileGrid[x].cuttableMassFlag = false;
		}
	}

	// second pass, check for any cuttableMassFlag that is set to true; break upon first encounter. 
	bool foundRemainingMass = false;
	int remainingMassCount = 0;
	for (int x = 0; x < numberOfTilesPerDimension*numberOfTilesPerDimension; x++)
	{
		if (rasterizationTileGrid[x].cuttableMassFlag == true)
		{
			//std::cout << "!! Triangle retains some mass, shouldn't be completely destroyed..." << std::endl;
			foundRemainingMass = true;
			//break;
			remainingMassCount++;
		}
	}

	std::cout << "!!! Original number of cuttable tiles: " << cuttableCounter << std::endl;
	std::cout << "!!! Total tiles with remaining mass: " << remainingMassCount << std::endl;
	float remainingMassPercentage = float(float(remainingMassCount) / float(cuttableCounter));
	std::cout << "!!! <> Remaining mass percentage is: " << remainingMassPercentage << std::endl;

	if (foundRemainingMass == false)
	{
		std::cout << "No mass found; triangle was entirely consumed. " << std::endl;
		wasConsumed = true;
	}

	return remainingMassPercentage;
}

void CoplanarAreaRasterizer::runLinePlotting(BrasenhamLineData in_brasenhamLine, RasterizedMassType in_massType)
{
	//std::cout << "!!! Running line: (" << in_brasenhamLine.x0 << ", " << in_brasenhamLine.y0 << ") -> (" << in_brasenhamLine.x1 << ", " << in_brasenhamLine.y1 << ")" << std::endl;

	if (abs(in_brasenhamLine.y1 - in_brasenhamLine.y0) < abs(in_brasenhamLine.x1 - in_brasenhamLine.x0))
	{
		if (in_brasenhamLine.x0 > in_brasenhamLine.x1)
		{
			// low, but swapped
			//std::cout << ">>> running low, swapped. " << std::endl;
			in_brasenhamLine.swapLine();
			plotLineLow(in_brasenhamLine, in_massType);
		}
		else
		{
			// low
			//std::cout << ">>> running low, no swap. " << std::endl;
			plotLineLow(in_brasenhamLine, in_massType);
		}
	}
	else
	{
		if (in_brasenhamLine.y0 > in_brasenhamLine.y1)
		{
			// high, but swapped
			//std::cout << ">>> running high, swapped. " << std::endl;
			in_brasenhamLine.swapLine();
			plotLineHigh(in_brasenhamLine, in_massType);
		}
		else
		{
			//std::cout << ">>> running high, no swap. " << std::endl;
			//std::cout << ":::: calling high, no swap. " << std::endl;
			// high
			plotLineHigh(in_brasenhamLine, in_massType);
		}
	}
}

void CoplanarAreaRasterizer::plotLineLow(BrasenhamLineData in_brasenhamLine, RasterizedMassType in_massType)
{
	bool specialDebugFlag = false;

	int dx = in_brasenhamLine.x1 - in_brasenhamLine.x0;
	int dy = in_brasenhamLine.y1 - in_brasenhamLine.y0;
	int yi = 1;		// represents the y direction, can be 1 or -1
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	int D = (2 * dy) - dx;
	int y = in_brasenhamLine.y0;
	int previous_x = in_brasenhamLine.x0;
	for (int x = in_brasenhamLine.x0; x < in_brasenhamLine.x1 + 1; x++)		
	{
		// update tile
		TileLocation currentLocation(x, y);
		updateTile(currentLocation, in_massType, specialDebugFlag);
		//std::cout << "plotLineLow, tile location to insert is: " << currentLocation.x << ", " << currentLocation.y << std::endl;

		if (D > 0)
		{
			// be sure to fill previous tile location
			int previous_y = y;
			y = y + yi;
			D = D + (2 * (dy - dx));

			// Line fattening -- can only do these operations if we aren't on the last iteration
			if (x < in_brasenhamLine.x1)
			{

				TileLocation previousTileLocation(previous_x, y);
				updateTile(previousTileLocation, in_massType, specialDebugFlag);
				//std::cout << "plotLineLow, option #2 insert: " << previousTileLocation.x << ", " << previousTileLocation.y << std::endl;

				if (x < (numberOfTilesPerDimension - 1))
				{
					TileLocation previousTileLocation2(x + 1, previous_y);
					updateTile(previousTileLocation2, in_massType, specialDebugFlag);
					//std::cout << "plotLineLow, option #3 insert: " << previousTileLocation2.x << ", " << previousTileLocation2.y << std::endl;
				}
			}
		}
		else
		{
			D = D + 2 * dy;
		}
		previous_x = x;
	}
}

void CoplanarAreaRasterizer::plotLineHigh(BrasenhamLineData in_brasenhamLine, RasterizedMassType in_massType)
{
	bool specialDebug = false;
	bool specialFlagSecondary = false;
	if
	(
		(in_brasenhamLine.x0 == 0)
		&&
		(in_brasenhamLine.y0 == 43)
	)
	{
		//specialDebug = true;
	}

	int dx = in_brasenhamLine.x1 - in_brasenhamLine.x0;
	int dy = in_brasenhamLine.y1 - in_brasenhamLine.y0;
	int xi = 1;
	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	int D = (2 * dx) - dy;
	int x = in_brasenhamLine.x0;
	int previous_y = in_brasenhamLine.y0;
	for (int y = in_brasenhamLine.y0; y < in_brasenhamLine.y1 + 1; y++)
	{
		TileLocation currentLocation(x, y);
		updateTile(currentLocation, in_massType, specialDebug);
		//std::cout << "plotLineHigh, tile location to insert is: " << currentLocation.x << ", " << currentLocation.y << std::endl;

		if (D > 0)
		{
			// be sure to fill previous tile location
			//std::cout << "Shift detected..." << std::endl;

			int previous_x = x;
			x = x + xi;
			D = D + (2 * (dx - dy));

			//std::cout << "new x: " << x << std::endl;
			//std::cout << "previous y: " << previous_y << std::endl;

			
			// Line fattening -- can only do these operations if we aren't on the last iteration
			if (y < in_brasenhamLine.y1)
			{
				TileLocation previousTileLocation(x, previous_y);
				updateTile(previousTileLocation, in_massType, specialFlagSecondary);
				//std::cout << "plotLineHigh, option #2 insert: " << previousTileLocation.x << ", " << previousTileLocation.y << " | current y value: " << y << " | y-max: " << in_brasenhamLine.y1 + 1 << std::endl;

				if (y < (numberOfTilesPerDimension - 1))
				{
					TileLocation previousTileLocation2(previous_x, y + 1);
					updateTile(previousTileLocation2, in_massType, specialFlagSecondary);
					//std::cout << "plotLineHigh, option #3 insert: " << previousTileLocation2.x << ", " << previousTileLocation2.y << std::endl;
				}
			}
		}
		else
		{
			D = D + 2 * dx;
		}
		previous_y = y;
	}
}

void CoplanarAreaRasterizer::updateTile(TileLocation in_tileLocation, RasterizedMassType in_massType, bool in_debugFlag)
{
	int arrayIndex = determineTileLocationArrayIndex(in_tileLocation);
	if (in_massType == RasterizedMassType::CUTTABLE)
	{
		rasterizationTileGrid[arrayIndex].cuttableMassFlag = true;
		cuttableHorizontalScanLineRegister[in_tileLocation.y].insertLocation(in_tileLocation, in_debugFlag);

	}
	else if (in_massType == RasterizedMassType::CUTTING)
	{
		rasterizationTileGrid[arrayIndex].cuttingMassFlag = true;
		cuttingHorizontalScanLineRegister[in_tileLocation.y].insertLocation(in_tileLocation, in_debugFlag);
	}
}

int CoplanarAreaRasterizer::determineTileLocationArrayIndex(TileLocation in_tileLocation)
{
	int returnX = in_tileLocation.x * numberOfTilesPerDimension;
	int returnY = in_tileLocation.y;
	return returnX + returnY;
}

TileLocation CoplanarAreaRasterizer::getTileLocationFromIndexValue(int in_indexValue)
{
	int tileX = in_indexValue / numberOfTilesPerDimension;
	int tileY = int(fmod(in_indexValue, numberOfTilesPerDimension));
	return TileLocation(tileX, tileY);
}

void CoplanarAreaRasterizer::printRemainingCuttableTiles()
{
	for (int x = 0; x < (numberOfTilesPerDimension*numberOfTilesPerDimension); x++)
	{
		if (rasterizationTileGrid[x].cuttableMassFlag == true)
		{
			TileLocation currentIndexTileLocation = getTileLocationFromIndexValue(x);
			std::cout << "Tile at " << currentIndexTileLocation.x << ", " << currentIndexTileLocation.y << " remains. " << std::endl;
		}
	}
}

int CoplanarAreaRasterizer::getCountOfTilesWithCuttableArea()
{
	cuttableCounter = 0;
	for (int x = 0; x < (numberOfTilesPerDimension*numberOfTilesPerDimension); x++)
	{
		if (rasterizationTileGrid[x].cuttableMassFlag == true)
		{
			cuttableCounter++;
		}
	}
	//std::cout << "Remaining tiles with cuttable area: " << cuttableCounter << std::endl;
	return cuttableCounter;
}

void CoplanarAreaRasterizer::printCuttableXRegister()
{
	for (int y = 0; y < (numberOfTilesPerDimension); y++)
	{
		if (cuttableHorizontalScanLineRegister[y].locationA.x != cuttableHorizontalScanLineRegister[y].locationB.x)
		{
			std::cout << "Tiles in for Y line " << y << ": " << std::endl;
			int minVal = std::min(cuttableHorizontalScanLineRegister[y].locationA.x, cuttableHorizontalScanLineRegister[y].locationB.x);
			int maxVal = std::max(cuttableHorizontalScanLineRegister[y].locationA.x, cuttableHorizontalScanLineRegister[y].locationB.x);
			std::cout << "min val: (" << minVal << ", " << y << ") " << std::endl;
			std::cout << "max val: (" << maxVal << ", " << y << ") " << std::endl;
		}
	}
}


TileLocation CoplanarAreaRasterizer::determinePointTileLocation(glm::vec3 in_point)
{

	int x_value = 0;
	float whole_x_tiles = in_point.x / tileGridWidth;
	//float x_remainder = std::fmod(in_point.x, tileGridWidth);
	if (whole_x_tiles == numberOfTilesPerDimension)
	{
		x_value = numberOfTilesPerDimension - 1;
	}
	else
	{
		x_value = int(whole_x_tiles);
	}

	int y_value = 0;
	float whole_y_tiles = in_point.y / tileGridWidth;
	if (whole_y_tiles == numberOfTilesPerDimension)
	{
		y_value = numberOfTilesPerDimension - 1;
	}
	else
	{
		y_value = int(whole_y_tiles);
	}

	//std::cout << "X value is: " << x_value << std::endl;
	//std::cout << "Y value is: " << y_value << std::endl;

	return TileLocation(x_value, y_value);
}