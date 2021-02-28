#pragma once

#ifndef COPLANARRELATIONSHIPDEBUGFLAGS_H
#define COPLANARRELATIONSHIPDEBUGFLAGS_H

#include "DebugOption.h"

class CoplanarRelationshipDebugFlags
{
	public:
		CoplanarRelationshipDebugFlags() {};
	private:
		friend class CoplanarRelationship;
		friend class CoplanarRelationshipTracker;

		void handleSpecificTrackedSPolyDebugOption(DebugOption in_debugOption);

};

#endif
