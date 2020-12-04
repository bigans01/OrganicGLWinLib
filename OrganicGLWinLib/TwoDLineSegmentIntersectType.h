#pragma once

#ifndef TwoDLineSegmentINTERSECTTYPE_H
#define TwoDLineSegmentINTERSECTTYPE_H

enum class TwoDLineSegmentIntersectType
{
	COLINEAR_OVERLAP,
	COLINEAR_NOOVERLAP,
	PARALLEL,
	NONCOLINEAR_INTERSECT,
	NO_INTERSECT,
	NOVAL
};

#endif
