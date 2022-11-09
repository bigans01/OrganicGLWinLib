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

		TwoDPoint operator*(const float& in_multiplierValue)
		{
			TwoDPoint returnPoint;
			returnPoint.x = x;
			returnPoint.y = y;
			returnPoint.x *= in_multiplierValue;
			returnPoint.y *= in_multiplierValue;
			return returnPoint;
		}

		TwoDPoint operator-(const TwoDPoint& in_twoDPointB)
		{
			TwoDPoint returnPoint;
			returnPoint.x = x;
			returnPoint.y = y;
			returnPoint.x -= in_twoDPointB.x;
			returnPoint.y -= in_twoDPointB.y;

			//std::cout << "~~~~~ Return value is: " << returnPoint.x << ", " << returnPoint.y << std::endl;
			return returnPoint;
		}

		TwoDPoint operator+(const TwoDPoint& in_twoDPointB)
		{
			TwoDPoint returnPoint;
			returnPoint.x = x;
			returnPoint.y = y;
			returnPoint.x += in_twoDPointB.x;
			returnPoint.y += in_twoDPointB.y;
			return returnPoint;
		}

		float x = 0;
		float y = 0;
};

#endif
