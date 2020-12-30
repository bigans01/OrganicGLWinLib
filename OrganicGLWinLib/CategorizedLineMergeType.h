#pragma once

#ifndef CATEGORIZEDLINEMERGETYPE_H
#define CATEGORIZEDLINEMERGETYPE_H

enum class CategorizedLineMergeType
{
	MERGE_TO_PARTIAL_BOUND,
	MERGE_TO_A_SLICE,
	MERGE_TO_INTERCEPTS_POINT_PRECISE,
	NO_MERGE_REQUIRED,			// used when there's just 1 line
	NOVAL
};

#endif
