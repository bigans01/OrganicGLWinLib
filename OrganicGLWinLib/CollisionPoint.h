#pragma once

#ifndef COLLISIONPOINT_H
#define COLLISIONPOINT_H

#include <glm/glm.hpp>
#include "ECBPPOrientations.h"

class CollisionPoint
{
	public:
		CollisionPoint() {};
		CollisionPoint(ECBPPOrientations in_originalGridOrientation, glm::vec3 in_originalPoint) :
			originalGridOrientation(in_originalGridOrientation),
			originalValue(in_originalPoint),
			currentValue(in_originalPoint)
		{};

		glm::vec3 originalValue;	// will always stay the same after it has been set
		glm::vec3 currentValue;		// can be manipulated by any MorphableMesh that has this as a point
	private:
		bool massHit = false;		// would determine if the point collided with some form of matter during a trace by a MorphableMesh
		ECBPPOrientations originalGridOrientation; // the point's original orientation, in relation to the grid it belongs in.
};

#endif
