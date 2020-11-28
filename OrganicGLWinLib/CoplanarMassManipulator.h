#pragma once

#ifndef COPLANARMASSMANIPULATOR
#define COPLANARMASSMANIPULATOR

#include "SPoly.h"
#include "SPolyRefMap.h"
#include <map>

class CoplanarMassManipulator
{
	public:
		void initialize(SPoly* in_trackedSPolyRef, SPolyRefMap in_sPolyRefMap);
	private:
		SPoly* trackedSPolyRef = nullptr;
		SPolyRefMap sPolyRefMap;
};

#endif
