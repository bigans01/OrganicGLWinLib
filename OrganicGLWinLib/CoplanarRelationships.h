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
#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "CoplanarRelationshipDebugFlags.h"
#include "STriangleCutter.h"
#include "DebugOptionSet.h"

class CoplanarRelationships
{
	public:
		CoplanarRelationships() {};
		CoplanarRelationships& operator=(const CoplanarRelationships& in_coplanarRelationshipsB)
		{
			trackedPolyID = in_coplanarRelationshipsB.trackedPolyID;
			trackedSPoly = in_coplanarRelationshipsB.trackedSPoly;
			relationshipMap = in_coplanarRelationshipsB.relationshipMap;
			//rotationManager = in_coplanarRelationshipsB.rotationManager;
			//coplanarPoints = in_coplanarRelationshipsB.coplanarPoints;
			//pointTranslator = in_coplanarRelationshipsB.pointTranslator;
			return *this;
		}
	private:
		friend class CoplanarRelationshipTracker;

		std::unique_ptr<CoplanarMassManipulator> manipulator;		// for either CREATION or DESTRUCTION modes; flagged as deprecated (2/25/2021)
		int trackedPolyID = 0;
		SPoly trackedSPoly;
		SPolyRefMap relationshipMap;
		QuatRotationManager rotationManager;
		QuatRotationPoints coplanarPoints;
		PointTranslationCheck pointTranslator;
		PolyDebugLevel relationshipsDebugLevel = PolyDebugLevel::NONE;
		PolyDebugLevel dlPrintBorderLines = PolyDebugLevel::NONE;	// for printing out the border lines in the tracked and related SPolys.
		PolyLogger relationshipsLogger;
		std::map<int, DebugOptionSet> specificTrackedSTriangleDOSForCutter;
		std::map<int, DebugOptionSet> specificTrackedSPolyCutterCuttingDOS;

		void setTrackedPolyData(int in_trackedPolyID, SPoly in_trackedSPolyRef);
		void insertRelationship(int in_sPolyIndex, SPoly in_sPolyRef);
		bool rotateToXYPlaneAndRunCuttingSequenceTests();
		void setLoggerDebugLevel(PolyDebugLevel in_polyDebugLevel);
		bool performCuttingSequenceTest();
		void applyCoplanarRelationshipDebugOptions(CoplanarRelationshipDebugFlags* in_coplanarRelationshipsDebugFlagsRef);
		DebugOptionSet acquireDOSForSpecificTrackedSTriangle(int in_sTriangleID);

};

#endif
