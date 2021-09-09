#pragma once

#ifndef BRASENHAMLINEDATA_H
#define BRASENHAMLINEDATA_H

#include "TileLocation.h"

class BrasenhamLineData
{
	public:
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

#endif
