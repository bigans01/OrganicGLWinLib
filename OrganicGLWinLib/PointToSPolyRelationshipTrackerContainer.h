#pragma once

#ifndef POINTTOSPOLYRELATIONSHIPTRACKERCONTAINER_H
#define POINTTOSPOLYRELATIONSHIPTRACKERCONTAINER_H

#include "PointToSPolyRelationshipTracker.h"
#include <map>
#include "STriangle.h"
#include <iostream>
#include "OperableIntSet.h"
#include <glm/glm.hpp>

class PointToSPolyRelationshipTrackerContainer
{
		friend class MassZonePointClipper;
		std::map<int, PointToSPolyRelationshipTracker> relationshipTrackerContainer;
		void insertRelationshipTrackerData(glm::vec3 in_point, int in_sPolyID, int in_sTriangleID, STriangle* in_sTriangleRef);
		void printRelationshipTrackerData();
		OperableIntSet produceRelatedSPolyList();
		bool checkForAnyPointsWithSingleSPoly();
		PointToSPolyRelationshipTracker* fetchSpecificSPolyRelationshipTrackerByPoint(glm::vec3 in_pointToFind);
};

#endif
