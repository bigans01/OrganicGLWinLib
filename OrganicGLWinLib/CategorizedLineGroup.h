#pragma once

#ifndef CATEGORIZEDLINEGROUP_H
#define CATEGORIZEDLINEGROUP_H

#include <vector>
#include "IntersectionType.h"
#include "CategorizedLineInsertionRecord.h"

class CategorizedLineGroup
{
	public:
		void insertRecord(IntersectionType in_intersectionType, int in_categorizedLineIndex)
		{
			CategorizedLineInsertionRecord newRecord(in_intersectionType, in_categorizedLineIndex);
			recordVector.push_back(newRecord);
		}
		std::vector<CategorizedLineInsertionRecord> recordVector;
};

#endif
