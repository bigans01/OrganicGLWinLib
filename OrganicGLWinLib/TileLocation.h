#pragma once

#ifndef TILELOCATION_H
#define TILELOCATION_H

class TileLocation
{
	public:
		TileLocation() {};
		TileLocation(short in_x, short in_y) :
			x(in_x),
			y(in_y)
		{};

		short x = 0;
		short y = 0;

		void reset()
		{
			x = 0;
			y = 0;
		};
};

#endif
