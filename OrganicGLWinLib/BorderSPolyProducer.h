#pragma once

#ifndef BORDERSPOLYPRODUCER_H
#define BORDERSPOLYPRODUCER_H

#include "SPoly.h"
#include "MassZone.h"
#include "MassZoneBoxType.h"
#include <map>
#include "MassZoneBoxBoundaryOrientation.h"
#include <vector>
#include "DebugOption.h"
#include "PolyDebugLevel.h"
#include "MessageContainer.h"

class BorderSPolyProducer
{
	public:
		// template function for permit settings
		template<typename FirstMassZoneBoxBoundaryOrientation, typename ...RemainingMassZoneBoxBoundaryOrientation> 
		void setBoxFaceProductionPermit(FirstMassZoneBoxBoundaryOrientation && firstOption, RemainingMassZoneBoxBoundaryOrientation && ...remainingOptions)
		{
			productionMassZone.boxFaceProductionPermits.insert(std::forward<FirstMassZoneBoxBoundaryOrientation>(firstOption));
			setBoxFaceProductionPermit(std::forward<RemainingMassZoneBoxBoundaryOrientation>(remainingOptions)...);
		}
		void setBoxFaceProductionPermit() {};

		// template function for debug options
		template<typename FirstDebugOption, typename ...RemainingDebugOptions>
		void setBorderDebugOption(FirstDebugOption && firstOption, RemainingDebugOptions && ...remainingOptions)
		{
			handleBorderDebugOption(std::forward<FirstDebugOption>(firstOption));
			setBorderDebugOption(std::forward<RemainingDebugOptions>(remainingOptions)...);
		}
		void setBorderDebugOption() {};



		void addInputSPoly(SPoly in_inputSPoly);						// Step 1: add all input SPolys
		void configurePolysWithoutNormalCalcs();						// Step 2: configure empty normals
		MessageContainer produceBorderSPolys(MassZoneBoxType in_massZoneBoxType);	// Step 3: produce the border SPolys
		std::vector<SPoly> fetchAllSPolys();
	private:
		void handleBorderDebugOption(DebugOption in_debugOption);

		PolyDebugLevel printOutputSPolysDebugLevel = PolyDebugLevel::NONE;
		std::map<int, SPoly> inputSPolys;
		std::map<MassZoneBoxBoundaryOrientation, SPolySupergroup> outputSPolySuperGroups;
		int numberOfInputSPolys = 0;
		MassZone productionMassZone;
};

#endif
