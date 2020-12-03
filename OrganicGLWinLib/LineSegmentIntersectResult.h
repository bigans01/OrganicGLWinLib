#pragma once

#ifndef LINESEGMENTINTERSECTRESULT_H
#define LINESEGMENTINTERSECTRESULT_H

#include "LineSegmentIntersectType.h"
#include "TwoDPoint.h"

class LineSegmentIntersectResult
{
	public:
		void setResult(LineSegmentIntersectType in_lineSegmentIntersectType, TwoDPoint in_intersectedPoint)
		{
			intersectType = in_lineSegmentIntersectType;
			intersectedPoint = in_intersectedPoint;
		}
		LineSegmentIntersectType intersectType = LineSegmentIntersectType::NOVAL;
		TwoDPoint intersectedPoint;
};

#endif
