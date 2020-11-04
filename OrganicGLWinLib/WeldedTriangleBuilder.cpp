#include "stdafx.h"
#include "WeldedTriangleBuilder.h"

void WeldedTriangleBuilder::setWeldedLinePool(WeldedLinePool in_weldedlinePool)
{
	linePool = in_weldedlinePool;
}

void WeldedTriangleBuilder::runTracingObservers()
{
	tracer.setWeldedLinePoolRef(&linePool);
	while (linePool.getPoolSize() > 3)		// when we remove the last of 4 lines from the pool, that's when we stop, as it means there are 3 lines left (only one triangle left to form)
	{
		acquireWeldedLinesForWindowAndBuildObservation();
	}
}

void WeldedTriangleBuilder::incrementCurrentObservationEndLineIndex()
{
	currentObservationEndLineIndex++;
}

void WeldedTriangleBuilder::acquireWeldedLinesForWindowAndBuildObservation()
{
	//auto windowObservationEndLine = linePool.fetchLastLineInPool();
	//tracer.buildNewObservation...
}