#pragma once

#ifndef COPLANARAREARASTERIZER_H
#define COPLANARAREARASTERIZER_H

#include <mutex>
#include <set>
#include <glm/glm.hpp>
#include <iostream>

class CoplanarAreaRasterizer
{
	public:
		void buildGrid(int in_numberOfTilesPerDimension, float in_dimensionLimit);
		void insertCuttableTriangleMass(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
		void runScanWithCurrentCuttableTriangle();
		struct TileLocation
		{
			TileLocation() {};
			TileLocation(int in_x, int in_y) :
				x(in_x),
				y(in_y)
			{};

			int x = 0;
			int y = 0;
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
				int tempX0 = x0;
				int tempY0 = y0;

				x0 = x1;
				y0 = y1;

				x1 = tempX0;
				y1 = tempY0;
			};

			int x0 = 0;
			int y0 = 0;

			int x1 = 0;
			int y1 = 0;
		};

		enum RasterizedMassType
		{
			CUTTABLE,
			CUTTING
		};


		std::unique_ptr<RasterizationTile[]> rasterizationTileGrid;
		std::unique_ptr<std::set<int>[]> cuttableHorizontalScanLineRegister;
		std::unique_ptr<std::set<int>[]> cuttingHorizontalScanLineRegister;
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
