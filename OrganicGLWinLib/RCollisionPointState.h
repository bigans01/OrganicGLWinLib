#pragma once

#ifndef RCOLLISIONPOINTSTATE_H
#define RCOLLISIONPOINTSTATE_H

enum class RCollisionPointState
{
	FREE,		// default value for a point
	IMMUTABLE,	// surrounded on 8 sides
	MOVED		// was once free, but has been modified
};

#endif
