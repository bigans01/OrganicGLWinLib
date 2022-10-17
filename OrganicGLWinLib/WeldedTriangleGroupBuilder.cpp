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
	tracer.setObservationLogLevel(weldedTriangleGroupBuilderLoggerLogLevel);
	currentLineOfSightLineIndex = linePool.getFirstElementID();	// this value should always be set to the initial key value of the first element in the linePool; the initial value should always be 0

	WeldedLinePoolGuide poolGuide(currentLineOfSightLineIndex, &linePool);	// testing only, remove when needed.
	//WeldedLinePoolGuide poolGuide(8, &linePool);	// testing only, remove when needed.
	//poolGuide.printGuideValues();

	// un-comment below block when ready to test.

	//std::cout << "getPoolSize, pre check: " << linePool.getPoolSize() << std::endl;
	weldedTriangleGroupBuilderLogger.log("(WeldedTriangleGroupBuilder) Size of line pool, prior to observation building loop: ", linePool.getPoolSize(), "\n");
	
	if (linePool.getPoolSize() > 3)
	{
		// The below 3 variables are a safety that prevents an infinite loop from occurring;
		// this is needed due to the fact that the call to TracingObserver::insertWeldedTriangleIfLineOfSightIsMaintained() doesn't always return true.
		// When this function returns false, it means that a line wasn't extracted from the linePool, and makes the 
		// currentObserverState of the tracer equal to TracingObserverState::TERMINATED.
		int terminationLimit = int(linePool.getPoolSize()) * 10;
		auto originalLinesVector = linePool.printLinesToStringVector();
		int currentTerminationCounter = 0;
		bool terminationExceeded = false;
		while 
		(
			(linePool.getPoolSize() > 3)		// when we remove the last of 4 lines from the pool, that's when we stop, as it means there are 3 lines left (only one triangle left to form)
			&&
			terminationExceeded == false
		)
		{
			//std::cout << "###~~~~~ Building new observation. " << std::endl;
			//std::cout << " ### Size of line pool is: " << linePool.getPoolSize() << std::endl;
			acquireWeldedLinesForWindowAndBuildObservation();

			if (tracer.getCurrentObserverState() == TracingObserverState::TERMINATED)
			{
				currentTerminationCounter++;
			}

			// if the terminationLimit is exceeded, throw a recorded exception.
			if (currentTerminationCounter > terminationLimit)
			{
				terminationExceeded = true;
				throwTerminationAttemptsExceeded(originalLinesVector);	// throw the appropriate exception
			}
		}

		//std::cout << "getPoolSize, post check: " << linePool.getPoolSize() << std::endl;
		weldedTriangleGroupBuilderLogger.log("(WeldedTriangleGroupBuilder) Size of line pool, after one observation loop tick: ", linePool.getPoolSize(), "\n");

		//std::cout << "getPoolSize, pre check 2" << std::endl;

		//acquireWeldedLinesForWindowAndBuildObservation();
		if (terminationExceeded == false)
		{
			handleFinalObservation();
		}
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

	//std::cout << "weldedTriangleContainerVector size is: " << weldedTriangleContainerVector.size() << std::endl;
	weldedTriangleGroupBuilderLogger.log("(WeldedTriangleGroupBuilder) weldedTriangleContainerVector size is: ", weldedTriangleContainerVector.size(), "\n");
	
	auto lastContainer = weldedTriangleContainerVector.rbegin();
	auto lastTriangleInLastContainer = lastContainer->triangleMap.rbegin();
	auto tracerContainer = tracer.currentContainer.triangleMap.begin();	// since there's only one triangle that will be produced, just get a pointer to it.

	// we will compare the points of the last line in the most recent triangle, to the first line in the newest triangle we are about to add.
	WeldedLine* mostRecentTriangleWeldedLineRef = lastTriangleInLastContainer->second.fetchTriangleLineRef(2);	
	WeldedLine* lastTriangleWeldedLineRef = tracerContainer->second.fetchTriangleLineRef(0);

	//std::cout << "*** Prior to welded line printout " << std::endl;

	//std::cout << "Most recent welded line, point A: " << mostRecentTriangleWeldedLineRef->pointA.x << ", " << mostRecentTriangleWeldedLineRef->pointA.y << ", " << mostRecentTriangleWeldedLineRef->pointA.z << std::endl;
	//std::cout << "Last triangle welded line, point A: " << lastTriangleWeldedLineRef->pointA.x << ", " << lastTriangleWeldedLineRef->pointA.y << ", " << lastTriangleWeldedLineRef->pointA.z << std::endl;
	weldedTriangleGroupBuilderLogger.log("(WeldedTriangleGroupBuilder) Most recent welded line, point A: ", mostRecentTriangleWeldedLineRef->pointA.x, ", ", mostRecentTriangleWeldedLineRef->pointA.y, ", ", mostRecentTriangleWeldedLineRef->pointA.z, "\n");
	weldedTriangleGroupBuilderLogger.log("(WeldedTriangleGroupBuilder) Last triangle welded line, point A: ", lastTriangleWeldedLineRef->pointA.x, ", ", lastTriangleWeldedLineRef->pointA.y, ", ", lastTriangleWeldedLineRef->pointA.z, "\n");

	bool isAligned = false;
	bool wereShiftsExceeded = false;
	int shiftCount = 0;
	while 
	(
		(isAligned == false)
		&&
		(wereShiftsExceeded == false)
	)
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
			shiftCount++;
			if (shiftCount > 3)
			{
				wereShiftsExceeded = true;
				// --Record exception, EXCEPTION_WELDED_TRIANGLE_SHIFT_LINES_EXCEEDED here.
				//std::cout << "(WeldedTriangleGroupBuilder::handleFinalObservation): NOTICE: shifts of tracerContainer exceeded; handling required. " << std::endl;
				Message weldingContextMessage;	// no explicit message type required here, for this kind of exception.
				std::string getOrientationString = IndependentUtils::getBoundaryOrientationString(groupBuilderBoundaryOrientation);
				std::string firstContextLine = "\tAn ExceptionRecordType::EXCEPTION_WELDED_TRIANGLE_SHIFT_LINES_EXCEEDED was detected during call to WeldedTriangleGroupBuilder::handleFinalObservation(), on the "
					+ getOrientationString + " face. ";
				std::string secondContextLine = "\t\tThis may trigger an SPolyResolution to be called on this same face.";
				weldingContextMessage.insertString(firstContextLine);
				weldingContextMessage.insertString(secondContextLine);

				ExceptionRecord exceededShiftRecord(ExceptionRecordType::EXCEPTION_WELDED_TRIANGLE_SHIFT_LINES_EXCEEDED, weldingContextMessage);
				groupBuilderRecorderRef->insertException(exceededShiftRecord);
				//int shifts = 3;
				//std::cin >> shifts;
			}
		}

		//std::cout << "!! Looping alignment while..." << std::endl;
		weldedTriangleGroupBuilderLogger.log("(WeldedTriangleGroupBuilder) !! Looping alignment while...", "\n");
		if (weldedTriangleGroupBuilderLogger.isLoggingSet())
		{
			int someVal = 3;
			std::cin >> someVal;
		}
	}

	//std::cout << "!!! handleFinalObservation, while loop complete..." << std::endl;
	weldedTriangleGroupBuilderLogger.log("(WeldedTriangleGroupBuilder) !!! handleFinalObservation, while loop complete...", "\n");

	// before we insert the last triangle, we must make sure it is aligned to the rule of the triangle fan (should be done from the previous loop)
	if (wereShiftsExceeded == false)
	{
		lastContainer->insertWeldedTriangle(std::move(tracerContainer->second));
	}
}

void WeldedTriangleGroupBuilder::throwTerminationAttemptsExceeded(std::vector<std::string> in_originalLines)
{
	Message excessiveTerminationsMessage;

	// Part 1: set up the context strings.
	std::string getOrientationString = IndependentUtils::getBoundaryOrientationString(groupBuilderBoundaryOrientation);
	std::string firstContextLine("\tAn ExceptionRecordType::EXCEPTION_TERMINATION_ATTEMPTS_EXCEEDED was detected during call to WeldedTriangleGroupBuilder::runTracingObservers() , on the "
		+ getOrientationString + " face.");
	excessiveTerminationsMessage.insertString(firstContextLine);
	std::string secondContextline("\tThe final size of the weldedTriangleContainerVector was: " + std::to_string(weldedTriangleContainerVector.size()) + "; now printing original lines.");
	excessiveTerminationsMessage.insertString(secondContextline);


	// Part 2: Write the original lines; prefix with a context string.
	excessiveTerminationsMessage.insertString("\t\tOriginal lines, prior to observations: ");
	excessiveTerminationsMessage.insertInt(int(in_originalLines.size()));
	for (auto& currentStringedLine : in_originalLines)
	{
		excessiveTerminationsMessage.insertString(std::string("\t\t" + currentStringedLine));
	}

	// Part 3: Write the final lines; prefix with a context string.
	excessiveTerminationsMessage.insertString("\t\tRemaining lines, after observations: ");
	excessiveTerminationsMessage.insertInt(int(linePool.getPoolSize()));
	auto remainingLines = linePool.printLinesToStringVector();
	for (auto& currentRemainingLine : remainingLines)
	{
		excessiveTerminationsMessage.insertString(std::string("\t\t" + currentRemainingLine));
	}

	ExceptionRecord exceededShiftRecord(ExceptionRecordType::EXCEPTION_TERMINATION_ATTEMPTS_EXCEEDED, excessiveTerminationsMessage);
	groupBuilderRecorderRef->insertException(exceededShiftRecord);

	// because the group building was bad, we must clear out the value of weldedTriangleContainerVector, as this value must be 0
	// if we wish to run handling in MassZoneBoxBoundarySPolySet::buildBoundarySPolyFromFactory.
	weldedTriangleContainerVector.clear();
}