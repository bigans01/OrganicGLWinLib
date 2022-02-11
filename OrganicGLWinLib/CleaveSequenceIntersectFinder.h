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

class CleaveSequenceIntersectFinder
{
public:
	CleaveSequenceIntersectFinder(int in_originalPolyID, SPoly* in_sPolyRef, PolyDebugLevel in_polyDebugLevel);
	int originalPolyID = 0;
	SPoly* sPolyRef;
	void loadInterceptRecords();		// read all CleaveSequences, put the metadata about the borderlines they intercept into the borderlines.
	void printCurrentLineValuesInCleaveSequences();
	WeldedLinePool linePool;
	WeldedTriangleSuperGroup triangleSupergroup;
	bool wasWeldingSuccessful = true;	// can be set to false if the LineWelder fails doing its thing
private:
	PolyLogger intersectFinderLogger;
	PolyDebugLevel intersectFinderLoggerDebugLevel = PolyDebugLevel::NONE;
};

#endif