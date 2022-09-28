#pragma once

#ifndef CLEAVESEQUENCEINTERSECTFINDER_H
#define CLEAVESEQUENCEINTERSECTFINDER_H

#include "SPoly.h"
#include "BorderLineCycler.h"
#include "CleaveSequence.h"
#include <chrono>
#include "LineWelder.h"
#include "WeldedLinePool.h"
#include "WeldedTriangleGroupBuilder.h"
#include <vector>
#include "WeldedTriangleSuperGroup.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "ExceptionRecorder.h"
#include "BoundaryOrientation.h"

class CleaveSequenceIntersectFinder
{
public:
	// Value for ExceptionRecorder (ref) and BoundaryOrientation must be given below, 
	// for operations such as constructing an instance of WeldedTriangleGroupBuilder (it uses these for recording exceptions)
	CleaveSequenceIntersectFinder(int in_originalPolyID, 
								SPoly* in_sPolyRef, 
								PolyDebugLevel in_polyDebugLevel,
								ExceptionRecorder* in_exceptionRecorderRef, 
								BoundaryOrientation in_intersectBoundaryOrientation);
	int originalPolyID = 0;
	SPoly* sPolyRef;
	void loadInterceptRecords();		// read all CleaveSequences, put the metadata about the borderlines they intercept into the borderlines.
	void printCurrentLineValuesInCleaveSequences();
	WeldedLinePool linePool;
	WeldedTriangleSuperGroup triangleSupergroup;
	bool wasWeldingSuccessful = true;	// can be set to false if the LineWelder fails doing its thing
private:
	PolyLogger intersectFinderLogger;
	ExceptionRecorder* sequenceRecorderRef = nullptr;	// set by constructor; used by instances of WeldedTriangleGroupBuilder
	BoundaryOrientation intersectBoundaryOrientation = BoundaryOrientation::NONE;	// also used by instances of WeldedTriangleGroupBuilder
	PolyDebugLevel intersectFinderLoggerDebugLevel = PolyDebugLevel::NONE;
};

#endif