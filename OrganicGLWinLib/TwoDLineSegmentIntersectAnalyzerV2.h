#pragma once

#ifndef TWODLINESEGMENTANALYZERV2_H
#define TWODLINESEGMENTANALYZERV2_H

#include "TwoDLineSegmentIntersectResult.h"
#include "TwoDLineSegment.h"
#include "TwoDPoint.h"
#include <iostream>
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "QMBoolAreLinesColinear.h"
#include "QMBoolDoLinesIntersect.h"
#include "QuatRotationPoints.h"

class TwoDLineSegmentIntersectAnalyzerV2
{
	public:	
		TwoDLineSegmentIntersectAnalyzerV2(TwoDLineSegment in_TwoDLineSegmentA, TwoDLineSegment in_TwoDLineSegmentB, PolyDebugLevel in_polyDebugLevel) :
			twoDLineSegmentA(in_TwoDLineSegmentA),
			twoDLineSegmentB(in_TwoDLineSegmentB),
			//analyzerOption(in_option),
			twoDLineSegmentIntersectV2LogLevel(in_polyDebugLevel)
		{
			twoDLineSegmentIntersectV2Logger.setDebugLevel(in_polyDebugLevel);
			performAnalysis();
		};
		TwoDLineSegment twoDLineSegmentA;
		TwoDLineSegment twoDLineSegmentB;
		TwoDPoint r, s;
		TwoDLineSegmentIntersectResult analyzedResult;
	private:
		glm::vec3 convert2DPointTo3D(TwoDPoint in_2dPoint);
		void performAnalysis();
		float calculate2DCross(TwoDPoint in_crossPointA, TwoDPoint in_crossPointB);
		float calculateTwoDLineSegmentIntersectScalar(float in_numerator, float in_denominator);
		PolyLogger twoDLineSegmentIntersectV2Logger;
		PolyDebugLevel twoDLineSegmentIntersectV2LogLevel = PolyDebugLevel::NONE;
};

#endif
