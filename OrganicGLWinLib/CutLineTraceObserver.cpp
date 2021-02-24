#include "stdafx.h"
#include "CutLineTraceObserver.h"

void CutLineTraceObserver::setCutLinePoolRef(CutLinePool* in_cutLinePoolRef)
{
	cutLinePoolRef = in_cutLinePoolRef;
}

void CutLineTraceObserver::setCutTriangleContainerVectorRef(std::vector<CutTriangleContainer>* in_cutTriangleContainerVectorRef)
{
	cutTriangleContainerVectorRef = in_cutTriangleContainerVectorRef;
}

void CutLineTraceObserver::setObservationLogLevel(PolyDebugLevel in_polyDebugLevel)
{
	cutLineTraceObserverLogger.setDebugLevel(in_polyDebugLevel);
}

void CutLineTraceObserver::buildNewCutObservation(CutLinePoolGuide in_cutLinePoolGuide)
{
	currentContainer.cutTrianglesMap.clear();	// clear, before we insert new triangles.
	poolGuide = in_cutLinePoolGuide;
	remainingRadians = 1000.0f;
	lineOfSight = cutLinePoolRef->fetchLineFromPoolViaIndex(poolGuide.lineOfSightLineIndex);
	observationEndLine = cutLinePoolRef->fetchLineFromPoolViaIndex(poolGuide.observationEndLineIndex);

	determineObservationState();

	// only do this *while* (only test this while, don't run it entirely) we are in a CONTINUE_OBSERVE state.
	while (currentObserverState == TracingObserverState::CONTINUE_OBSERVE)
	{
		//std::cout << "::::: Running tick for TracingObserver, CONTINUE_OBSERVE: " << std::endl;
		//poolGuide.printGuideValues();

		determineObservationRadians();



		if
			(
			(areRemainingRadiansValid == true)
				&&
				(insertWeldedTriangleIfLineOfSightIsMaintained() == true)
				)
		{
			// 1. build a triangle from the new candidate line, remove lines 0 and 1 in the pool, insert a new value for line 0 in the pool, and the shift the remainders by 1.

			// NOTE: be sure to invert the last line in the WeldedTriangle, before inserting as the "new" line into the pool.

			// 2. insert the triangle in some map

			determineObservationState();	// 3. update the state when we're done, to see if we terminate the while loop.
		}
		else
		{
			currentObserverState = TracingObserverState::TERMINATED;
		}

		//int someVal = 3;
		//std::cout << "::: Enter number to continue the loop of CONTINUE_OBSERVE. " << std::endl;
		//std::cin >> someVal;
	}

	if (currentObserverState != TracingObserverState::TERMINATED)
	{
		std::cout << "!! State is not Terminated. " << std::endl;
		if (currentObserverState == TracingObserverState::FINAL_OBSERVE)
		{
			// run logic for final observe

			std::cout << "!! State is not Terminated, but entering FINAL_OBSERVE. " << std::endl;

			if
				(
				(areRemainingRadiansValid == true)
					&&
					(insertWeldedTriangleIfLineOfSightIsMaintained() == true)
					)
			{
				// 1. build a triangle from the new candidate line, remove lines 0 and 1 in the pool, insert a new value for line 0 in the pool, and the shift the remainders by 1.

				// NOTE: be sure to invert the last line in the WeldedTriangle, before inserting as the "new" line into the pool.

				// 2. insert the triangle in some map

				determineObservationState();	// 3. update the state when we're done, to see if we terminate the while loop.
			}
			else
			{
				currentObserverState = TracingObserverState::TERMINATED;
			}

			//std::cout << "!!!! Final observe detected. Enter number to continue. " << std::endl;
			//int someVal = 3;
			//std::cin >> someVal;
		}
		else if (currentObserverState == TracingObserverState::FINISHED)
		{
			// we're finished, just insert the final WeldedTriangle.
			//std::cout << ":::: Test; - observation is finished, printing out the lines of the last WeldedTriangle: " << std::endl;
			//tracingObserverLogger.log("(TracingObserver) :::: Test; - observation is finished, printing out the lines of the last WeldedTriangle: ", "\n");
			//weldedLinePoolRef->printLines();
			CutTriangle finalTriangle(cutLinePoolRef->fetchLineFromPoolViaIndex(0), cutLinePoolRef->fetchLineFromPoolViaIndex(1), cutLinePoolRef->fetchLineFromPoolViaIndex(2));
			//weldedTriangleVectorRef->push_back(finalTriangle);
			//weldedTriangleVectorRef->insertWeldedTriangle(finalTriangle);
			currentContainer.insertWeldedTriangle(finalTriangle);
		}
		// run logic for finished
	}
}

bool CutLineTraceObserver::insertWeldedTriangleIfLineOfSightIsMaintained()
{
	bool isLineOfSightMaintained = true;

	//std::cout << "###### (1) Pool size is: " << poolGuide.weldedLinePoolRef->getPoolSize() << std::endl;

	// need a new class here, that will take in a copy of the line pool and the current pool guide, and use it to verify the line of sight.
	// this class should return a value of "true" or "false" which can then be used.
	CutTriangleProducer triangleProducer(cutLinePoolRef, poolGuide);
	//std::cout << "###### (2) Pool size is: " << poolGuide.weldedLinePoolRef->getPoolSize() << std::endl;
	//isLineOfSightMaintained = triangleProducer.wasValidWeldedTriangleProduced;
	CutTriangleProductionResult productionResult = triangleProducer.getProductionResult();
	isLineOfSightMaintained = productionResult.wasProductionValid;

	/*
	auto comparablesBegin = poolGuide.comparables.begin();
	auto comparablesEnd = poolGuide.comparables.end();
	for (; comparablesBegin != comparablesEnd; comparablesBegin++)
	{
		std::cout << "!! _> Comparison needed against line: " << *comparablesBegin << std::endl;
		// run function to get bool value for isLineOfSightBroken here.
	}
	*/

	// if there was an intersect found, the line of sight has become broken.
	if (isLineOfSightMaintained == false)
	{
		std::cout << "###### This observation has been TERMINATED. " << std::endl;
		std::cout << "###### (3) Pool size is: " << poolGuide.weldedLinePoolRef->getPoolSize() << std::endl;
		currentObserverState = TracingObserverState::TERMINATED;
	}
	else if (isLineOfSightMaintained == true)
	{
		std::cout << "!!!! Line of sight is MAINTAINED for the produced triangle; it is VALID! " << std::endl;
		CutLine invertedLine = productionResult.producedCutTriangle.fetchTriangleLine(2);
		invertedLine.swapPointsAndInvertNormal();

		// insert the triangle into the WeldedTriangle map (or vector?)
		currentContainer.insertWeldedTriangle(productionResult.producedCutTriangle);
		poolGuide.updateGuide(poolGuide.lineOfSightLineIndex, invertedLine);
		//poolGuide.weldedLinePoolRef->printLines();

		lineOfSight = cutLinePoolRef->fetchLineFromPoolViaIndex(poolGuide.lineOfSightLineIndex);
		observationEndLine = cutLinePoolRef->fetchLineFromPoolViaIndex(poolGuide.observationEndLineIndex);
	}

	return isLineOfSightMaintained;
}

TracingObserverState CutLineTraceObserver::getCurrentObserverState()
{
	return currentObserverState;
}

void CutLineTraceObserver::determineObservationRadians()
{
	// use copies of lineOfSight and observationEndLine to calculate the radians
	CutLine lineOfSightCopy = lineOfSight;
	CutLine observationEndLineCopy = observationEndLine;

	// lineOfSightCopy will be  pointBRef in the quatRotationManager (index 3)
	QuatRotationPoints points;			
	points.insertPointRefs(&observationEndLineCopy.pointA, &observationEndLineCopy.pointB, &lineOfSightCopy.pointA, &lineOfSightCopy.pointB);

	// adjust the x and y to 0, if need be.
	// do a translation check.
	glm::vec3 pointToTranslateAgainst;
	pointToTranslateAgainst = points.getPointByIndex(1);
	PointTranslationCheck translationChecker;
	translationChecker.performCheck(pointToTranslateAgainst);
	if (translationChecker.requiresTranslation == 1)
	{
		points.applyTranslation(translationChecker.getTranslationValue());
	}

	// last two elements of vector will be the empty normals; but should only be inserted post translation.
	// index 5 = should check in the QuatRotationManager for flipping on the Y axis
	points.insertPointRefs(&observationEndLineCopy.emptyNormal, &lineOfSightCopy.emptyNormal);

	QuatRotationManager rotationManager;
	float calculatedRadians = rotationManager.initializeAndRunForFindingObserverRadians(&points);

	//std::cout << ":::::::::::::: Value of remainingRadians: " << remainingRadians << std::endl;
	//std::cout << ":::::::::::::: Value of calculatedRadians: " << calculatedRadians << std::endl;

	// verify observation validity for radians
	if (calculatedRadians < remainingRadians)
	{
		areRemainingRadiansValid = true;
		//std::cout << "!> Observation is still VALID!" << std::endl;
	}
	else
	{
		areRemainingRadiansValid = false;
		//std::cout << "!> Observation is NO LONGER VALID!" << std::endl;
	}

	remainingRadians = calculatedRadians;
}

void CutLineTraceObserver::determineObservationState()
{

	int weldedPoolCount = cutLinePoolRef->getPoolSize();

	// if the number of lines in the pool is 3, we should be FINISHED.
	if (weldedPoolCount == 3)
	{
		currentObserverState = TracingObserverState::FINISHED;
	}
	else if (weldedPoolCount == 4)		// we can do one final observation, but there are no comparables to do intersect tests against.
	{
		std::cout << "!!! This shall be a FINAL_OBSERVE" << std::endl;
		currentObserverState = TracingObserverState::FINAL_OBSERVE;
	}
	else if (weldedPoolCount >= 5)
	{
		currentObserverState = TracingObserverState::CONTINUE_OBSERVE;
	}
}
