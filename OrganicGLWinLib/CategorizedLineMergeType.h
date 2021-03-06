#pragma once

#ifndef CATEGORIZEDLINEMERGETYPE_H
#define CATEGORIZEDLINEMERGETYPE_H

enum class CategorizedLineMergeType
{
	MERGE_TO_PARTIAL_BOUND,
	MERGE_TO_A_SLICE,
	MERGE_TO_INTERCEPTS_POINT_PRECISE,
	MERGE_TO_A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE,	// used when there is 1 single intercepts point precise, and 1 partial bound.
	MERGE_TO_NON_BOUND,			// used when there is > 1 non-bound
	NO_MERGE_REQUIRED,			// used when there's just 1 line
	NOVAL
};

#endif
