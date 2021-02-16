#pragma once

#ifndef CUTTRIANGLEGROUPBUILDER_H
#define CUTTRIANGLEGROUPBUILDER_H

#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "CutLinePool.h"
#include <vector>
#include "CutTriangleContainer.h"
#include "CutLineTraceObserver.h"

class CutTriangleGroupBuilder
{
	public:
		CutTriangleGroupBuilder(PolyDebugLevel in_polyDebugLevel, CutLinePool in_cutLinePool) :
			cutTriangleGroupBuilderLoggerLogLevel(in_polyDebugLevel),
			linePool(in_cutLinePool)
		{
			cutTriangleGroupBuilderLogger.setDebugLevel(in_polyDebugLevel);
		};
		void runCutTraceObserver();
		std::vector<CutTriangleContainer> cutTriangleContainerVector;
	private:
		CutLinePool linePool;
		CutLineTraceObserver tracer;
		int currentLineOfSightLineIndex = 0;		// default is at 0, because we don't know what the ending ID of the linePool is. (which should be set before the builder runs)
		void acquireWeldedLinesForWindowAndBuildObservation();
		void handleFinalObservation();
		PolyLogger cutTriangleGroupBuilderLogger;
		PolyDebugLevel cutTriangleGroupBuilderLoggerLogLevel = PolyDebugLevel::NONE;
};

#endif
