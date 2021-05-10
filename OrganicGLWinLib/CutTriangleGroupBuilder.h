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
	private:
		CutTriangleGroupBuilder(PolyDebugLevel in_polyDebugLevel, CutLinePool in_cutLinePool) :
			cutTriangleGroupBuilderLoggerLogLevel(in_polyDebugLevel),
			linePool(in_cutLinePool)
		{
			cutTriangleGroupBuilderLogger.setDebugLevel(in_polyDebugLevel);	// should be set by REFERENCED_CUTTINGTRIANGLE_CUTTRIANGLEGROUPBUILDER
		};
		friend class CuttableTriangle;				// should only need to be used by this class.

		std::vector<CutTriangleContainer> cutTriangleContainerVector;
		CutLinePool linePool;
		CutLineTraceObserver tracer;
		int currentLineOfSightLineIndex = 0;		// default is at 0, because we don't know what the ending ID of the linePool is. (which should be set before the builder runs)
		PolyLogger cutTriangleGroupBuilderLogger;
		PolyDebugLevel cutTriangleGroupBuilderLoggerLogLevel = PolyDebugLevel::NONE;
		bool wasRunComplete = true;
		
		void acquireWeldedLinesForWindowAndBuildObservation();
		void handleFinalObservation();
		void runCutTraceObserver();
		std::vector<CutTriangle> produceAndReturnCutTriangleVector();
	
};

#endif
