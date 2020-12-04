#pragma once

#ifndef TwoDLineSegmentINTERSECTRESULT_H
#define TwoDLineSegmentINTERSECTRESULT_H

#include "TwoDLineSegmentIntersectType.h"
#include "TwoDPoint.h"

class TwoDLineSegmentIntersectResult
{
	public:
		void setResult(TwoDLineSegmentIntersectType in_TwoDLineSegmentIntersectType, TwoDPoint in_intersectedPoint)
		{
			intersectType = in_TwoDLineSegmentIntersectType;
			intersectedPoint = in_intersectedPoint;
		}
		TwoDLineSegmentIntersectType intersectType = TwoDLineSegmentIntersectType::NOVAL;
		TwoDPoint intersectedPoint;
};

#endif
