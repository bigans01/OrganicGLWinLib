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

void BorderLineIntersectRecorder::printRecords()
{
	auto recordsBegin = records.begin();
	auto recordsEnd = records.end();
	for (; recordsBegin != recordsEnd; recordsBegin++)
	{
		auto lineRefsBegin = recordsBegin->second.lineRefs.begin();
		auto lineRefsEnd = recordsBegin->second.lineRefs.end();
		for (; lineRefsBegin != lineRefsEnd; lineRefsBegin++)
		{
			std::cout << "## Printing categorized line stats: (Cleave Sequence ID: " << recordsBegin->first << ") " << std::endl;
			std::cout << "point A: " << lineRefsBegin->second->line.pointA.x << ", " << lineRefsBegin->second->line.pointA.y << ", " << lineRefsBegin->second->line.pointA.z << std::endl;
			std::cout << "point B: " << lineRefsBegin->second->line.pointB.x << ", " << lineRefsBegin->second->line.pointB.y << ", " << lineRefsBegin->second->line.pointB.z << std::endl;
			std::cout << "border line ID: " << lineRefsBegin->second->line.getBorderLineIDFromSingularBorderLineCount() << std::endl;
		}
	}
}