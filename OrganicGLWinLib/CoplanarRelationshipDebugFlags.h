#pragma once

#ifndef COPLANARRELATIONSHIPDEBUGFLAGS_H
#define COPLANARRELATIONSHIPDEBUGFLAGS_H

#include "DebugOption.h"
#include "PolyDebugLevel.h"

class CoplanarRelationshipDebugFlags
{
	public:
		CoplanarRelationshipDebugFlags() {};
	private:
		friend class CoplanarRelationships;
		friend class CoplanarRelationshipTracker;

		void handleSpecificTrackedSPolyDebugOption(DebugOption in_debugOption);

		PolyDebugLevel dlPrintBorderLinesOfTrackedAndRelatedSPolys = PolyDebugLevel::NONE;

};

#endif
