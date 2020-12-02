#pragma once

#ifndef LINESEGMENTINTERSECTANALYZER_H
#define LINESEGMENTINTERSECTANALYZER_H

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
	private:
		void performAnalysis();
		float calculate2DCross(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB);
};

#endif