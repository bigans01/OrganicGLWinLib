#include "stdafx.h"
#include "CoplanarRelationshipDebugFlags.h"

void CoplanarRelationshipDebugFlags::handleSpecificTrackedSPolyDebugOption(DebugOption in_debugOption)
{
	if (in_debugOption == DebugOption::COPLANARRELATIONSHIP_SHOW_TRACKED_SPOLY_BORDER_LINES)
	{
		dlPrintBorderLinesOfTrackedAndRelatedSPolys = PolyDebugLevel::DEBUG;
	}
}

void CoplanarRelationshipDebugFlags::handleSpecificTrackedSPolySTriangleDebugOption(int in_sTriangleID, DebugOption in_debugOption)
{
	specificTrackedSTriangleDOSMap[in_sTriangleID] += in_debugOption;
}

void CoplanarRelationshipDebugFlags::handleSpecificTrackedSPolyCutterCuttingTriangleDebugOption(int in_cuttingTriangleID, DebugOption in_debugOption)
{
	specificTrackedCutterCuttingTriangleDOSMap[in_cuttingTriangleID] += in_debugOption;
}