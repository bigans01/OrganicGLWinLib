#include "stdafx.h"
#include "TracingObserver.h"

void TracingObserver::setWeldedLinePoolRef(WeldedLinePool* in_weldedLinePoolRef)
{
	weldedLinePoolRef = in_weldedLinePoolRef;
}

void TracingObserver::buildNewObservation(WeldedLine in_lineOfSight, WeldedLine in_observationEndLine)
{
	remainingRadians = 1000.0f;
	lineOfSight = in_lineOfSight;
	observationEndLine = in_observationEndLine;
	//determineObservationRadians();
	determineObservationState();

	// only do this *while* (only test this while, don't run it entirely) we are in a CONTINUE_OBSERVE state.
	while (currentObserverState == TracingObserverState::CONTINUE_OBSERVE)
	{
		determineObservationRadians();


		if 	
		(
			(areRemainingRadiansValid == true)
			&&
			(checkIfLineOfSightIsBroken() == false)
		)
		{
			// 1. build a triangle from the new candidate line, remove lines 0 and 1 in the pool, insert a new value for line 0 in the pool, and the shift the remainders by 1.
			// 2. insert the triangle in some map
		}

		determineObservationState();	

	}

	if (currentObserverState != TracingObserverState::LINE_OF_SIGHT_BROKEN)
	{
		// run logic for final observe
		// run logic for finished
	}
}

bool TracingObserver::checkIfLineOfSightIsBroken()
{
	bool isLineOfSightBroken = false;


	// if there was an intersect found, the line of sight has become broken.
	if (isLineOfSightBroken == true)
	{
		currentObserverState = TracingObserverState::LINE_OF_SIGHT_BROKEN;
	}

	return isLineOfSightBroken;
}

void TracingObserver::determineObservationRadians()
{
	// use copies of lineOfSight and observationEndLine to calculate the radians
	WeldedLine lineOfSightCopy = lineOfSight;
	WeldedLine observationEndLineCopy = observationEndLine;	

	// do the calculation for radians
	// float radianValue = some function;
	// if (radianValue <= remainingRadians)
	// {
	//    areRemainingRadiansValid = true;
	// }
	// else
	// {
	//    areRemainingRadiansValue = false;
	// }
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