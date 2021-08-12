#pragma once

#ifndef RCOLLISIONPOINTSTATE_H
#define RCOLLISIONPOINTSTATE_H

enum class RCollisionPointState
{
	FREE,		// default value for a point
	WITHIN_MASS,	// the point is found as being within some mass
	IMMUTABLE,	// surrounded on 8 sides
	MOVED		// was once free, but has been modified
};

#endif
