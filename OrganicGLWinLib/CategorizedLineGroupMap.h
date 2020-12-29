#pragma once

#ifndef CATEGORIZEDLINEGROUPMAP_H
#define CATEGORIZEDLINEGROUPMAP_H

#include <map>
#include "IntersectionType.h"
#include "CategorizedLineGroup.h"

class CategorizedLineGroupMap
{
	public:
		void insertGroupRecord(int in_parentSPolyID, IntersectionType in_intersectionType, int in_poolIndex)
		{
			groups[in_parentSPolyID].insertRecord(in_intersectionType, in_poolIndex);
		};
		void printGroupLineCounts()
		{
			auto groupsBegin = groups.begin();
			auto groupsEnd = groups.end();
			for (; groupsBegin != groupsEnd; groupsBegin++)
			{
				std::cout << "!! Parent SPolyID " << groupsBegin->first << ", has " << groupsBegin->second.recordVector.size() << " records. " << std::endl;
				auto recordsBegin = groupsBegin->second.recordVector.begin();
				auto recordsEnd = groupsBegin->second.recordVector.end();
				for (; recordsBegin != recordsEnd; recordsBegin++)
				{
					std::cout << "! Record: ";
					if (recordsBegin->categorizedLineIntersectionType == IntersectionType::A_SLICE)
					{
						std::cout << "A_SLICE ";
					}
					else if (recordsBegin->categorizedLineIntersectionType == IntersectionType::INTERCEPTS_POINT_PRECISE)
					{
						std::cout << "INTERCEPTS_POINT_PRECISE ";
					}
					else if (recordsBegin->categorizedLineIntersectionType == IntersectionType::PARTIAL_BOUND)
					{
						std::cout << "PARTIAL_BOUND ";
					}
					else if (recordsBegin->categorizedLineIntersectionType == IntersectionType::NON_BOUND)
					{
						std::cout << "NON_BOUND ";
					}
					std::cout << " | Index in associated pool: " << recordsBegin->categorizedLineIndex << std::endl;
				}
			}
		}
		std::map<int, CategorizedLineGroup> groups;
};

#endif
