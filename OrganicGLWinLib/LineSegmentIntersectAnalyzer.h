#pragma once

#ifndef LINESEGMENTINTERSECTANALYZER_H
#define LINESEGMENTINTERSECTANALYZER_H

#include "LineSegmentIntersectResult.h"
#include "LineSegment.h"
#include "TwoDPoint.h"
#include <iostream>

class LineSegmentIntersectAnalyzer
{
	public:
		LineSegmentIntersectAnalyzer(LineSegment in_lineSegmentA, LineSegment in_lineSegmentB) :
			lineSegmentA(in_lineSegmentA),
			lineSegmentB(in_lineSegmentB)
		{
			performAnalysis();
		};
		LineSegment lineSegmentA;
		LineSegment lineSegmentB;
		TwoDPoint r, s;
		LineSegmentIntersectResult analyzedResult;
	private:
		void performAnalysis();
		bool checkForColinearOverlap();
		float calculate2DCross(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB);
		float calculate2DDot(TwoDPoint in_dotPointA, TwoDPoint in_dotPointB);
		float calculateLineSegmentIntersectScalar(float in_numerator, float in_denominator);
};

#endif