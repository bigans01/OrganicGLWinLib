#pragma once
#ifndef TwoDLineSegment_H
#define TwoDLineSegment_H

#include "TwoDPoint.h"

class TwoDLineSegment
{
	public:
		TwoDLineSegment() {};
		TwoDLineSegment(float in_Ax, float in_Ay, float in_Bx, float in_By) 
		{
			a.x = in_Ax;
			a.y = in_Ay;
			b.x = in_Bx;
			b.y = in_By;
		};
		TwoDLineSegment(TwoDPoint in_twoDPointA, TwoDPoint in_twoDPointB)
		{
			a.x = in_twoDPointA.x;
			a.y = in_twoDPointA.y;
			b.x = in_twoDPointB.x;
			b.y = in_twoDPointB.y;
		}
		TwoDPoint a;
		TwoDPoint b;
};

#endif
