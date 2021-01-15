#pragma once

#ifndef FUSIONCANDIDATE_H
#define FUSIONCANDIDATE_H

#include "IntersectionResult.h"

class FusionCandidate
{
	public:
		bool wasCandidateBorderLineAndWithinPlane = false;
		IntersectionResult candidateIntersectionResult;
};

#endif
