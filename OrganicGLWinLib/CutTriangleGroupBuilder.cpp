#include "stdafx.h"
#include "CutTriangleGroupBuilder.h"

void CutTriangleGroupBuilder::runCutTraceObserver()
{
	tracer.setCutLinePoolRef(&linePool);
	tracer.setCutTriangleContainerVectorRef(&cutTriangleContainerVector);
	tracer.setObservationLogLevel(cutTriangleGroupBuilderLoggerLogLevel);
	currentLineOfSightLineIndex = linePool.getFirstElementID();	// this value should always be set to the initial key value of the first element in the linePool; the initial value should always be 0

	CutLinePoolGuide poolGuide(currentLineOfSightLineIndex, &linePool);	// testing only, remove when needed.

	//std::cout << ":::::::::::::::::::: Printing out contents of line pool. :::::::::::::::::::::............" << std::endl;
	//linePool.printLines();
	cutTriangleGroupBuilderLogger.log("(CutTriangleGroupBuilder) :::::::::::::::::::: Printing out contents of line pool. :::::::::::::::::::::............", "\n");
	if (cutTriangleGroupBuilderLogger.isLoggingSet())
	{
		linePool.printLines();
	}
	
	//std::cout << ":::|||||| Size of vector, before anything: " << cutTriangleContainerVector.size() << std::endl;
	if (linePool.getPoolSize() > 3)
	{
		while (linePool.getPoolSize() > 3)		// when we remove the last of 4 lines from the pool, that's when we stop, as it means there are 3 lines left (only one triangle left to form)
		{
			//std::cout << "###~~~~~ Building new observation. " << std::endl;
			//std::cout << " ### Size of line pool is: " << linePool.getPoolSize() << std::endl;
			int preObservationPoolSize = linePool.getPoolSize();
			acquireWeldedLinesForWindowAndBuildObservation();
			int postObservationPoolSize = linePool.getPoolSize();
			//std::cout << ":::|||||| Size of vector, after while tick: " << cutTriangleContainerVector.size() << std::endl;
			cutTriangleGroupBuilderLogger.log("(CutTriangleGroupBuilder) :::|||||| Size of vector, after while tick: ", cutTriangleContainerVector.size(), "\n");
			if (preObservationPoolSize == postObservationPoolSize)
			{
				std::cout << "~~~~~!!!!!!! Warning, pre and post match...continue? " << std::endl;
				//cutTriangleGroupBuilderLogger.log("(CutTriangleGroupBuilder) ~~~~~!!!!!!! Warning, pre and post match...continue?  ", "\n");
				int prepostMatch = 3;
				std::cout << "~~~~~Number of lines in pool is: " << preObservationPoolSize << std::endl;
				while (prepostMatch == 3)
				{

				}
			}
		}

		//std::cout << "getPoolSize, post check: " << linePool.getPoolSize() << std::endl;
		//weldedTriangleGroupBuilderLogger.log("(WeldedTriangleGroupBuilder) Size of line pool, after one observation loop tick: ", linePool.getPoolSize(), "\n");

		//std::cout << "getPoolSize, pre check 2" << std::endl;

		//acquireWeldedLinesForWindowAndBuildObservation();
		handleFinalObservation();
	}
	else if (linePool.getPoolSize() == 3)
	{
		CutTriangle newTriangle(linePool.fetchLineFromPoolViaIndex(0), linePool.fetchLineFromPoolViaIndex(1), linePool.fetchLineFromPoolViaIndex(2));
		tracer.currentContainer.insertWeldedTriangle(newTriangle);
		cutTriangleContainerVector.push_back(tracer.currentContainer);
		linePool.clearPool();
	}
	//std::cout << "getPoolSize, post check" << std::endl;

	//std::cout << "Estimated number of triangles that will be produced is: " << weldedTriangleContainerVector.triangleMap.size() << std::endl;

	//int someVal = 5;
	//std::cin >> someVal;

	int totalContainers = 0;
	int totalTriangles = 0;
	auto vectorBegin = cutTriangleContainerVector.begin();
	auto vectorEnd = cutTriangleContainerVector.end();
	for (; vectorBegin != vectorEnd; vectorBegin++)
	{
		totalContainers++;
		auto trianglesBegin = vectorBegin->cutTrianglesMap.begin();
		auto trianglesEnd = vectorBegin->cutTrianglesMap.end();
		for (; trianglesBegin != trianglesEnd; trianglesBegin++)
		{
			totalTriangles++;
		}
	}


	//std::cout << "#### Total containers: " << totalContainers << std::endl;
	//std::cout << "#### Total triangles: " << totalTriangles << std::endl;
	//int totalVal = 3;
	//std::cin >> totalVal;

	cutTriangleGroupBuilderLogger.log("(CutTriangleGroupBuilder): #### Total containers produced by tracer: ", totalContainers, "\n");
	cutTriangleGroupBuilderLogger.log("(CutTriangleGroupBuilder): #### Total triangles produced by tracer: ", totalTriangles, "\n");
	cutTriangleGroupBuilderLogger.waitForDebugInput();
}

void CutTriangleGroupBuilder::acquireWeldedLinesForWindowAndBuildObservation()
{
	//auto windowObservationEndLine = linePool.fetchLastLineInPool();
	CutLinePoolGuide poolGuide(currentLineOfSightLineIndex, &linePool);
	//tracer.buildNewObservation(linePool.fetchLineFromPoolViaIndex(poolGuide.lineOfSightLineIndex), linePool.fetchLineFromPoolViaIndex(poolGuide.observationEndLineIndex));
	tracer.buildNewCutObservation(poolGuide);		// build it, let it run
	cutTriangleContainerVector.push_back(tracer.currentContainer);
	if (tracer.getCurrentObserverState() == TracingObserverState::TERMINATED)	// if it was prematurely terminated, we must increment the currentLineOfSightIndex
																				// before creating the new poolGuide
	{
		currentLineOfSightLineIndex = linePool.fetchFollowingIndex(currentLineOfSightLineIndex);
	}

	// if the result of the observation is a termination, fetch whatever was produced, and increment the currentObservationLineIndex by one.
}

void CutTriangleGroupBuilder::handleFinalObservation()
{
	CutLinePoolGuide poolGuide(currentLineOfSightLineIndex, &linePool);
	tracer.buildNewCutObservation(poolGuide);		// build it, let it run

	auto lastContainer = cutTriangleContainerVector.rbegin();
	auto lastTriangleInLastContainer = lastContainer->cutTrianglesMap.rbegin();
	auto tracerContainer = tracer.currentContainer.cutTrianglesMap.begin();	// since there's only one triangle that will be produced, just get a pointer to it.

	// we will compare the points of the last line in the most recent triangle, to the first line in the newest triangle we are about to add.
	CutLine* mostRecentTriangleWeldedLineRef = lastTriangleInLastContainer->second.fetchTriangleLineRef(2);
	CutLine* lastTriangleWeldedLineRef = tracerContainer->second.fetchTriangleLineRef(0);

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

		//std::cout << "!! Looping alignment while..." << std::endl;
		//weldedTriangleGroupBuilderLogger.log("(WeldedTriangleGroupBuilder) !! Looping alignment while...", "\n");
		//int someVal = 3;
		//std::cin >> someVal;

		
	}

	//std::cout << "+++++++++++ Final observed triangle points are: " << std::endl;
	//tracerContainer->second.printPoints();
	//int observeHalt = 3;
	//std::cin >> observeHalt;

	lastContainer->insertWeldedTriangle(std::move(tracerContainer->second));
}

std::vector<CutTriangle> CutTriangleGroupBuilder::produceAndReturnCutTriangleVector()
{
	std::vector<CutTriangle> returnVector;
	auto containerVectorBegin = cutTriangleContainerVector.begin();
	auto containerVectorEnd = cutTriangleContainerVector.end();
	for (; containerVectorBegin != containerVectorEnd; containerVectorBegin++)
	{
		auto currentCutTriangleBegin = containerVectorBegin->cutTrianglesMap.begin();
		auto currentCutTriangleEnd = containerVectorBegin->cutTrianglesMap.end();
		for (; currentCutTriangleBegin != currentCutTriangleEnd; currentCutTriangleBegin++)
		{
			returnVector.push_back(currentCutTriangleBegin->second);
		}
	}
	return returnVector;
}