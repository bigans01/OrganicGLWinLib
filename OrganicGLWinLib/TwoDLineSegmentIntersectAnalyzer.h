#pragma once

#ifndef TwoDLineSegmentINTERSECTANALYZER_H
#define TwoDLineSegmentINTERSECTANALYZER_H

#include "TwoDLineSegmentIntersectResult.h"
#include "TwoDLineSegment.h"
#include "TwoDPoint.h"
#include <iostream>
#include "TwoDLineSegmentJudge.h"

class TwoDLineSegmentIntersectAnalyzer
{
	public:
		TwoDLineSegmentIntersectAnalyzer(TwoDLineSegment in_TwoDLineSegmentA, TwoDLineSegment in_TwoDLineSegmentB) :
			TwoDLineSegmentA(in_TwoDLineSegmentA),
			TwoDLineSegmentB(in_TwoDLineSegmentB)
		{
			performAnalysis();
		};
		TwoDLineSegment TwoDLineSegmentA;
		TwoDLineSegment TwoDLineSegmentB;
		TwoDPoint r, s;
		TwoDLineSegmentIntersectResult analyzedResult;
	private:
		void performAnalysis();
		bool checkForColinearOverlap();
		TwoDPoint round2DPointToHundredths(TwoDPoint in_point);
		float calculate2DCross(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB);
		float calculate2DDot(TwoDPoint in_dotPointA, TwoDPoint in_dotPointB);
		float calculateTwoDLineSegmentIntersectScalar(float in_numerator, float in_denominator);
};

#endif