#pragma once

#ifndef CLEAVESEQUENCEINTERSECTFINDER_H
#define CLEAVESEQUENCEINTERSECTFINDER_H

#include "SPoly.h"
#include "BorderLineCycler.h"
#include "CleaveSequence.h"
#include <chrono>
#include "LineWelder.h"
#include "WeldedLinePool.h"
#include "WeldedTriangleBuilder.h"

class CleaveSequenceIntersectFinder
{
public:
	CleaveSequenceIntersectFinder(SPoly* in_sPolyRef);
	SPoly* sPolyRef;
	void loadInterceptRecords();		// read all CleaveSequences, put the metadata about the borderlines they intercept into the borderlines.
	WeldedLinePool linePool;
};

#endif