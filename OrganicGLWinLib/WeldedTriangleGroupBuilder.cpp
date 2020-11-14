#include "stdafx.h"
#include "WeldedTriangleGroupBuilder.h"

void WeldedTriangleGroupBuilder::setWeldedLinePool(WeldedLinePool in_weldedlinePool)
{
	linePool = in_weldedlinePool;
}

void WeldedTriangleGroupBuilder::runTracingObservers()
{
	tracer.setWeldedLinePoolRef(&linePool);
	tracer.setWeldedTriangleVectorRef(&weldedTriangleVector);
	currentLineOfSightLineIndex = linePool.getFirstElementID();	// this value should always be set to the initial key value of the first element in the linePool; the initial value should always be 0

	WeldedLinePoolGuide poolGuide(currentLineOfSightLineIndex, &linePool);	// testing only, remove when needed.
	//WeldedLinePoolGuide poolGuide(8, &linePool);	// testing only, remove when needed.
	//poolGuide.printGuideValues();

	// un-comment below block when ready to test.

	//std::cout << "getPoolSize, pre check" << std::endl;
	
	while (linePool.getPoolSize() > 3)		// when we remove the last of 4 lines from the pool, that's when we stop, as it means there are 3 lines left (only one triangle left to form)
	{
		//std::cout << "###~~~~~ Building new observation. " << std::endl;
		//std::cout << " ### Size of line pool is: " << linePool.getPoolSize() << std::endl;
		acquireWeldedLinesForWindowAndBuildObservation();
	}

	//std::cout << "getPoolSize, pre check 2" << std::endl;

	acquireWeldedLinesForWindowAndBuildObservation();

	//std::cout << "getPoolSize, post check" << std::endl;

	std::cout << "Estimated number of triangles that will be produced is: " << weldedTriangleVector.size() << std::endl;
	//int someVal = 5;
	//std::cin >> someVal;
	
}

void WeldedTriangleGroupBuilder::acquireWeldedLinesForWindowAndBuildObservation()
{
	//auto windowObservationEndLine = linePool.fetchLastLineInPool();
	WeldedLinePoolGuide poolGuide(currentLineOfSightLineIndex, &linePool);
	//tracer.buildNewObservation(linePool.fetchLineFromPoolViaIndex(poolGuide.lineOfSightLineIndex), linePool.fetchLineFromPoolViaIndex(poolGuide.observationEndLineIndex));
	tracer.buildNewObservation(poolGuide);		// build it, let it run
	if (tracer.getCurrentObserverState() == TracingObserverState::TERMINATED)	// if it was prematurely terminated, we must increment the currentLineOfSightIndex
																				// before creating the new poolGuide
	{
		currentLineOfSightLineIndex = linePool.fetchFollowingIndex(currentLineOfSightLineIndex);
	}


	// if the result of the observation is a termination, fetch whatever was produced, and increment the currentObservationLineIndex by one.
}