#pragma once

#ifndef TWODCRAWLINGATTEMPT_H
#define TWODCRWALINGATTEMPT_H

#include "TwoDCrawlingType.h"

class TwoDCrawlingAttempt
{
	public:
		TwoDCrawlingAttempt() {};
		TwoDCrawlingAttempt(TwoDCrawlingType in_crawlingType, int in_cuttingTriangleLineID, int in_beginIntersectionLineID, int in_endIntersectionLineID) :
			crawlingType(in_crawlingType),
			cuttingTriangleLineID(in_cuttingTriangleLineID),
			beginIntersectionLineID(in_beginIntersectionLineID),
			endIntersectionLineID(in_endIntersectionLineID)
		{};
		TwoDCrawlingType crawlingType = TwoDCrawlingType::UNDETERMINED;
		int cuttingTriangleLineID = 0;
		int beginIntersectionLineID = 0;
		int endIntersectionLineID = 0;
};

#endif
