#pragma once

#ifndef CATEGORIZEDLINEGROUP_H
#define CATEGORIZEDLINEGROUP_H

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

		void removeRecord(IntersectionType in_intersectionType, int in_categorizedLineIndex)
		{
			//std::cout << "!!! record vector size: " << recordVector.size() << std::endl;
		
			int recordIndex = 0;
			int targetIndexToRemove = 0;
			bool wasTargetFound = false;
			auto recordVectorBegin = recordVector.begin();
			auto recordVectorEnd = recordVector.end();
			for (; recordVectorBegin != recordVectorEnd; recordVectorBegin++)
			{
				if
				(
					(recordVectorBegin->categorizedLineIntersectionType == in_intersectionType)
					&&
					(recordVectorBegin->categorizedLineIndex == in_categorizedLineIndex)
				)
				{
					//std::cout << "!!! Found index to remove" << std::endl;
					//std::cout << "!! Index value is: " << recordIndex;
					targetIndexToRemove = recordIndex;
					wasTargetFound = true;
				}
				recordIndex++;
			}

			//std::cout << "!!! Prior to erase. " << std::endl;
			//int preVal = 3;
			//std::cin >> preVal;


			if (wasTargetFound == true)
			{
				recordVector.erase(recordVector.begin() + targetIndexToRemove);
			}

			//std::cout << "!!! End of record removal function. " << std::endl;
			//int someVal = 3;
			//std::cin >> someVal;
		}
		std::vector<CategorizedLineInsertionRecord> recordVector;
};

#endif
