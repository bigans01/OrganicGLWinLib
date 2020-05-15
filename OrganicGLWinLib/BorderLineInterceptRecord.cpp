#include "stdafx.h"
#include "BorderLineIntersectRecord.h"

void BorderLineIntersectRecord::insertCategorizedLineRef(int in_categoriedLineID, CategorizedLine* in_categorizedLineRef)
{
	lineRefs[in_categoriedLineID] = in_categorizedLineRef;
	numberOfCatLines++;
}