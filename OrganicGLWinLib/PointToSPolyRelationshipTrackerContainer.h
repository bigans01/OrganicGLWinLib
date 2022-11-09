#pragma once

#ifndef POINTTOSPOLYRELATIONSHIPTRACKERCONTAINER_H
#define POINTTOSPOLYRELATIONSHIPTRACKERCONTAINER_H

#include "PointToSPolyRelationshipTracker.h"
#include "STriangle.h"
#include "QuatUtils.h"
#include "SPoly.h"
#include "PointToMassTrial.h"

class PointToSPolyRelationshipTrackerContainer
{
		friend class MassZonePointClipper;
		std::map<int, PointToSPolyRelationshipTracker> relationshipTrackerContainer;
		void insertRelationshipTrackerData(glm::vec3 in_point, int in_sPolyID, int in_sTriangleID, STriangle* in_sTriangleRef, glm::vec3 in_relatedSPolyEmptyNormal);
		void printRelationshipTrackerData();
		OperableIntSet produceRelatedSPolyList();
		bool checkForAnyPointsWithSingleSPoly();
		PointToSPolyRelationshipTracker* fetchSpecificSPolyRelationshipTrackerByPoint(glm::vec3 in_pointToFind);
		void removePointsExistingOutsideOfShell(PolyDebugLevel in_polyDebugLevel);
		bool runPointsAgainstShellSlices(PolyDebugLevel in_polyDebugLevel, std::map<int, SPoly*> in_clippingShellMapCopy);
};

#endif
