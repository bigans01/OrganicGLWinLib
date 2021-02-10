#pragma once

#ifndef TWODCRAWLINGATTEMPT_H
#define TWODCRWALINGATTEMPT_H

#include "TwoDCrawlingType.h"

class TwoDCrawlingAttempt
{
	public:
		TwoDCrawlingAttempt() {};
		TwoDCrawlingAttempt(TwoDCrawlingType in_crawlingType, 
							int in_cuttingTriangleLineID, 
							int in_beginIntersectionLineID,
							glm::vec3 in_beginIntersectingPoint) :
			crawlingType(in_crawlingType),
			cuttingTriangleLineID(in_cuttingTriangleLineID),
			beginIntersectionLineID(in_beginIntersectionLineID),
			beginIntersectingPoint(in_beginIntersectingPoint)
		{};
		TwoDCrawlingAttempt(TwoDCrawlingType in_crawlingType, 
							int in_cuttingTriangleLineID, 
							int in_beginIntersectionLineID, 
							glm::vec3 in_beginIntersectingPoint,
							int in_endIntersectionLineID,
							glm::vec3 in_endIntersectingPoint) :
			crawlingType(in_crawlingType),
			cuttingTriangleLineID(in_cuttingTriangleLineID),
			beginIntersectionLineID(in_beginIntersectionLineID),
			beginIntersectingPoint(in_beginIntersectingPoint),
			endIntersectionLineID(in_endIntersectionLineID),
			endIntersectingPoint(in_endIntersectingPoint)
		{};
		TwoDCrawlingType crawlingType = TwoDCrawlingType::UNDETERMINED;
		int cuttingTriangleLineID = 0;
		int beginIntersectionLineID = 0;
		glm::vec3 beginIntersectingPoint;
		int endIntersectionLineID = 0;
		glm::vec3 endIntersectingPoint;
};

#endif
