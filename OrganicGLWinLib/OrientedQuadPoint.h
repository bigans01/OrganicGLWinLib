#pragma once

#ifndef ORIENTEDQUADPOINT_H
#define ORIENTEDQUADPOINT_H

#include "QuadPointOrientation.h"

class OrientedQuadPoint
{
	public:
		QuadPointOrientation orientation = QuadPointOrientation::NONE; // the default is "NONE" but this will change.
		glm::vec3 worldSpaceCoord;									   // the coordinate in world space
};

#endif
