#pragma once

#ifndef COPLANARAREARASTERIZER_H
#define COPLANARAREARASTERIZER_H

#include <mutex>
#include <set>
#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>

class CoplanarAreaRasterizer
{
	public:
		void buildGrid(int in_numberOfTilesPerDimension, float in_dimensionLimit);
		void insertCuttableTriangleMass(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
		void runScanWithCurrentCuttableTriangle();
		struct TileLocation
		{
			TileLocation() {};
			TileLocation(short in_x, short in_y) :
				x(in_x),
				y(in_y)
			{};

			short x = 0;
			short y = 0;
		};
		TileLocation determinePointTileLocation(glm::vec3 in_point);
		void printRemainingCuttableTiles();
		void printCuttableXRegister();
	private:
		struct RasterizationTile
		{
			bool cuttableMassFlag = false;
			bool cuttingMassFlag = false;
		};

		struct BrasenhamLineData
		{
			BrasenhamLineData() {};
			BrasenhamLineData(TileLocation in_point0Tile, TileLocation in_point1Tile) :
				x0(in_point0Tile.x),
				y0(in_point0Tile.y),
				x1(in_point1Tile.x),
				y1(in_point1Tile.y)
			{};

			void swapLine()
			{
				short tempX0 = x0;
				short tempY0 = y0;

				x0 = x1;
				y0 = y1;

				x1 = tempX0;
				y1 = tempY0;
			};

			short x0 = 0;
			short y0 = 0;

			short x1 = 0;
			short y1 = 0;
		};

		struct RasterizedScanLineEndpoints
		{
			TileLocation locationA;
			TileLocation locationB;
			bool wasFirstLocationInserted = false;
			bool wasSecondLocationInserted = false;
			void insertLocation(TileLocation in_tileLocation)
			{
				if (wasFirstLocationInserted == false)
				{
					wasFirstLocationInserted = true;
					locationA = in_tileLocation;
				}
				else if (wasFirstLocationInserted == true)
				{
					if (wasSecondLocationInserted == false)
					{
						locationB = in_tileLocation;
						wasSecondLocationInserted = true;
					}
					else if (wasSecondLocationInserted == true)
					{
						int intCurrentDistBetweenLocations = abs(locationA.x - locationB.x);	
						int currentDistance = abs(locationA.x - in_tileLocation.x);
						if (currentDistance > intCurrentDistBetweenLocations)
						{
							locationB = in_tileLocation;
						}
					}
				}
			}
		};

		enum RasterizedMassType
		{
			CUTTABLE,
			CUTTING
		};


		std::unique_ptr<RasterizationTile[]> rasterizationTileGrid;
		std::unique_ptr<RasterizedScanLineEndpoints[]> cuttableHorizontalScanLineRegister;
		std::unique_ptr<RasterizedScanLineEndpoints[]> cuttingHorizontalScanLineRegister;
		int numberOfTilesPerDimension = 0;	
		float dimensionLimit = 0.0f;					
		float tileGridWidth;

		void runLinePlotting(BrasenhamLineData in_brasenhamLine, RasterizedMassType in_massType);
		void plotLineLow(BrasenhamLineData in_brasenhamLine, RasterizedMassType in_massType);
		void plotLineHigh(BrasenhamLineData in_brasenhamLine, RasterizedMassType in_massType);
		void updateTile(TileLocation in_tileLocation, RasterizedMassType in_massType);
		int determineTileLocationArrayIndex(TileLocation in_tileLocation);
		TileLocation getTileLocationFromIndexValue(int in_indexValue);

};

#endif
