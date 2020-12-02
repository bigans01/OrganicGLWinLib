#pragma once
#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "TwoDPoint.h"

class LineSegment
{
	public:
		LineSegment() {};
		LineSegment(float in_Ax, float in_Ay, float in_Bx, float in_By) 
		{
			a.x = in_Ax;
			a.y = in_Ay;
			b.x = in_Bx;
			b.y = in_By;
		};
		TwoDPoint a;
		TwoDPoint b;
};

#endif
