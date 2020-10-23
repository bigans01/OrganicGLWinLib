#pragma once

#ifndef FOUNDCLEAVESEQUENCE_H
#define FOUNDCLEAVESEQUENCE_H

#include <glm/glm.hpp>
#include "DistanceToPoint.h"

class FoundCleaveSequence
{
	public:

		FoundCleaveSequence() {};
		FoundCleaveSequence(int in_cleaveSequenceID, DistanceToPoint in_distanceToPoint) :
			cleaveSequenceID(in_cleaveSequenceID),
			distance(in_distanceToPoint.distance),
			cleaveSequenceTracingBeginPoint(in_distanceToPoint.point)
		{};

		int cleaveSequenceID;
		float distance;
		glm::vec3 cleaveSequenceTracingBeginPoint;
};

#endif
