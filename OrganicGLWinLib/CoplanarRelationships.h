#pragma once

#ifndef COPLANARRELATIONSHIPS_H
#define COPLANARRELATIONSHIPS_H

#include "SPoly.h"
#include "SPolyRefMap.h"

class CoplanarRelationships
{
	public:
		int trackedPolyID = 0;
		SPoly* trackedSPolyRef = nullptr;
		SPolyRefMap relationshipMap;
		void setTrackedPolyData(int in_trackedPolyID, SPoly* in_trackedSPolyRef)
		{
			trackedPolyID = in_trackedPolyID;
			trackedSPolyRef = in_trackedSPolyRef;
		}
		void insertRelationship(int in_sPolyIndex, SPoly* in_sPolyRef)
		{
			relationshipMap.insertSPolyRef(in_sPolyIndex, in_sPolyRef);
		}
		void rotateToXYPlaneAndCompare()
		{
			// step 1: rotate involved SPolys to the XY plane


			// step 2: check if its MassManipulationMode::CREATION or DESTRUCTION.
			//		if CREATION -> use CoplanarMassCreator
			//		if DESTRUCTION -> use CoplanarMassDestroyer

		}
};

#endif
