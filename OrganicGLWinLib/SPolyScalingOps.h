#pragma once

#ifndef SPOLYSCALINGOPS_H
#define SPOLYSCALINGOPS_H

#include <glm/glm.hpp>
#include <stack>
#include "QuatRotationRecord.h"

class SPolyScalingOps
{
	public:
		friend class SPolyScaler;

		glm::vec3 translationVector;										// OP 1: the very first operation that happens: the "center" of the SPoly, 
																			//		 determined by SPolyScaler, which all points in the SPoly need to be adjusted by.
		std::stack<QuatRotationRecord> rotationRecords;						// OP 2: any records that had to be applied to the SPoly, to get it on the XZ plane (y = 0)
		float sPolyScalarAdjustment = 0.0f;									// OP 3: the scalar value that is applied to all points of the SPoly, to have it form the "best fit" in the
																			//       scaled up version of the container (that is, the 32 x 32 x 32 area upscaled by 1.41421f); 
																			// 

		void setOP1Value(glm::vec3 in_vector);
};

#endif
