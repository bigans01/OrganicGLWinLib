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
	}
}

void CoplanarAreaRasterizer::runLinePlotting(BrasenhamLineData in_brasenhamLine, RasterizedMassType in_massType)
{
	if (abs(in_brasenhamLine.y1 - in_brasenhamLine.y0) < abs(in_brasenhamLine.x1 - in_brasenhamLine.x0))
	{
		if (in_brasenhamLine.x0 > in_brasenhamLine.x1)
		{
			// low, but swapped
			in_brasenhamLine.swapLine();
			plotLineLow(in_brasenhamLine, in_massType);
		}
		else
		{
			// low
			plotLineLow(in_brasenhamLine, in_massType);
		}
	}
	else
	{
		if (in_brasenhamLine.y0 > in_brasenhamLine.y1)
		{
			// high, but swapped
			in_brasenhamLine.swapLine();
			plotLineHigh(in_brasenhamLine, in_massType);
		}
		else
		{
			// high
			plotLineHigh(in_brasenhamLine, in_massType);
		}
	}
}

void CoplanarAreaRasterizer::plotLineLow(BrasenhamLineData in_brasenhamLine, RasterizedMassType in_massType)
{
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
	for (int x = in_brasenhamLine.x0; x < in_brasenhamLine.x1 + 1; x++)		
	{
		// update tile
		TileLocation currentLocation(x, y);
		updateTile(currentLocation, in_massType);
		if (D > 0)
		{
			y = y + yi;
			D = D + (2 * (dy - dx));
		}
		else
		{
			D = D + 2 * dy;
		}
	}
}

void CoplanarAreaRasterizer::plotLineHigh(BrasenhamLineData in_brasenhamLine, RasterizedMassType in_massType)
{
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
	for (int y = in_brasenhamLine.y0; y < in_brasenhamLine.y1 + 1; y++)
	{
		TileLocation currentLocation(x, y);
		updateTile(currentLocation, in_massType);
		if (D > 0)
		{
			x = x + xi;
			D = D + (2 * (dx - dy));
		}
		else
		{
			D = D + 2 * dx;
		}
	}
}

void CoplanarAreaRasterizer::updateTile(TileLocation in_tileLocation, RasterizedMassType in_massType)
{
	int arrayIndex = determineTileLocationArrayIndex(in_tileLocation);
	if (in_massType == RasterizedMassType::CUTTABLE)
	{
		rasterizationTileGrid[arrayIndex].cuttableMassFlag = true;
		cuttableHorizontalScanLineRegister[in_tileLocation.y].insertLocation(in_tileLocation);

	}
	else if (in_massType == RasterizedMassType::CUTTING)
	{
		rasterizationTileGrid[arrayIndex].cuttingMassFlag = true;
		cuttingHorizontalScanLineRegister[in_tileLocation.y].insertLocation(in_tileLocation);
	}
}

int CoplanarAreaRasterizer::determineTileLocationArrayIndex(TileLocation in_tileLocation)
{
	int returnX = in_tileLocation.x * numberOfTilesPerDimension;
	int returnY = in_tileLocation.y;
	return returnX + returnY;
}

CoplanarAreaRasterizer::TileLocation CoplanarAreaRasterizer::getTileLocationFromIndexValue(int in_indexValue)
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


CoplanarAreaRasterizer::TileLocation CoplanarAreaRasterizer::determinePointTileLocation(glm::vec3 in_point)
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
		x_value = whole_x_tiles;
	}

	int y_value = 0;
	float whole_y_tiles = in_point.y / tileGridWidth;
	if (whole_y_tiles == numberOfTilesPerDimension)
	{
		y_value = numberOfTilesPerDimension - 1;
	}
	else
	{
		y_value = whole_y_tiles;
	}

	//std::cout << "X value is: " << x_value << std::endl;
	//std::cout << "Y value is: " << y_value << std::endl;

	return TileLocation(x_value, y_value);
}