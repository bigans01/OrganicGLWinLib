#pragma once

#ifndef POINTTOSPOLYRELATIONSHIPTRACKER_H
#define POINTTOSPOLYRELATIONSHIPTRACKER_H

#include <glm/glm.hpp>
#include <map>
#include "PointToSPolyRelationship.h"
#include "STriangle.h"
#include "OperableIntSet.h"

class PointToSPolyRelationshipTracker
{
	public:
		void insertPointToSPolyRelationship(int in_sPolyID, int in_sTriangleID, STriangle* in_sTriangleRef)
		{
			relationships[in_sPolyID].insertNewSTriangleRelationship(in_sTriangleID, in_sTriangleRef);
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
