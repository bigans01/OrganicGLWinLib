#pragma once

#ifndef COPLANARAREARASTERIZER_H
#define COPLANARAREARASTERIZER_H

#include <mutex>
#include <set>
#include <glm/glm.hpp>
#include <iostream>
#include <iomanip>
#include <algorithm>

class CoplanarAreaRasterizer
{
	public:
		void buildGrid(int in_numberOfTilesPerDimension, float in_dimensionLimit);
		void insertCuttableTriangleMass(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
		void runScanWithCurrentCuttableTriangle();
		void printCuttableLineScanAtIndex(int in_index);

		void insertCuttingTriangleMass(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
		void runScanWithCurrentCuttingTriangle();
		void printCuttingLineScanAtIndex(int in_index);

		float getRemainingCuttableAreaPercentage();
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
		int getCountOfTilesWithCuttableArea();
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
			void insertLocation(TileLocation in_tileLocation, bool in_debugFlag)
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
						if
						(
							// a second point will only be inserted if it doesn't equal point A.
							!((locationA.x == locationB.x)
								&&
							(locationA.y == locationB.y))
						)
						{
							locationB = in_tileLocation;
							wasSecondLocationInserted = true;
							orderLocationsMinToMax();
						}
					}
					else if (wasSecondLocationInserted == true)
					{
						if (in_debugFlag == true)
						{
							std::cout << "!!! Entered condition: wasSecondLocationInserted == true " << std::endl;
							std::cout << "---> Tile location is: (" << in_tileLocation.x << ", " << in_tileLocation.y << ") " << std::endl;
						}

						orderLocationsMinToMax();


						int intCurrentDistBetweenLocations = abs(locationA.x - locationB.x);	
						int currentDistance = abs(locationA.x - in_tileLocation.x);

						int currentMin2 = std::min(locationA.x, locationB.x);
						int currentMax2 = std::max(locationA.x, locationB.x);
						if (in_tileLocation.x < currentMin2)
						{
							locationA = in_tileLocation;
						}
						else if (in_tileLocation.x > currentMax2)
						{
							locationB = in_tileLocation;
						}

						if (in_debugFlag == true)
						{
							std::cout << "Current locationA: " << locationA.x << ", " << locationA.y << std::endl;
							std::cout << "Current locationB: " << locationB.x << ", " << locationB.y << std::endl;

							std::cout << "Distance between locations: " << intCurrentDistBetweenLocations << std::endl;
							std::cout << "DIstance between A and current tile: " << currentDistance << std::endl;
						}
					}

				}
			}

			void orderLocationsMinToMax()
			{
				// swap, so that A is always the least
				int currentMin = std::min(locationA.x, locationB.x);
				if (currentMin == locationB.x)
				{
					int tempLocation = locationA.x;
					locationA.x = locationB.x;
					locationB.x = tempLocation;
				}
			}

			void reset()
			{
				wasFirstLocationInserted = false;
				wasSecondLocationInserted = false;
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
		void updateTile(TileLocation in_tileLocation, RasterizedMassType in_massType, bool in_debugFlag);
		int determineTileLocationArrayIndex(TileLocation in_tileLocation);
		TileLocation getTileLocationFromIndexValue(int in_indexValue);

		int cuttableCounter = 0;

};

#endif
