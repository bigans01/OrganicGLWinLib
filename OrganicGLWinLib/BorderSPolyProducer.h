#pragma once

#ifndef BORDERSPOLYPRODUCER_H
#define BORDERSPOLYPRODUCER_H

#include "SPoly.h"
#include "MassZone.h"
#include "MassZoneBoxType.h"
#include <map>
#include "MassZoneBoxBoundaryOrientation.h"

class BorderSPolyProducer
{
	public:
		template<typename FirstMassZoneBoxBoundaryOrientation, typename ...RemainingMassZoneBoxBoundaryOrientation> 
		void setBoxFaceProductionPermit(FirstMassZoneBoxBoundaryOrientation && firstOption, RemainingMassZoneBoxBoundaryOrientation && ...remainingOptions)
		{
			productionMassZone.boxFaceProductionPermits.insert(std::forward<FirstMassZoneBoxBoundaryOrientation>(firstOption));
			setBoxFaceProductionPermit(std::forward<RemainingMassZoneBoxBoundaryOrientation>(remainingOptions));
		}
		void setBoxFaceProductionPermit() {};

		void addInputSPoly(SPoly in_inputSPoly);						// Step 1: add all input SPolys
		void configurePolysWithoutNormalCalcs();						// Step 2: configure empty normals
		void produceBorderSPolys(MassZoneBoxType in_massZoneBoxType);	// Step 3: produce the border SPolys
	private:
		std::map<int, SPoly> inputSPolys;
		std::map<MassZoneBoxBoundaryOrientation, SPolySupergroup> outputSPolySuperGroups;
		int numberOfInputSPolys = 0;
		MassZone productionMassZone;
};

#endif
