#pragma once

#ifndef FUSEDINTERSECTIONLINE_H
#define FUSEDINTERSECTIONLINE_H

#include "IntersectionLine.h"
#include "FusionAnalysis.h"

class FusedIntersectionLine
{
	public:
		IntersectionLine deprecatedLine;
		FusionAnalysis completedAnalysis;
};

#endif
