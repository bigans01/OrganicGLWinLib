#include "stdafx.h"
#include "BorderLineIntersectRecorder.h"

void BorderLineIntersectRecorder::insertNewRecord(int in_sequenceID, int in_categorizedLineID, CategorizedLine* in_categorizedLineRef)
{
	auto recordsBegin = records.find(in_sequenceID);
	if (recordsBegin == records.end())	// it wasn't found, so insert
	{
		//std::cout << "!!!! Record not found, inserting..." << std::endl;
		BorderLineIntersectRecord newRecord;
		records[in_sequenceID] = newRecord;
	}

	records[in_sequenceID].insertCategorizedLineRef(in_categorizedLineID, in_categorizedLineRef);
	//std::cout << "Inserted new record for sequenceID " << in_sequenceID << ", categorizedLineID " << in_categorizedLineID << std::endl;
}