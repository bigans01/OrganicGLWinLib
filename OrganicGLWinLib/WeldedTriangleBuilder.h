#pragma once

#ifndef WELDEDTRIANGLEBUILDER_H
#define WELDEDTRIANGLEBUILDER_H

#include "WeldedLinePool.h"
#include "TracingObserver.h"

class WeldedTriangleBuilder
{
public:
	void setWeldedLinePool(WeldedLinePool in_weldedlinePool);
	void runTracingObservers();
private:
	WeldedLinePool linePool;
	TracingObserver tracer;
	int currentObservationEndLineIndex = 0;
	void incrementCurrentObservationEndLineIndex();
	void acquireWeldedLinesForWindowAndBuildObservation();
};

#endif