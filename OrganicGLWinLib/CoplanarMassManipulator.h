#pragma once

#ifndef COPLANARMASSMANIPULATOR
#define COPLANARMASSMANIPULATOR

#include "SPoly.h"
#include "SPolyRefMap.h"
#include "QuatRotationPoints.h"
#include "CoplanarCategorizedLineProducer.h"
#include <map>

class CoplanarMassManipulator
{
	public:
		void initialize(SPoly* in_trackedSPolyRef, SPolyRefMap in_sPolyRefMap, QuatRotationPoints* in_coplanarPointsRef);
		virtual void runMassManipulation() = 0;
	
	// private:
		SPoly* trackedSPolyRef = nullptr;
		SPolyRefMap sPolyRefMap;
		QuatRotationPoints* coplanarPointsRef = nullptr;

		float calculateSPolyArea(SPoly* in_sPolyRef);
		float calculateTriangleArea(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
};

#endif
