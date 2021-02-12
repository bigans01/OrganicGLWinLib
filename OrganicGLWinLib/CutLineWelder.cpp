#include "stdafx.h"
#include "CutLineWelder.h"
#include "CuttableTriangle.h"

CutLineWelder::CutLineWelder(CuttableTriangle* in_cuttableTriangleRef, 
							CuttingTriangle* in_cuttingTriangleRef, 
							TwoDCrawlingAttempt in_attemptCopy, 
							CutLinePool in_basePool,
							CyclingDirection in_cuttableCyclingDirection)
{
	cuttableTriangleRef = in_cuttableTriangleRef;
	cuttingTriangleRef = in_cuttingTriangleRef;
	copiedAttempt = in_attemptCopy;
	currentPool = in_basePool;
	cuttableCyclingDirection = in_cuttableCyclingDirection;

	if (copiedAttempt.crawlingType == TwoDCrawlingType::TYPICAL)
	{
		handleTypicalRun();
	}
	else if (copiedAttempt.crawlingType == TwoDCrawlingType::SLICE)
	{
		handleSliceRun();
	}
}

int CutLineWelder::fetchNextLineViaCyclingDirection(int in_currentLineID, CyclingDirection in_cyclingDirection)
{
	int nextIndex = 0;
	if (in_cyclingDirection == CyclingDirection::FORWARD)
	{
		if (in_currentLineID == 0)
		{
			nextIndex = 1;
		}
		else if (in_currentLineID == 1)
		{
			nextIndex = 2;
		}
		else if (in_currentLineID == 2)
		{
			nextIndex = 0;
		}
	}
	else if (in_cyclingDirection == CyclingDirection::REVERSE)
	{
		if (in_currentLineID == 0)
		{
			nextIndex = 2;
		}
		else if (in_currentLineID == 1)
		{
			nextIndex = 0;
		}
		else if (in_currentLineID == 2)
		{
			nextIndex = 1;
		}
	}
	return nextIndex;
}

void CutLineWelder::handleTypicalRun()
{
	int nextCuttableLineIndexToUse = fetchNextLineViaCyclingDirection(copiedAttempt.beginIntersectionLineID, cuttableCyclingDirection);
	std::cout << "Handling TYPICAL run; nextCuttableLineIndexToUse is: " << nextCuttableLineIndexToUse << std::endl;
	// check whether or not the the line in the cuttableTriangle, having an index of nextCuttableLineIndexToUse, contains any 
	// registries. If it doesn't, enter the entire line into the pool, taking into account the FORWARD or REVERSE CyclingDirection.
	if (cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.numberOfRecords() == 0)
	{
		std::cout << "TYPICAL-> No records on line with index: " << nextCuttableLineIndexToUse << "; inserting this line as a new line into the pool. " << std::endl;
		CutLine newLine = produceEntireCuttableCutLineForPool(nextCuttableLineIndexToUse);
		currentPool.insertLineIntoPool(newLine);
		std::cout << "TYPICAL-> Pool size is now: " << currentPool.getPoolSize() << std::endl;
	}
}

void CutLineWelder::handleSliceRun() 
{
	int nextCuttableLineIndexToUse = fetchNextLineViaCyclingDirection(copiedAttempt.beginIntersectionLineID, cuttableCyclingDirection);
	std::cout << "Handling SLICE run; nextCuttableLineIndexToUse is: " << nextCuttableLineIndexToUse << std::endl;
	bool continueFlag = true;
	while (continueFlag == true)
	{
		std::cout << "!! cuttingTriangleLineID is: " << copiedAttempt.cuttingTriangleLineID << std::endl;
		// check whether or not the the line in the cuttableTriangle, having an index of nextCuttableLineIndexToUse, contains any 
		// registries. If it doesn't, enter the entire line into the pool, taking into account the FORWARD or REVERSE CyclingDirection.
		if (cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.numberOfRecords() == 0)
		{
			std::cout << "SLICE-> No records on line with index: " << nextCuttableLineIndexToUse << "; inserting this line as a new line into the pool. " << std::endl;
			CutLine newLine = produceEntireCuttableCutLineForPool(nextCuttableLineIndexToUse);
			currentPool.insertLineIntoPool(newLine);
			std::cout << "SLICE-> Pool size is now: " << currentPool.getPoolSize() << std::endl;
		}
		//else if (cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.numberOfRecords() != 0)
		else if (cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.doesRecordExist(copiedAttempt.cuttingTriangleLineID) == true)
		{
			glm::vec3 intersectedPointToUse = cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.getPointForRecord(copiedAttempt.cuttingTriangleLineID);
			std::cout << "SLICE-> Records found on line with index: " << nextCuttableLineIndexToUse << std::endl;
			CutLine newLine = producePartialCuttableCutLineForPool(nextCuttableLineIndexToUse, intersectedPointToUse);
			currentPool.insertLineIntoPool(newLine);
			continueFlag = false;
		}
		nextCuttableLineIndexToUse = fetchNextLineViaCyclingDirection(nextCuttableLineIndexToUse, cuttableCyclingDirection);
	}
	std::cout << "SLICE run-> printing lines in pool..." << std::endl;
	currentPool.printLines();
}

CutLine CutLineWelder::produceEntireCuttableCutLineForPool(int in_currentCuttableLineID)
{
	CutLine returnLine;
	if (cuttableCyclingDirection == CyclingDirection::FORWARD)
	{
		returnLine.pointA = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].pointA;
		returnLine.pointB = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].pointB;
		returnLine.emptyNormal = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].cuttableTriangleCentroidFacingNormal;
	}
	else if (cuttableCyclingDirection == CyclingDirection::REVERSE)
	{
		returnLine.pointA = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].pointB;
		returnLine.pointB = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].pointA;
		returnLine.emptyNormal = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].cuttableTriangleCentroidFacingNormal;
	}
	return returnLine;
}

CutLine CutLineWelder::producePartialCuttableCutLineForPool(int in_currentCuttableLineID, glm::vec3 in_intersectedPointToUse)
{
	CutLine returnLine;
	if (cuttableCyclingDirection == CyclingDirection::FORWARD)
	{
		returnLine.pointA = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].pointA;
		returnLine.pointB = in_intersectedPointToUse;
		returnLine.emptyNormal = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].cuttableTriangleCentroidFacingNormal;
	}
	else if (cuttableCyclingDirection == CyclingDirection::REVERSE)
	{
		returnLine.pointA = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].pointB;
		returnLine.pointB = in_intersectedPointToUse;
		returnLine.emptyNormal = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].cuttableTriangleCentroidFacingNormal;
	}
	return returnLine;
}