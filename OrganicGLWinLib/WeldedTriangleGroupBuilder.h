#pragma once

#ifndef WeldedTriangleGroupBuilder_H
#define WeldedTriangleGroupBuilder_H

#include "WeldedLinePool.h"
#include "TracingObserver.h"
#include "WeldedLinePoolGuide.h"
#include <vector>

class WeldedTriangleGroupBuilder
{
public:
	void setWeldedLinePool(WeldedLinePool in_weldedlinePool);
	void runTracingObservers();
	std::vector<WeldedTriangle> weldedTriangleVector;
private:
	WeldedLinePool linePool;
	TracingObserver tracer;
	int currentLineOfSightLineIndex = 0;		// default is at 0, because we don't know what the ending ID of the linePool is. (which should be set before the builder runs)
	void acquireWeldedLinesForWindowAndBuildObservation();
};

#endif