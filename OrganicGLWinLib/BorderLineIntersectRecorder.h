#pragma once

#ifndef BORDERLINEINTERSECTRECORDER_H
#define BORDERLINEINTERSECTRECORDER_H

#include <map>
#include "BorderLineIntersectRecord.h"
#include "CategorizedLine.h"

class BorderLineIntersectRecorder
{
	public:
		std::map<int, BorderLineIntersectRecord> records;		// an ordered map of records.  the "int" represents the ID from the CleaveSequence map in the SPoly being fractured.
		void insertNewRecord(int in_sequenceID, int in_categorizedLineID, CategorizedLine* in_categorizedLineRef);
		void printRecords();
};

#endif
