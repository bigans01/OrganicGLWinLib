#pragma once

#ifndef POINTTOSPOLYRELATIONSHIP_H
#define POINTTOSPOLYRELATIONSHIP_H

#include "STriangle.h"
#include "PointToSTriangleRelationship.h"
#include <map>

class PointToSPolyRelationship
{
	public:
		void insertNewSTriangleRelationship(int in_sTriangleIndex, STriangle* in_sTriangleRef)
		{
			PointToSTriangleRelationship relationship(in_sTriangleIndex, in_sTriangleRef);
			sTriangleRelationshipMap[in_sTriangleIndex] = relationship;
		}
		void printSTriangleIndicesAndData()
		{
			auto relationshipMapBegin = sTriangleRelationshipMap.begin();
			auto relationshipMapEnd = sTriangleRelationshipMap.end();
			for (; relationshipMapBegin != relationshipMapEnd; relationshipMapBegin++)
			{
				std::cout << "[" << relationshipMapBegin->first << "] -> points: ";
				for (int x = 0; x < 3; x++)
				{
					std::cout << relationshipMapBegin->second.sTriangleRef->triangleLines[x].pointA.x << ", "
						<< relationshipMapBegin->second.sTriangleRef->triangleLines[x].pointA.y << ", "
						<< relationshipMapBegin->second.sTriangleRef->triangleLines[x].pointA.z << " | ";
				}
				std::cout << std::endl;
			}
		}
	private:
		std::map<int, PointToSTriangleRelationship> sTriangleRelationshipMap;
};

#endif
