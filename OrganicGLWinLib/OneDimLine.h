#pragma once

#ifndef ONEDIMLINE_H
#define ONEDIMLINE_H

#include <glm/glm.hpp>

class OneDimLine
{
	public:
		virtual bool isPointWithinLine(glm::vec3 in_pointToCompare) = 0;
		virtual void setup(float in_otherDimImmutableLocA, float in_otherDimImmutableLocB, float in_dimLimit) = 0; 
	private:
		float dimLimit = 0.0f;	// needs to be set by the call to setLimit
		float immutableDimA = 0.0f;	// the first of other dimensions
		float immutableDimB = 0.0f;	// the first of other dimensions
};

#endif
