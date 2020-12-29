#pragma once

#ifndef COPLANARRELATIONSHIPS_H
#define COPLANARRELATIONSHIPS_H

#include "SPoly.h"
#include "SPolyRefMap.h"
#include "QuatRotationManager.h"
#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include "CoplanarMassManipulator.h"
#include "CoplanarMassCreator.h"
#include <mutex>

class CoplanarRelationships
{
	public:
		//CoplanarRelationships() {};

		
		CoplanarRelationships();
		
		CoplanarRelationships& operator=(const CoplanarRelationships& in_coplanarRelationshipsB)
		{
			trackedPolyID = in_coplanarRelationshipsB.trackedPolyID;
			trackedSPolyRef = in_coplanarRelationshipsB.trackedSPolyRef;
			relationshipMap = in_coplanarRelationshipsB.relationshipMap;
			//rotationManager = in_coplanarRelationshipsB.rotationManager;
			//coplanarPoints = in_coplanarRelationshipsB.coplanarPoints;
			//pointTranslator = in_coplanarRelationshipsB.pointTranslator;
			return *this;
		}
		

		std::unique_ptr<CoplanarMassManipulator> manipulator;		// for either CREATION or DESTRUCTION modes
		int trackedPolyID = 0;
		//SPoly* trackedSPolyRef = nullptr;
		SPoly trackedSPolyRef;
		SPolyRefMap relationshipMap;
		QuatRotationManager rotationManager;
		QuatRotationPoints coplanarPoints;
		PointTranslationCheck pointTranslator;
		//void setTrackedPolyData(int in_trackedPolyID, SPoly* in_trackedSPolyRef);
		void setTrackedPolyData(int in_trackedPolyID, SPoly in_trackedSPolyRef);
		//void insertRelationship(int in_sPolyIndex, SPoly* in_sPolyRef);
		void insertRelationship(int in_sPolyIndex, SPoly in_sPolyRef);
		void rotateToXYPlaneAndCompare();
};

#endif
