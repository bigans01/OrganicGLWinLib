#pragma once

#ifndef POINTTOSPOLYRELATIONSHIPTRACKER_H
#define POINTTOSPOLYRELATIONSHIPTRACKER_H

#include "PointToSPolyRelationship.h"
#include "STriangle.h"

class PointToSPolyRelationshipTracker
{
	public:
		void insertPointToSPolyRelationship(int in_sPolyID, int in_sTriangleID, STriangle* in_sTriangleRef, glm::vec3 in_sPolyEmptyNormal)
		{
			relationships[in_sPolyID].insertNewSTriangleRelationship(in_sTriangleID, in_sTriangleRef, in_sPolyEmptyNormal);
		}
		OperableIntSet generateSPolyIDSet()
		{
			OperableIntSet returnSet;
			auto relationshipsBegin = relationships.begin();
			auto relationshipsEnd = relationships.end();
			for (; relationshipsBegin != relationshipsEnd; relationshipsBegin++)
			{
				returnSet.intSet.insert(relationshipsBegin->first);
			}
			return returnSet;
		}
		glm::vec3 point;
		std::map<int, PointToSPolyRelationship> relationships;
};

#endif
