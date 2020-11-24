#pragma once

#ifndef COPLANARRELATIONSHIPS_H
#define COPLANARRELATIONSHIPS_H

#include "SPoly.h"
#include "SPolyRefMap.h"
#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"

class CoplanarRelationships
{
	public:
		int trackedPolyID = 0;
		SPoly* trackedSPolyRef = nullptr;
		SPolyRefMap relationshipMap;
		QuatRotationPoints coplanarPoints;
		PointTranslationCheck pointTranslator;
		void setTrackedPolyData(int in_trackedPolyID, SPoly* in_trackedSPolyRef);
		void insertRelationship(int in_sPolyIndex, SPoly* in_sPolyRef);
		void rotateToXYPlaneAndCompare();
};

#endif
