#pragma once

#ifndef BORDERLINEINTERSECTRECORD_H
#define BORDERLINEINTERSECTRECORD_H

#include "CategorizedLine.h"
#include "BorderLineIntersectRecordType.h"
#include <map>

class BorderLineIntersectRecord
{
	public:
		void insertCategorizedLineRef(int in_categoriedLineID, CategorizedLine* in_categorizedLineRef);
		void determineRecordType();

		int numberOfCatLines = 0;	// the number of cat lines from this CleaveSequence that are registered on the border line. Incremented every time a new categorized line ref is added.
		BorderLineIntersectRecordType recordType = BorderLineIntersectRecordType::NOVAL;	// what type of intersect is this?
		std::map<int, CategorizedLine*> lineRefs;		// the index of the referenced categorized line in the CleaveSequence, and the ref to the categorized line itself.
};

#endif