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
	private:
		std::map<int, PointToSTriangleRelationship> sTriangleRelationshipMap;
};

#endif
