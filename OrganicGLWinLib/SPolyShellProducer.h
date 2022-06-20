#pragma once

#ifndef BORDERSPOLYPRODUCER_H
#define BORDERSPOLYPRODUCER_H

#include "SPoly.h"
#include "MassZone.h"
#include "MassZoneBoxType.h"
#include <map>
#include "BoundaryOrientation.h"
#include <vector>
#include "DebugOption.h"
#include "PolyDebugLevel.h"
#include "MessageContainer.h"

class SPolyShellProducer
{
	/*
	
	The purpose of this class is to produce a resulting "shell" that consists of the input SPolys (added via the function addInputsPoly),
	and the shell-spawned SPolys -- which are SPolys that occupy an entire face, and are put into outputSPolySuperGroups. In some cases,
	an appropriate SPoly cannot be produced due to bad CleaveSequence generation; in this case, an attempt to fix the issue should be performed.

	If a resolution to the fix can't be performed, the resulting SPoly to use for the corresponding BoundaryOrientation/SPolySupergroup pairing
	should be an SPoly that occupys that entire face (i.e, for POS_X face this would be a square polygon with points (4,0,0) (4,4,0) (4,4,4) and (4,0,4)
	The fix attempt needs to run in code that is called by productionMassZone.createMassZoneShell.

	*/

	public:
		// template function for permit settings
		template<typename FirstBoundaryOrientation, typename ...RemainingBoundaryOrientation> 
		void setBoxFaceProductionPermit(FirstBoundaryOrientation && firstOption, RemainingBoundaryOrientation && ...remainingOptions)
		{
			productionMassZone.boxFaceProductionPermits.insert(std::forward<FirstBoundaryOrientation>(firstOption));
			setBoxFaceProductionPermit(std::forward<RemainingBoundaryOrientation>(remainingOptions)...);
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



		void addInputSPoly(SPoly in_inputSPoly);	// Step 1: add all input SPolys
		void configurePolysWithoutNormalCalcs();	// Step 2: configure empty normals
		MessageContainer runSPolyShellConstruction(MassZoneBoxType in_massZoneBoxType);	// Step 3: produce the SPolys that constitute the shell; the MessageContainer
																					// return value should contain messages (and associated metadata) for production failures
																					// for any of supergroups specified in the outputSPolySuperGroups class member. 
																					// The call to productionMassZone.createMassZoneShell should be done in order to attempt
																					// a "fix" to malformed CleaveSequences; if it cannot be fixed, an SPoly that constitutes the entire face
																					// should be returned as a safety measure (although this isn't pretty, it's necessary).
		std::vector<SPoly> fetchAllSPolys();
	private:
		void handleBorderDebugOption(DebugOption in_debugOption);

		PolyDebugLevel printOutputSPolysDebugLevel = PolyDebugLevel::NONE;
		std::map<int, SPoly> inputSPolys;
		std::map<BoundaryOrientation, SPolySupergroup> outputSPolySuperGroups;
		int numberOfInputSPolys = 0;
		MassZone productionMassZone;
};

#endif
