#pragma once

#ifndef LINESEGMENTINTERSECTTYPE_H
#define LINESEGMENTINTERSECTTYPE_H

enum class LineSegmentIntersectType
{
	COLINEAR_OVERLAP,
	COLINEAR_NOOVERLAP,
	PARALLEL,
	NONCOLINEAR_INTERSECT,
	NO_INTERSECT,
	NOVAL
};

#endif