#pragma once

#ifndef INTERSECTIONTYPE_H
#define INTERSECTIONTYPE_H

enum class IntersectionType
{
	TWIN,				// potentially unsued.
	A_SLICE,			// polygon A has two of it's border lines go through polygon B; the result is that the CategorizedLine "slices" A.
	A_SLICE_SEGMENT_ENDPOINT, // the resulting line is actually a single point, since the endpoints of the lines match, and there was a condition match when determinining the line;
						// in this case, there should be exactly 1 other A_SLICE_SEGMENT_ENDPOINT, from the guest SPoly that registers its lines to the host (for a total of 2)
	PARTIAL_BOUND,		// polygon A has one of its border lines go through B; this means that the CategorizedLine has one border line intercept that is a result of running a border line through Poly B.
	NON_BOUND,			// polygon A has none of its border lines go through B; but at least one of polygon B's border lines goes through A.
	INTERCEPTS_POINT_PRECISE,	// polygon A has two of it's border lines go through (not parallel) to the SAME line in polygon B; in othe words, a point belonging to 
								// polygon A that is shared between two border lines is on a single border line in poly B.
	A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE, // (Still needs logic -- this is unused/untested -- but will be needed), as of 2/9/2021) exists in a situation where there is an intercepts point precise, but the other point is also a border line point.
	INTERCEPTS_POINT_PRECISE_UNSOLVED_COPLANAR,		// exists in the situation where a Coplanar comparison has a point that sits between a border line and non-border line.
	NONE				// no intercept found.
};

#endif
