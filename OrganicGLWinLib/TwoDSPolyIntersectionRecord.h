#pragma once
#ifndef TWODSPOLYINTERSECTIONRECORD_H
#define TWODSPOLYINTERSECTIONRECORD_H

#include "TwoDSPolyIntersectionType.h"
#include "TwoDPoint.h"

class TwoDSPolyIntersectionRecord
{
	public:
		TwoDSPolyIntersectionRecord() {};
		TwoDSPolyIntersectionRecord(TwoDSPolyIntersectionType in_intersectionType, bool in_isOnBorderLine, unsigned char in_intersectdBorderLineID, TwoDPoint in_intersectedPoint) :
			intersectionType(in_intersectionType),
			intersectedBorderLine(in_isOnBorderLine),
			intersectedBorderLineID(in_intersectdBorderLineID),
			intersectedPoint(in_intersectedPoint)
		{}
		TwoDSPolyIntersectionType intersectionType = TwoDSPolyIntersectionType::NOVAL;	// default value;
		bool intersectedBorderLine = false;
		unsigned char intersectedBorderLineID = 0;		// used if intersectedBorderLine is true
		TwoDPoint intersectedPoint;
};

#endif
