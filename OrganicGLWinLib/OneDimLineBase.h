#pragma once

#ifndef ONEDIMLINEBASE_H
#define ONEDIMLINEBASE_H

#include <glm/glm.hpp>

class OneDimLineBase
{
	public:
		void setOneDimLimit(float in_limit)
		{
			dimLimit = in_limit;
		}
		virtual bool isPointWithinLine(glm::vec3 in_pointToCompare) = 0;
		virtual void setStaticDims(float in_otherDimImmutableLocA, float in_otherDimImmutableLocB) = 0; 
		virtual void printMetaData() = 0;
	protected:
		float dimLimit = 0.0f;	// needs to be set by the call to setLimit
		float immutableDimA = 0.0f;	// the first of other dimensions
		float immutableDimB = 0.0f;	// the first of other dimensions
};

#endif
