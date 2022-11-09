#pragma once

#ifndef COPLANARMASSMANIPULATOR
#define COPLANARMASSMANIPULATOR

#include "SPoly.h"
#include "SPolyRefMap.h"
#include "QuatRotationPoints.h"
#include "CoplanarCategorizedLineProducer.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class CoplanarMassManipulator
{
	public:
		//void initialize(SPoly* in_trackedSPolyRef, SPolyRefMap in_sPolyRefMap, QuatRotationPoints* in_coplanarPointsRef);
		void initialize(SPoly in_trackedSPolyRef, SPolyRefMap in_sPolyRefMap, QuatRotationPoints* in_coplanarPointsRef, PolyDebugLevel in_polyDebugLevel);
		virtual void runMassManipulation() = 0;
	
	// private:
		//SPoly* trackedSPolyRef = nullptr;
		SPoly trackedSPolyRef;
		SPolyRefMap sPolyRefMap;
		QuatRotationPoints* coplanarPointsRef = nullptr;
		PolyLogger massManipulatorLogger;
		PolyDebugLevel massManipulatorLogLevel = PolyDebugLevel::NONE;

		float calculateSPolyArea(SPoly* in_sPolyRef);
		float calculateTriangleArea(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
		double calculateTriangleAreaDouble(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
};

#endif
