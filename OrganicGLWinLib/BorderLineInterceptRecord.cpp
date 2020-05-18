#include "stdafx.h"
#include "BorderLineIntersectRecord.h"

void BorderLineIntersectRecord::insertCategorizedLineRef(int in_categoriedLineID, CategorizedLine* in_categorizedLineRef)
{
	lineRefs[in_categoriedLineID] = in_categorizedLineRef;
	numberOfCatLines++;
}

void BorderLineIntersectRecord::determineRecordType()
{
	if (numberOfCatLines == 1)
	{
		recordType = BorderLineIntersectRecordType::OPEN;
	}
	else if (numberOfCatLines == 2)
	{
		auto lineRefsBegin = lineRefs.begin();		// get the first categorized line
		auto lineRefsEnd = lineRefs.rbegin();		// get the last categorized line

		glm::vec3 pointA = lineRefsBegin->second->line.getBorderPointFromSingularBorderLineCount();	// get the first categorized line's intercepting border point.
		glm::vec3 pointB = lineRefsEnd->second->line.getBorderPointFromSingularBorderLineCount();	// get the first categorized line's intercepting border point.

		// do function call here
	}
}