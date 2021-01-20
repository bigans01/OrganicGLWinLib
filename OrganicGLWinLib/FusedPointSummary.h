#pragma once

#ifndef FUSEDPOINTSUMMARY_H
#define FUSEDPOINTSUMMARY_H

enum class FusedPointSummary
{
	PRECISE_BORDERLINE,		// there's at least 1 border line involved in the point
	PRECISE_NONBORDERLINE,	// there are no border lines involved with the point
	TYPICAL_BORDERLINE,		// there is just 1 border line involved with the point
	TYPICAL_NONBORDERLINE,	// there is just 1 NONBORDERLINE involved with the point
	UNDETERMINED
};

#endif
