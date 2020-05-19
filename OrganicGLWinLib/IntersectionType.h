#pragma once

#ifndef INTERSECTIONTYPE_H
#define INTERSECTIONTYPE_H

enum class IntersectionType
{
	TWIN,				// potentially unsued.
	A_SLICE,			// polygon A has two of it's border lines go through polygon B; the result is that the CategorizedLine "slices" A.
	PARTIAL_BOUND,		// polygon A has one of its border lines go through B; this means that the CategorizedLine has one border line intercept that is a result of running a border line through Poly B.
	NON_BOUND,			// polygon A has none of its border lines go through B; but at least one of polygon B's border lines goes through A.
	NONE				// no intercept found.
};

#endif
