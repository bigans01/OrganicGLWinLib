#pragma once

#ifndef BORDERSPOLYPRODUCER_H
#define BORDERSPOLYPRODUCER_H

#include "SPoly.h"
#include "MassZone.h"
#include "MassZoneBoxType.h"
#include "DebugOption.h"
#include "PolyDebugLevel.h"
#include "MessageContainer.h"
#include "SPolyUtils.h"
#include "ExceptionRecorder.h"

class SPolyShellProducer
{
	/*

	Description:
	
	The purpose of this class is to produce a resulting "shell" that consists of the input SPolys (added via the function addInputsPoly),
	and the shell-spawned SPolys -- which are SPolys that occupy an entire face, and are put into outputSPolySuperGroups. In some cases,
	an appropriate SPoly cannot be produced due to bad CleaveSequence generation; in this case, an attempt to fix the issue should be performed.

	There is no need to apply any special filtering logic on an SPoly that is to be used for the call to addInputSPolys; that function will analyze the BoundaryIndicator value of the
	SPoly to determine what to do with it (see the function addInputSPolys itself for the logic). This means:

	1.)	any input SPoly that has a BoundaryIndicator value of 0 will be ALWAYS be used as an input SPoly.

	2.)	existing SPolys that are added which have a BoundaryIndicator value -AND- are not a SCAB_CHILD or SCAB PARENT will cause the MassZone::createMassZoneShell to ignore
		SPoly generation on that corresponding face, and instead set the SPolySupergroup of the MassZoneBoxBoundarySPolySet to be that group of 
		SPolys (see 2022_4_002 (3) in MassZone.cpp)

	3.)	existing SPolys that are added which DO have a SCAB_CHILD or SCAB_PARENT child will get put into a specific SPolySupergroup 
		called existingScabSupergroups; at the end of the SPolyShellProducer::runSPolyShellConstruction function, the call to 
		relocateScabPolysToOutput() will put it where it needs to be. See that function for more details.

	If a resolution to the fix can't be performed, the resulting SPoly to use for the corresponding BoundaryOrientation/SPolySupergroup pairing
	should be an SPoly that occupys that entire face (i.e, for POS_X face this would be a square polygon with points (4,0,0) (4,4,0) (4,4,4) and (4,0,4)
	The fix attempt needs to run in code that is called by productionMassZone.createMassZoneShell. In this case,
	the resulting SPoly will also have to be flagged as a SCAB_PARENT.

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
		std::vector<SPoly> fetchAllSPolys();		// Fetches a vector that contains all SPolys from the following members: 
													//	-inputSPolys
													//	-outputSPolySuperGroups
													//	-existingFreeScabChildSupergroup
		void printSupergroupBoundaryIndicators();
		void printAllProducedSPolys();
		std::set<BoundaryOrientation> fetchOutputBoundaries();
		Message convertBoundarySPolySupergroupToMessage(BoundaryOrientation in_targetBoundary);
		int getNumberOfProducedSPolysInMassZoneBoundarySPoly(BoundaryOrientation in_targetBoundary);

		// debug functions
		void checkForPosZSquareBoundaryInOutput();
		ExceptionRecorder getExceptions();			// this will retrieve a copy of the ExceptionRecorder,
													// so that any recorded exceptions can be analyzed/tracked/logged 

	private:
		void handleBorderDebugOption(DebugOption in_debugOption);
		void relocateScabPolysToOutput();	// This function cycles through each SPoly in the existingScabSupergroups member (if any exist),
											// and puts them back into their corresponding SPolySupergroup that exists in the 
											// outputSPolySuperGroups member. Non-oriented SCAB_PARENT / SCAB_CHILD SPolys may need to be
											// put into their own SPolySupergroup (change pending, 10/16/2022)

		PolyDebugLevel printOutputSPolysDebugLevel = PolyDebugLevel::NONE;
		std::map<int, SPoly> inputSPolys;
		std::map<BoundaryOrientation, SPolySupergroup> outputSPolySuperGroups;
		int numberOfInputSPolys = 0;
		MassZone productionMassZone;
		ExceptionRecorder producerExceptionRecorder;	// the instance of ExceptionRecorder that is passed to various sub-components
														// contained within the instance of SPolyShellProducer.

		// exclusion sets
		std::set<BoundaryOrientation> excludedOrientations;

		// storage for already-produced legitimate boundary SPolys, that were added to the SPolyShellProducer.
		std::map<BoundaryOrientation, SPolySupergroup> existingLegitimateBoundarySupergroups;

		// Below: storage for already-produced SCAB_PARENT/SCAB_CHILD SPolys, that were added to the SPolyShellProducer;
		// they should only need one SPolySupergroup (this will change in the future). When everything else is done,
		// we will need to insert each SPoly from this supergroup back into it's respective outputSPolySupergroups map, 
		// which would be done based on the BoundaryOrientation for that poly. The relocateScabPolysToOutput() function
		// will move the SCAB_PARENT/SCAB_CHILD polys into the outputSPolySupergroups map, and should be called at the 
		// end of the 
		SPolySupergroup existingScabSupergroups;

		SPolySupergroup existingFreeScabChildSupergroup;	// this supergroup contains any SPoly that is classified as a SCAB_CHILD and is not
															// bound to any particular BoundaryOrientation; these are effectively "Free", and should
															// be put into this member, which is not tied to any BoundaryOrientation (such as POS_X)
};

#endif
