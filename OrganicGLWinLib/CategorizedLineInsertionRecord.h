#pragma once

#ifndef CATEGORIZEDLINEINSERTIONRECORD_H
#define CATEGORIZEDLINEINSERTIONRECORD_H

#include "IntersectionType.h"

class CategorizedLineInsertionRecord
{
	public:
		CategorizedLineInsertionRecord() {};
		CategorizedLineInsertionRecord(IntersectionType in_intersectionType, int in_categorizedLineIndex) :
			categorizedLineIntersectionType(in_intersectionType),
			categorizedLineIndex(in_categorizedLineIndex)
		{};
		IntersectionType categorizedLineIntersectionType = IntersectionType::NONE;		// the intersection type.
		int categorizedLineIndex = 0;															// the index of the categorized line, in the appropriate pool of lines it exists in
};

#endif
