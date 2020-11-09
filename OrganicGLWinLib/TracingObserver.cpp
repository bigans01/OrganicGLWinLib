#include "stdafx.h"
#include "TracingObserver.h"

void TracingObserver::setWeldedLinePoolRef(WeldedLinePool* in_weldedLinePoolRef)
{
	weldedLinePoolRef = in_weldedLinePoolRef;
}

void TracingObserver::setWeldedTriangleVectorRef(std::vector<WeldedTriangle>* in_weldedTriangleVectorRef)
{
	weldedTriangleVectorRef = in_weldedTriangleVectorRef;
}

void TracingObserver::buildNewObservation(WeldedLinePoolGuide in_poolGuide)
{
	poolGuide = in_poolGuide;
	remainingRadians = 1000.0f;
	lineOfSight = weldedLinePoolRef->fetchLineFromPoolViaIndex(poolGuide.lineOfSightLineIndex);
	observationEndLine = weldedLinePoolRef->fetchLineFromPoolViaIndex(poolGuide.observationEndLineIndex);

	//determineObservationRadians();
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
			(checkIfLineOfSightIsMaintained() == true)
		)
		{
			// 1. build a triangle from the new candidate line, remove lines 0 and 1 in the pool, insert a new value for line 0 in the pool, and the shift the remainders by 1.

			// NOTE: be sure to invert the last line in the WeldedTriangle, before inserting as the "new" line into the pool.

			// 2. insert the triangle in some map

			determineObservationState();	// 3. update the state when we're done, to see if we terminate the while loop.
		}
		else
		{
			currentObserverState == TracingObserverState::TERMINATED;
		}

		//int someVal = 3;
		//std::cout << "::: Enter number to continue the loop of CONTINUE_OBSERVE. " << std::endl;
		//std::cin >> someVal;
	}

	if (currentObserverState != TracingObserverState::TERMINATED)
	{
		if (currentObserverState == TracingObserverState::FINAL_OBSERVE)
		{
			// run logic for final observe
			if
			(
				(areRemainingRadiansValid == true)
				&&
				(checkIfLineOfSightIsMaintained() == true)
			)
			{
				// 1. build a triangle from the new candidate line, remove lines 0 and 1 in the pool, insert a new value for line 0 in the pool, and the shift the remainders by 1.

				// NOTE: be sure to invert the last line in the WeldedTriangle, before inserting as the "new" line into the pool.

				// 2. insert the triangle in some map

				determineObservationState();	// 3. update the state when we're done, to see if we terminate the while loop.
			}
			else
			{
				currentObserverState == TracingObserverState::TERMINATED;
			}

			//std::cout << "!!!! Final observe detected. Enter number to continue. " << std::endl;
			//int someVal = 3;
			//std::cin >> someVal;
		}
		else if (currentObserverState == TracingObserverState::FINISHED)
		{
			// we're finished, just insert the final WeldedTriangle.
			//std::cout << ":::: Test; - observation is finished, printing out the lines of the last WeldedTriangle: " << std::endl;
			//weldedLinePoolRef->printLines();
			WeldedTriangle finalTriangle(weldedLinePoolRef->fetchLineFromPoolViaIndex(0), weldedLinePoolRef->fetchLineFromPoolViaIndex(1), weldedLinePoolRef->fetchLineFromPoolViaIndex(2));
			weldedTriangleVectorRef->push_back(finalTriangle);
		}
		// run logic for finished
	}
}

TracingObserverState TracingObserver::getCurrentObserverState()
{
	return currentObserverState;
}

bool TracingObserver::checkIfLineOfSightIsMaintained()
{
	bool isLineOfSightMaintained = true;

	//std::cout << "###### (1) Pool size is: " << poolGuide.weldedLinePoolRef->getPoolSize() << std::endl;

	// need a new class here, that will take in a copy of the line pool and the current pool guide, and use it to verify the line of sight.
	// this class should return a value of "true" or "false" which can then be used.
	WeldedTriangleProducer triangleProducer(weldedLinePoolRef, poolGuide);
	//std::cout << "###### (2) Pool size is: " << poolGuide.weldedLinePoolRef->getPoolSize() << std::endl;
	//isLineOfSightMaintained = triangleProducer.wasValidWeldedTriangleProduced;
	WeldedTriangleProductionResult productionResult = triangleProducer.getProductionResult();
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
		//std::cout << "###### This observation has been TERMINATED. " << std::endl;
		//std::cout << "###### (3) Pool size is: " << poolGuide.weldedLinePoolRef->getPoolSize() << std::endl;
		currentObserverState = TracingObserverState::TERMINATED;
	}
	else if (isLineOfSightMaintained == true)
	{
		//std::cout << "!!!! Line of sight is MAINTAINED for the produced triangle; it is VALID! " << std::endl;
		WeldedLine invertedLine = productionResult.producedTriangle.fetchTriangleLine(2);
		invertedLine.swapPointsAndInvertNormal();

		// insert the triangle into the WeldedTriangle map (or vector?)
		weldedTriangleVectorRef->push_back(productionResult.producedTriangle);

		//productionResult.producedTriangle.printPoints();
		poolGuide.updateGuide(poolGuide.lineOfSightLineIndex, invertedLine);
		//poolGuide.weldedLinePoolRef->printLines();

		

		lineOfSight = weldedLinePoolRef->fetchLineFromPoolViaIndex(poolGuide.lineOfSightLineIndex);
		observationEndLine = weldedLinePoolRef->fetchLineFromPoolViaIndex(poolGuide.observationEndLineIndex);
	}

	return isLineOfSightMaintained;
}

void TracingObserver::determineObservationRadians()
{
	// use copies of lineOfSight and observationEndLine to calculate the radians
	WeldedLine lineOfSightCopy = lineOfSight;
	WeldedLine observationEndLineCopy = observationEndLine;	

	QuatRotationPoints points;
	points.pointsRefVector.push_back(&observationEndLineCopy.pointA);
	points.pointsRefVector.push_back(&observationEndLineCopy.pointB);
	points.pointsRefVector.push_back(&lineOfSightCopy.pointA);
	points.pointsRefVector.push_back(&lineOfSightCopy.pointB);				// this will be  pointBRef in the quatRotationManager (index 3)

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
	points.pointsRefVector.push_back(&observationEndLineCopy.emptyNormal);
	points.pointsRefVector.push_back(&lineOfSightCopy.emptyNormal);			// index 5, which should check in the QuatRotationManager for flipping on the Y axis

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

void TracingObserver::determineObservationState()
{
	
	int weldedPoolCount = weldedLinePoolRef->getPoolSize();

	// if the number of lines in the pool is 3, we should be FINISHED.
	if (weldedPoolCount == 3)
	{
		currentObserverState = TracingObserverState::FINISHED;
	}
	else if (weldedPoolCount == 4)		// we can do one final observation, but there are no comparables to do intersect tests against.
	{
		currentObserverState = TracingObserverState::FINAL_OBSERVE;
	}
	else if (weldedPoolCount >= 5)
	{
		currentObserverState = TracingObserverState::CONTINUE_OBSERVE;
	}
}