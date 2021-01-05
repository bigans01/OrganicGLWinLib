#pragma once

#ifndef MASSZONEBOXBOUNDARYSTATE_H
#define MASSZONEBOXBOUNDARYSTATE_H

enum class MassZoneBoxBoundaryState
{
	INACTIVE,					// it the boundary isn't used at all
	ACTIVE_COVERS_ENTIRE_AREA,	// the boundary is used, but all of the registered points are all corner points; because of this, the boundary would still cover its entire area as long as all points registered so far are corner points (up to 4)
	ACTIVE_MODIFIED				// the boundary is used, but at least one of the points isn't a corner point; this means that it can't possibly maintain it's original state
};

#endif
