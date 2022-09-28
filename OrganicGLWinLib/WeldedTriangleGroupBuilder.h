#pragma once

#ifndef WeldedTriangleGroupBuilder_H
#define WeldedTriangleGroupBuilder_H

#include "WeldedLinePool.h"
#include "TracingObserver.h"
#include "WeldedLinePoolGuide.h"
#include "WeldedTriangleContainer.h"
#include <vector>
#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "ExceptionRecorder.h"
#include "BoundaryOrientation.h"

class WeldedTriangleGroupBuilder
{
public:
	WeldedTriangleGroupBuilder(PolyDebugLevel in_polyDebugLevel,
							   ExceptionRecorder* in_exceptionRecorderRef, 
							   BoundaryOrientation in_groupBuilderBoundaryOrientation) :
		weldedTriangleGroupBuilderLoggerLogLevel(in_polyDebugLevel),
		groupBuilderRecorderRef(in_exceptionRecorderRef),
		groupBuilderBoundaryOrientation(in_groupBuilderBoundaryOrientation)
	{
		weldedTriangleGroupBuilderLogger.setDebugLevel(in_polyDebugLevel);
	};
	void setWeldedLinePool(WeldedLinePool in_weldedlinePool);
	void runTracingObservers();
	//WeldedTriangleContainer weldedTriangleVector;
	std::vector<WeldedTriangleContainer> weldedTriangleContainerVector;
private:
	WeldedLinePool linePool;
	ExceptionRecorder* groupBuilderRecorderRef = nullptr;	// a pointer to an ExceptionRecorder; needs to be set by constructor.
															// Infinite while loops, and another anomalies should be recorded into this.
	TracingObserver tracer;
	BoundaryOrientation groupBuilderBoundaryOrientation = BoundaryOrientation::NONE;	// needs to be set by constructor
	int currentLineOfSightLineIndex = 0;		// default is at 0, because we don't know what the ending ID of the linePool is. (which should be set before the builder runs)
	void acquireWeldedLinesForWindowAndBuildObservation();
	void handleFinalObservation();
	PolyLogger weldedTriangleGroupBuilderLogger;
	PolyDebugLevel weldedTriangleGroupBuilderLoggerLogLevel = PolyDebugLevel::NONE;
};

#endif