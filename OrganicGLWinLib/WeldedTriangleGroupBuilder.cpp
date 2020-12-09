#include "stdafx.h"
#include "WeldedTriangleGroupBuilder.h"

void WeldedTriangleGroupBuilder::setWeldedLinePool(WeldedLinePool in_weldedlinePool)
{
	linePool = in_weldedlinePool;
}

void WeldedTriangleGroupBuilder::runTracingObservers()
{
	tracer.setWeldedLinePoolRef(&linePool);
	tracer.setWeldedTriangleContainerVectorRef(&weldedTriangleContainerVector);
	currentLineOfSightLineIndex = linePool.getFirstElementID();	// this value should always be set to the initial key value of the first element in the linePool; the initial value should always be 0

	WeldedLinePoolGuide poolGuide(currentLineOfSightLineIndex, &linePool);	// testing only, remove when needed.
	//WeldedLinePoolGuide poolGuide(8, &linePool);	// testing only, remove when needed.
	//poolGuide.printGuideValues();

	// un-comment below block when ready to test.

	std::cout << "getPoolSize, pre check: " << linePool.getPoolSize() << std::endl;
	
	if (linePool.getPoolSize() > 3)
	{
		while (linePool.getPoolSize() > 3)		// when we remove the last of 4 lines from the pool, that's when we stop, as it means there are 3 lines left (only one triangle left to form)
		{
			//std::cout << "###~~~~~ Building new observation. " << std::endl;
			//std::cout << " ### Size of line pool is: " << linePool.getPoolSize() << std::endl;
			acquireWeldedLinesForWindowAndBuildObservation();
		}

		std::cout << "getPoolSize, post check: " << linePool.getPoolSize() << std::endl;

		//std::cout << "getPoolSize, pre check 2" << std::endl;

		//acquireWeldedLinesForWindowAndBuildObservation();
		handleFinalObservation();
	}
	else if (linePool.getPoolSize() == 3)
	{
		WeldedTriangle newTriangle(linePool.fetchLineFromPoolViaIndex(0), linePool.fetchLineFromPoolViaIndex(1), linePool.fetchLineFromPoolViaIndex(2));
		tracer.currentContainer.insertWeldedTriangle(newTriangle);
		weldedTriangleContainerVector.push_back(tracer.currentContainer);
		linePool.clearPool();
	}
	//std::cout << "getPoolSize, post check" << std::endl;

	//std::cout << "Estimated number of triangles that will be produced is: " << weldedTriangleContainerVector.triangleMap.size() << std::endl;

	//int someVal = 5;
	//std::cin >> someVal;
	
}

void WeldedTriangleGroupBuilder::acquireWeldedLinesForWindowAndBuildObservation()
{
	//auto windowObservationEndLine = linePool.fetchLastLineInPool();
	WeldedLinePoolGuide poolGuide(currentLineOfSightLineIndex, &linePool);
	//tracer.buildNewObservation(linePool.fetchLineFromPoolViaIndex(poolGuide.lineOfSightLineIndex), linePool.fetchLineFromPoolViaIndex(poolGuide.observationEndLineIndex));
	tracer.buildNewObservation(poolGuide);		// build it, let it run
	weldedTriangleContainerVector.push_back(tracer.currentContainer);
	if (tracer.getCurrentObserverState() == TracingObserverState::TERMINATED)	// if it was prematurely terminated, we must increment the currentLineOfSightIndex
																				// before creating the new poolGuide
	{
		currentLineOfSightLineIndex = linePool.fetchFollowingIndex(currentLineOfSightLineIndex);
	}

	// if the result of the observation is a termination, fetch whatever was produced, and increment the currentObservationLineIndex by one.
}

void WeldedTriangleGroupBuilder::handleFinalObservation()
{
	// this would run once the observation is marked as FINISHED.
	WeldedLinePoolGuide poolGuide(currentLineOfSightLineIndex, &linePool);
	tracer.buildNewObservation(poolGuide);		// build it, let it run

	std::cout << "Fetch (1) " << std::endl;
	std::cout << "weldedTriangleContainerVector size is: " << weldedTriangleContainerVector.size() << std::endl;
	
	auto lastContainer = weldedTriangleContainerVector.rbegin();
	auto lastTriangleInLastContainer = lastContainer->triangleMap.rbegin();
	auto tracerContainer = tracer.currentContainer.triangleMap.begin();	// since there's only one triangle that will be produced, just get a pointer to it.

	// we will compare the points of the last line in the most recent triangle, to the first line in the newest triangle we are about to add.

	std::cout << "Fetch (2) " << std::endl;

	WeldedLine* mostRecentTriangleWeldedLineRef = lastTriangleInLastContainer->second.fetchTriangleLineRef(2);	
	WeldedLine* lastTriangleWeldedLineRef = tracerContainer->second.fetchTriangleLineRef(0);

	std::cout << "*** Prior to welded line printout " << std::endl;
	std::cout << "Most recent welded line, point A: " << mostRecentTriangleWeldedLineRef->pointA.x << ", " << mostRecentTriangleWeldedLineRef->pointA.y << ", " << mostRecentTriangleWeldedLineRef->pointA.z << std::endl;
	std::cout << "Last triangle welded line, point A: " << lastTriangleWeldedLineRef->pointA.x << ", " << lastTriangleWeldedLineRef->pointA.y << ", " << lastTriangleWeldedLineRef->pointA.z << std::endl;

	bool isAligned = false;
	while (isAligned == false)
	{
		if
		(
			// the first line in the new triangle is equivalent to the reverse of line 2 in the preceding triangle, when aligned properly.
			(lastTriangleWeldedLineRef->pointA == mostRecentTriangleWeldedLineRef->pointB)
			&&
			(lastTriangleWeldedLineRef->pointB == mostRecentTriangleWeldedLineRef->pointA)
		)
		{
			//std::cout << "Triangle is aligned! " << std::endl;
			isAligned = true;
		}
		else
		{
			tracerContainer->second.shiftLines();
		}

		std::cout << "!! Looping alignment while..." << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;
	}

	std::cout << "!!! handleFinalObservation, while loop complete..." << std::endl;

	// before we insert the last triangle, we must make sure it is aligned to the rule of the triangle fan (should be done from the previous loop)
	lastContainer->insertWeldedTriangle(std::move(tracerContainer->second));
}