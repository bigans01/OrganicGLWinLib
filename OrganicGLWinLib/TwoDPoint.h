#pragma once

#ifndef TWODPOINT_H
#define TWODPOINT_H

class TwoDPoint
{
	public:
		TwoDPoint() {};
		TwoDPoint(float in_x, float in_y) :
			x(in_x),
			y(in_y)
		{};

		TwoDPoint& operator-=(const TwoDPoint& in_twoDPointB)
		{
			x -= in_twoDPointB.x;
			y -= in_twoDPointB.y;
			return *this;
		}

		TwoDPoint& operator+=(const TwoDPoint& in_twoDPointB)
		{
			x += in_twoDPointB.x;
			y += in_twoDPointB.y;
			return *this;
		}

		float x = 0;
		float y = 0;
};

#endif
