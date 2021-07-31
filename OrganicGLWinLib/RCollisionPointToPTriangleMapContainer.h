#pragma once

#ifndef RCOLLISIONPOINTTOPTRIANGLEMAPCONTAINER_H
#define RCOLLISIONPOINTTOPTRIANGLEMAPCONTAINER_H

#include "RCollisionPointToPTriangleMapper.h"
#include "PTriangleMapperUpdate.h"

class RCollisionPointToPTriangleMapContainer
{
	public:
		void insertMapperUpdate(PTriangleMapperUpdate in_mapperUpdate)
		{
			// first, check to see if it exists already.
			bool doesAlreadyExist = false;
			int targetFoundIndex = 0;
			auto mapperMapBegin = triangleMapperMap.begin();
			auto mapperMapEnd = triangleMapperMap.end();
			for (; mapperMapBegin != mapperMapEnd; mapperMapBegin++)
			{
				if (mapperMapBegin->second.fetchMapperCollisinPoint()->originalValue == in_mapperUpdate.mapperUpdateCollisionPointRef->originalValue)
				{
					targetFoundIndex = mapperMapBegin->first;
					doesAlreadyExist = true;
				}
			}

			if (doesAlreadyExist == true)
			{
				triangleMapperMap[targetFoundIndex].insertPTriangleToPointMapping(in_mapperUpdate.targetPTriangleID, in_mapperUpdate.targetPointInPTriangle);
			}
			else if (doesAlreadyExist == false)
			{
				//triangleMapperMap[triangleMapperMapCurrentIndex++] = in_mapperUpdate;
				RCollisionPointToPTriangleMapper newMapperEntry(in_mapperUpdate.mapperUpdateCollisionPointRef);
				triangleMapperMap[triangleMapperMapCurrentIndex] = newMapperEntry;
				triangleMapperMap[triangleMapperMapCurrentIndex].insertPTriangleToPointMapping(in_mapperUpdate.targetPTriangleID, in_mapperUpdate.targetPointInPTriangle);
				triangleMapperMapCurrentIndex++;
			}


		};

		void printMapperStats()
		{
			auto printBegin = triangleMapperMap.begin();
			auto printEnd = triangleMapperMap.end();
			for (; printBegin != printEnd; printBegin++)
			{
				std::cout << "|| Stats for point: " << printBegin->second.fetchMapperCollisinPoint()->originalValue.x << ", " <<
													printBegin->second.fetchMapperCollisinPoint()->originalValue.y << ", " <<
													printBegin->second.fetchMapperCollisinPoint()->originalValue.z << std::endl;
				auto currentPointMapBegin = printBegin->second.pTrianglePointMapper.begin();
				auto currentPointMapEnd = printBegin->second.pTrianglePointMapper.end();
				for (; currentPointMapBegin != currentPointMapEnd; currentPointMapBegin++)
				{
					std::cout << "Found Map to PTriangle with ID " << currentPointMapBegin->first << "; index point is: " << currentPointMapBegin->second << std::endl;
				}
			}
		}

	private:
		int triangleMapperMapCurrentIndex = 0;
		std::map<int, RCollisionPointToPTriangleMapper> triangleMapperMap;
};

#endif
