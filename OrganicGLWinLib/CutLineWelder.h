#pragma once

#ifndef CUTLINEWELDER_H
#define CUTLINEWELDER_H

#include "CuttingTriangle.h"
#include "TwoDCrawlingAttempt.h"
#include "CutLinePool.h"
#include "CyclingDirection.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "ErrorSensor.h"

class CuttableTriangle;
class CutLineWelder
{
	private:
		CutLineWelder(CuttableTriangle* in_cuttableTriangleRef, 
					 CuttingTriangle* in_cuttingTriangleRef, 
					 TwoDCrawlingAttempt in_crawlingAttempt, 
					 CutLinePool in_basePool,
			         CyclingDirection in_cuttableCyclingDirection,
			         PolyDebugLevel in_polyDebugLevel);
		friend class CuttableTriangle;
		CuttableTriangle* cuttableTriangleRef = nullptr;
		CuttingTriangle* cuttingTriangleRef = nullptr;
		TwoDCrawlingAttempt copiedAttempt;
		CutLinePool currentPool;
		CyclingDirection cuttableCyclingDirection;
		PolyLogger cutLineWelderLogger;
		PolyDebugLevel cutLineWelderDebugLevel = PolyDebugLevel::NONE;

		int fetchNextLineViaCyclingDirection(int in_currentLineID, CyclingDirection in_cyclingDirection);
		ErrorSensor executeRun();
		ErrorSensor handleTypicalRun();
		ErrorSensor handleSliceRun();
		CutLine produceEntireCuttableCutLineForPool(int in_currentCuttableLineID);
		CutLine producePartialCuttableCutLineForPool(int in_currentCuttableLineID, glm::vec3 in_intersectedPointToUse);
		CutLine produceEntireCuttingCutLineForPool(int in_currentCuttableLineID, CyclingDirection in_cuttingTriangleCyclingDirection);
		CutLine producePartialCuttingCutLineForPool(int in_currentCuttingLineID, glm::vec3 in_pointAForNewLine, glm::vec3 in_pointBForNewLine);
		CutLine produceCutLineFromTerminatedSliceRun(int in_currentCuttingLineID, glm::vec3 in_pointAForNewLine, glm::vec3 in_pointBForNewLine);
};

#endif
