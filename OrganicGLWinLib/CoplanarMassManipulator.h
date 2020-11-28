#pragma once

#ifndef COPLANARMASSMANIPULATOR
#define COPLANARMASSMANIPULATOR

#include "SPoly.h"
#include "SPolyRefMap.h"
#include "QuatRotationPoints.h"
#include <map>

class CoplanarMassManipulator
{
	public:
		void initialize(SPoly* in_trackedSPolyRef, SPolyRefMap in_sPolyRefMap, QuatRotationPoints* in_coplanarPointsRef);
	private:
		SPoly* trackedSPolyRef = nullptr;
		SPolyRefMap sPolyRefMap;
		QuatRotationPoints* coplanarPointsRef = nullptr;
};

#endif
