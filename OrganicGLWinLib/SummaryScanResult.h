#pragma once

#ifndef SUMMARYSCANRESULT_H
#define SUMMARYSCANRESULT_H

#include "FusedPointSummary.h"

class SummaryScanResult
{
	public:
		bool wasFound = false;
		int foundIndex = 0;
		glm::vec3 foundPoint;
		FusedPointSummary pointSummary = FusedPointSummary::UNDETERMINED;
};

#endif
