#pragma once

#ifndef TwoDLineSegmentINTERSECTANALYZER_H
#define TwoDLineSegmentINTERSECTANALYZER_H

#include "TwoDLineSegmentIntersectResult.h"
#include "TwoDLineSegment.h"
#include "TwoDPoint.h"
#include <iostream>
#include "TwoDLineSegmentJudge.h"
#include "IntersectAnalyzerOption.h"

class TwoDLineSegmentIntersectAnalyzer
{
	public:
		TwoDLineSegmentIntersectAnalyzer(TwoDLineSegment in_TwoDLineSegmentA, TwoDLineSegment in_TwoDLineSegmentB, IntersectAnalyzerOption in_option) :
			TwoDLineSegmentA(in_TwoDLineSegmentA),
			TwoDLineSegmentB(in_TwoDLineSegmentB),
			analyzerOption(in_option)
		{
			performAnalysis();
		};
		TwoDLineSegment TwoDLineSegmentA;
		TwoDLineSegment TwoDLineSegmentB;
		TwoDPoint r, s;
		TwoDLineSegmentIntersectResult analyzedResult;
	private:
		IntersectAnalyzerOption analyzerOption = IntersectAnalyzerOption::NONE;
		void performAnalysis();
		bool checkForColinearOverlap();
		TwoDPoint round2DPointToHundredths(TwoDPoint in_point);
		float calculate2DCross(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB);
		float calculate2DCrossWithOptionCheck(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB);
		float calculate2DDot(TwoDPoint in_dotPointA, TwoDPoint in_dotPointB);
		float calculate2DDotWithOptionCheck(TwoDPoint in_dotPointA, TwoDPoint in_dotPointB);
		float calculateTwoDLineSegmentIntersectScalar(float in_numerator, float in_denominator);
		bool isFloatWithinColinearThreshold(float in_threshold);
};

#endif