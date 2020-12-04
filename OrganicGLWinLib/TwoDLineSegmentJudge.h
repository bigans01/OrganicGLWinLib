#pragma once

#ifndef TWODLINESEGMENTJUDGE_H
#define TWODLINESEGMENTJUDGE_H

#include "TwoDLineSegmentIntersectResult.h"
#include "TwoDLineSegment.h"
#include "SPoly.h"


class TwoDLineSegmentJudge
{
	public:
		TwoDLineSegmentJudge(TwoDLineSegmentIntersectResult in_suspectResult, TwoDLineSegment in_suspectLine, SPoly* in_trackedSPolyCopyRef) :
			suspectResult(in_suspectResult),
			suspectLine(in_suspectLine),
			trackedSPolyCopyRef(in_trackedSPolyCopyRef)
		{};
		TwoDLineSegmentIntersectResult suspectResult;
		TwoDLineSegment suspectLine;
		SPoly* trackedSPolyCopyRef = nullptr;
};

#endif
