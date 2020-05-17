#pragma once

#ifndef BORDERLINEINTERSECTRECORDTYPE_H
#define BORDERLINEINTERSECTRECORDTYPE_H

enum class BorderLineIntersectRecordType
{
	NOVAL,					// for undefined values (default)
	OPEN,					// indicates that the CategorizedLine intercepts the border line one time.
	CLOSED_INWARD,			// indicates that the CategorizedLine intercepts the border line twice, and the empty normals are facing inward towards other categorized lines in the same sequence.
	CLOSED_OUTWARD			// indicates that the CategorizedLine intercepts the border line twice, and the empty normals are facing outward, away from other categorized lines in the same sequence.
};

#endif
