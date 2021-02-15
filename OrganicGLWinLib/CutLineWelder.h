#pragma once

#ifndef CUTLINEWELDER_H
#define CUTLINEWELDER_H

#include "CuttingTriangle.h"
#include "TwoDCrawlingAttempt.h"
#include "CutLinePool.h"
#include "CyclingDirection.h"

class CuttableTriangle;
class CutLineWelder
{
	public:
		CutLineWelder(CuttableTriangle* in_cuttableTriangleRef, 
					 CuttingTriangle* in_cuttingTriangleRef, 
					 TwoDCrawlingAttempt in_crawlingAttempt, 
					 CutLinePool in_basePool,
			         CyclingDirection in_cuttableCyclingDirection);
	private:
		CuttableTriangle* cuttableTriangleRef = nullptr;
		CuttingTriangle* cuttingTriangleRef = nullptr;
		TwoDCrawlingAttempt copiedAttempt;
		CutLinePool currentPool;
		CyclingDirection cuttableCyclingDirection;
		int fetchNextLineViaCyclingDirection(int in_currentLineID, CyclingDirection in_cyclingDirection);
		void handleTypicalRun();
		void handleSliceRun();
		CutLine produceEntireCuttableCutLineForPool(int in_currentCuttableLineID);
		CutLine producePartialCuttableCutLineForPool(int in_currentCuttableLineID, glm::vec3 in_intersectedPointToUse);
		CutLine producePartialCuttingCutLineForPool(int in_currentCuttingLineID, glm::vec3 in_pointAForNewLine, glm::vec3 in_pointBForNewLine);
};

#endif
