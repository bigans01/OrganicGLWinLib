#include "stdafx.h"
#include "CoplanarRelationshipDebugFlags.h"

void CoplanarRelationshipDebugFlags::handleSpecificTrackedSPolyDebugOption(DebugOption in_debugOption)
{
	if (in_debugOption == DebugOption::COPLANARRELATIONSHIP_SHOW_TRACKED_SPOLY_BORDER_LINES)
	{
		dlPrintBorderLinesOfTrackedAndRelatedSPolys = PolyDebugLevel::DEBUG;
	}
}