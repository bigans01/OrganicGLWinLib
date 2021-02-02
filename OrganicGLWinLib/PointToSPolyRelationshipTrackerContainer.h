#pragma once

#ifndef POINTTOSPOLYRELATIONSHIPTRACKERCONTAINER_H
#define POINTTOSPOLYRELATIONSHIPTRACKERCONTAINER_H

#include "PointToSPolyRelationshipTracker.h"
#include <map>
#include "STriangle.h"
#include <iostream>
#include "OperableIntSet.h"

class PointToSPolyRelationshipTrackerContainer
{
		friend class MassZonePointClipper;
		void insertRelationshipTrackerData(glm::vec3 in_point, int in_sPolyID, int in_sTriangleID, STriangle* in_sTriangleRef);
		void printRelationshipTrackerData();
		OperableIntSet produceRelatedSPolyList();
		std::map<int, PointToSPolyRelationshipTracker> relationshipTrackerContainer;
};

#endif
