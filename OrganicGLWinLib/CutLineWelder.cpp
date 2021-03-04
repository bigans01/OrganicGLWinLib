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

	std::cout << "::::::::::::::: First two lines of typical run pool are: " << std::endl;
	currentPool.printLines();


	std::cout << "Handling TYPICAL run; nextCuttableLineIndexToUse is: " << nextCuttableLineIndexToUse << std::endl;
	std::cout << "TYPICAL-> beginning intersection line ID is: " << copiedAttempt.beginIntersectionLineID << std::endl;


	std::cout << "::::::::::::::: Cuttable triangle points are: " << std::endl;
	cuttableTriangleRef->printCuttableTrianglePoints();
	std::cout << "::::::::::::::: Cutting triangle points are: " << std::endl;
	cuttingTriangleRef->printPoints();

	glm::vec3 leadingPoint;
	bool continueFlag = true;
	int typicalTicks = 0;

	if (cuttableCyclingDirection == CyclingDirection::NOVAL)
	{
		std::cout << "!!!!!!!!!!! Warning, no cycling direction set! " << std::endl;
	}

	while (continueFlag == true)
	{
		// check whether or not the the line in the cuttableTriangle, having an index of nextCuttableLineIndexToUse, contains any 
		// registries. If it doesn't, enter the entire line into the pool, taking into account the FORWARD or REVERSE CyclingDirection.
		if (cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.numberOfRecords() == 0)
		{
			std::cout << "TYPICAL-> No records on line with index: " << nextCuttableLineIndexToUse << "; inserting this line as a new line into the pool. " << std::endl;
			CutLine newLine = produceEntireCuttableCutLineForPool(nextCuttableLineIndexToUse);
			leadingPoint = newLine.pointB;
			currentPool.insertLineIntoPool(newLine);
			std::cout << "TYPICAL-> Pool size is now: " << currentPool.getPoolSize() << std::endl;
			std::cout << "TYPICAL-> Leading point is now: " << leadingPoint.x << ", " << leadingPoint.y << ", " << leadingPoint.z << ", " << std::endl;
			nextCuttableLineIndexToUse = fetchNextLineViaCyclingDirection(nextCuttableLineIndexToUse, cuttableCyclingDirection);
		}

		else
		{
			// if we're here, it means we found a cuttable triangle line with records on it.
			// we must find the closest point, and use that as point B of the new line we are about to insert. 
			std::cout << "TYPICAL-> Records found on line with index: " << nextCuttableLineIndexToUse << std::endl;
			std::cout << "TYPICAL-> Leading point is now: " << leadingPoint.x << ", " << leadingPoint.y << ", " << leadingPoint.z << ", " << std::endl;
			int cuttingLineIDOfClosestPoint = cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.getIdOfClosestPoint(leadingPoint);
			glm::vec3 intersectedPointToUse = cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.getPointForRecord(cuttingLineIDOfClosestPoint);
			CutLine newLine = producePartialCuttableCutLineForPool(nextCuttableLineIndexToUse, intersectedPointToUse);
			currentPool.insertLineIntoPool(newLine);
			continueFlag = false;						// no point in continuing after this, as we'll be finished with the crawling.

			//std::cout << "TYPICAL-> Leading point is now: " << leadingPoint.x << ", " << leadingPoint.y << ", " << leadingPoint.z << ", " << std::endl;
			std::cout << "TYPICAL-> Final value of nextCuttableLineIndexToUse: " << nextCuttableLineIndexToUse << std::endl;


			// Next, determine what to do for line-crawling on the CuttingTriangle; should always produce at least 1 line that is built from the CuttingTriangle at this point.
			// use the closest point's cuttableLineID (cuttingLineIDOfClosestPoint) to find the non intersecting point for that line, that exists in the current cuttable line.
			glm::vec3 startingNonintersectingPoint = cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].getNonIntersectingPountForCuttingLine(cuttingLineIDOfClosestPoint);
			std::cout << "TYPICAL-> Non-intersecting point to use will be: " << startingNonintersectingPoint.x << ", " << startingNonintersectingPoint.y << ", " << startingNonintersectingPoint.z << std::endl;

			CyclingDirection cuttingTriangleCyclingDirection = CyclingDirection::NOVAL;	// the cycling direction we'll use for crawling the cutting triangle lines
			cuttingTriangleCyclingDirection = cuttingTriangleRef->cuttingLines[cuttingLineIDOfClosestPoint].determineCyclingDirectionToUseFromPoint(startingNonintersectingPoint);
			if (cuttingTriangleCyclingDirection == CyclingDirection::FORWARD)
			{
				std::cout << "TYPICAL-> will traverse FORWARD on cutting triangle. " << std::endl;
			}
			else if (cuttingTriangleCyclingDirection == CyclingDirection::REVERSE)
			{
				std::cout << "TYPICAL-> will traverse REVERSE on cutting triangle. " << std::endl;
			}
			int nextCuttingLineIndexToUse = fetchNextLineViaCyclingDirection(cuttingLineIDOfClosestPoint, cuttingTriangleCyclingDirection);
			std::cout << "TYPICAL-> next cutting triangle line index will be: " << nextCuttingLineIndexToUse << std::endl;
			
			// if the value of nextCuttingLineIndexToUse is equal to copiedAttempt.beginIntersectionLineID, we're done, and we just need to insert this line. 
			// Otherwise, if the value of nextCuttingLineIndexToUse isn't equal to copiedAttempt.beginIntersectionLineID, there is still a line to crawl on the CuttingTriangle.
			if (copiedAttempt.beginIntersectionLineID == nextCuttingLineIndexToUse)
			{
				std::cout << "TYPICAL-> nextCuttinLineIndexToUse equals copiedAttempt.beginIntersectionLineID; no remaining lines to crawl. " << std::endl;
				CutLine newLineFromCuttingTriangle = producePartialCuttingCutLineForPool(cuttingLineIDOfClosestPoint, intersectedPointToUse, startingNonintersectingPoint);
				currentPool.insertLineIntoPool(newLineFromCuttingTriangle);
			}

			// if the nextCuttingLineIndexToUse -- which is the ID of the next line in the CuttingTriangle to use, based on the chosen CyclingDirection -- 
			// doesn't match copiedAttempt.beginIntersectionLineID, it means that the CuttingTriangleLine in the CuttingTriangle having ID of nextCuttingLineIndexToUse will
			// have to be inserted entirely. For example -- when doing a CyclingDirection value of FORWARD -- if copiedAttempt.beginIntersectionLineID = 2, but nextCuttingLineIndexToUse is 0, it means that the line with index 
			// 1 is an entire line in the CuttingTriangle that would have to be inserted, after the call to producePartialCuttingCutLineForPool. Then, we are done.
			else
			{
				std::cout << "TYPICAL-> nextCuttinLineIndexToUse doesn't equal copiedAttempt.beginIntersectionLineID; must crawl an additional line. " << std::endl;
				CutLine newLineFromCuttingTriangle = producePartialCuttingCutLineForPool(cuttingLineIDOfClosestPoint, intersectedPointToUse, startingNonintersectingPoint);
				currentPool.insertLineIntoPool(newLineFromCuttingTriangle);

				// produce the second line, and we'll be done
				CutLine secondLine = produceEntireCuttingCutLineForPool(nextCuttingLineIndexToUse, cuttingTriangleCyclingDirection);
				currentPool.insertLineIntoPool(secondLine);

			}

		}

		typicalTicks++;
		if (typicalTicks >= 5)
		{
			std::cout << "###################################################################################################" << std::endl;
			std::cout << "###################################################################################################" << std::endl;
			std::cout << "!!!! Warning, unusual TYPICAL trace detected; please check previous output. " << std::endl;
			std::cout << "###################################################################################################" << std::endl;
			std::cout << "###################################################################################################" << std::endl;
			int typicalCheck = 3;
			std::cin >> typicalCheck;
			int infiniteLoop = 3;
			while (infiniteLoop == 3)
			{

			}
		}

	}
	std::cout << "TYPICAL run-> printing lines in pool..." << std::endl;
	currentPool.printLines();
}

void CutLineWelder::handleSliceRun() 
{
	int nextCuttableLineIndexToUse = fetchNextLineViaCyclingDirection(copiedAttempt.beginIntersectionLineID, cuttableCyclingDirection);
	std::cout << "Handling SLICE run; nextCuttableLineIndexToUse is: " << nextCuttableLineIndexToUse << std::endl;
	std::cout << "Copied attempt cutting triangle line ID: " << copiedAttempt.cuttingTriangleLineID << std::endl;
	std::cout << "Points of nextCuttableLine: " << std::endl;
	glm::vec3 tempPointA = cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].pointA;
	glm::vec3 tempPointB = cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].pointB;
	std::cout << "A: " << tempPointA.x << ", " << tempPointA.y << ", " << tempPointA.z << std::endl;
	std::cout << "B: " << tempPointB.x << ", " << tempPointB.y << ", " << tempPointB.z << std::endl;

	std::cout << "Registered points on the cutting line are: " << std::endl;
	cuttingTriangleRef->cuttingLines[copiedAttempt.cuttingTriangleLineID].printCuttableIntersections();

	std::cout << "::::::::::::::: Cuttable triangle points are: " << std::endl;
	cuttableTriangleRef->printCuttableTrianglePoints();
	std::cout << "::::::::::::::: Cutting triangle points are: " << std::endl;
	cuttingTriangleRef->printPoints();

	bool continueFlag = true;
	int sliceTicks = 0;
	while (continueFlag == true)
	{
		std::cout << "!! cuttingTriangleLineID is: " << copiedAttempt.cuttingTriangleLineID << std::endl;
		// check whether or not the the line in the cuttableTriangle, having an index of nextCuttableLineIndexToUse, contains any 
		// registries. If it doesn't, enter the entire line into the pool, taking into account the FORWARD or REVERSE CyclingDirection.

		// check if the selected endpoint of the next cuttable line is actually within the cutting line's intersection record manager
		glm::vec3 endpointOfNextCuttableLine = cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].fetchNextPointBasedOnCyclingDirection(cuttableCyclingDirection);
		bool wasPointFound = cuttingTriangleRef->cuttingLines[copiedAttempt.cuttingTriangleLineID].cuttingIntersectionManager.doesPointExist(endpointOfNextCuttableLine);
		if (wasPointFound == false)
		{
			if (cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.numberOfRecords() == 0)
			{
				std::cout << "SLICE-> No records on line with index: " << nextCuttableLineIndexToUse << "; inserting this line as a new line into the pool. " << std::endl;
				CutLine newLine = produceEntireCuttableCutLineForPool(nextCuttableLineIndexToUse);
				currentPool.insertLineIntoPool(newLine);
				std::cout << "SLICE-> Pool size is now: " << currentPool.getPoolSize() << std::endl;
				nextCuttableLineIndexToUse = fetchNextLineViaCyclingDirection(nextCuttableLineIndexToUse, cuttableCyclingDirection);
			}
			else if (cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.doesRecordExist(copiedAttempt.cuttingTriangleLineID) == true)
			{
				glm::vec3 intersectedPointToUse = cuttableTriangleRef->cuttableTriangleLines[nextCuttableLineIndexToUse].cuttableIntersectionManager.getPointForRecord(copiedAttempt.cuttingTriangleLineID);
				std::cout << "SLICE-> Records found on line with index: " << nextCuttableLineIndexToUse << std::endl;
				CutLine newLine = producePartialCuttableCutLineForPool(nextCuttableLineIndexToUse, intersectedPointToUse);
				currentPool.insertLineIntoPool(newLine);
				continueFlag = false;
			}
		}
		else if (wasPointFound == true)
		{
			glm::vec3 beginPointForTerminatingLine = currentPool.fetchLineFromPoolViaIndex(1).pointB;
			CutLine newLine = produceCutLineFromTerminatedSliceRun(nextCuttableLineIndexToUse, beginPointForTerminatingLine, endpointOfNextCuttableLine);
			currentPool.insertLineIntoPool(newLine);
			continueFlag = false;
		}

		sliceTicks++;
		if (sliceTicks >= 4)
		{
			std::cout << "###################################################################################################" << std::endl;
			std::cout << "###################################################################################################" << std::endl;
			std::cout << "!!!! Warning, unusual SLICE trace detected; please check previous output. " << std::endl;
			std::cout << "###################################################################################################" << std::endl;
			std::cout << "###################################################################################################" << std::endl;
			int typicalCheck = 3;
			std::cin >> typicalCheck;
			int infiniteLoop = 3;
			while (infiniteLoop == 3)
			{

			}
		}
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
		std::cout << "::Direction is REVERSE. Points of cuttable line are: " << std::endl;

		glm::vec3 cuttableA = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].pointA;
		glm::vec3 cuttableB = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].pointB;

		std::cout << "Point A: " << cuttableA.x << ", " << cuttableA.y << ", " << cuttableA.z << std::endl;
		std::cout << "Point B: " << cuttableB.x << ", " << cuttableB.y << ", " << cuttableB.z << std::endl;

		//std::cout << "Point A:

		returnLine.pointA = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].pointB;
		returnLine.pointB = in_intersectedPointToUse;
		returnLine.emptyNormal = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].cuttableTriangleCentroidFacingNormal;
	}
	return returnLine;
}

CutLine CutLineWelder::produceEntireCuttingCutLineForPool(int in_currentCuttingLineID, CyclingDirection in_cuttingTriangleCyclingDirection)
{
	CutLine returnLine;
	if (in_cuttingTriangleCyclingDirection == CyclingDirection::FORWARD)
	{
		returnLine.pointA = cuttingTriangleRef->cuttingLines[in_currentCuttingLineID].pointA;
		returnLine.pointB = cuttingTriangleRef->cuttingLines[in_currentCuttingLineID].pointB;
	}
	else if (in_cuttingTriangleCyclingDirection == CyclingDirection::REVERSE)
	{
		returnLine.pointA = cuttingTriangleRef->cuttingLines[in_currentCuttingLineID].pointB;
		returnLine.pointB = cuttingTriangleRef->cuttingLines[in_currentCuttingLineID].pointA;
	}
	returnLine.emptyNormal = cuttingTriangleRef->cuttingLines[in_currentCuttingLineID].outwardFacingNormal;

	return returnLine;
}

CutLine CutLineWelder::producePartialCuttingCutLineForPool(int in_currentCuttingLineID, glm::vec3 in_pointAForNewLine, glm::vec3 in_pointBForNewLine)
{
	CutLine returnLine;
	returnLine.pointA = in_pointAForNewLine;
	returnLine.pointB = in_pointBForNewLine;
	returnLine.emptyNormal = cuttingTriangleRef->cuttingLines[in_currentCuttingLineID].outwardFacingNormal;
	return returnLine;
}

CutLine CutLineWelder::produceCutLineFromTerminatedSliceRun(int in_currentCuttableLineID, glm::vec3 in_pointAForNewLine, glm::vec3 in_pointBForNewLine)
{
	CutLine returnLine;
	returnLine.pointA = in_pointAForNewLine;
	returnLine.pointB = in_pointBForNewLine;
	returnLine.emptyNormal = cuttableTriangleRef->cuttableTriangleLines[in_currentCuttableLineID].cuttableTriangleCentroidFacingNormal;
	return returnLine;
}