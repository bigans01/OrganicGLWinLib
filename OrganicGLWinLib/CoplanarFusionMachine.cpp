#include "stdafx.h"
#include "CoplanarFusionMachine.h"

void CoplanarFusionMachine::insertCoplanarRelationship(int in_polySetGroupID, int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef)
{
	if (in_polySetGroupID == 0)
	{
		oldSetTracker.insertCoplanarRelationship(in_trackedSPolyID, in_trackedSPolyRef, in_relatedSPolyID, in_relatedSPolyRef);
	}
	else if (in_polySetGroupID == 1)
	{
		newSetTracker.insertCoplanarRelationship(in_trackedSPolyID, in_trackedSPolyRef, in_relatedSPolyID, in_relatedSPolyRef);
	}
}

void CoplanarFusionMachine::setCoplanarFusionMachineDebugLevel(PolyDebugLevel in_debugLevel)
{
	coplanarFusionMachineLogLevel = in_debugLevel;
	coplanarFusionMachineLogger.setDebugLevel(in_debugLevel);
}

void CoplanarFusionMachine::fmInsertDOForSpecificTrackedSPoly(int in_trackedSPolyID, DebugOption in_debugOption)
{
	fushionMachineRelationshipDebugFlags[in_trackedSPolyID].handleSpecificTrackedSPolyDebugOption(in_debugOption);
}

void CoplanarFusionMachine::fmInsertDOForSpecificTrackedSPolySTriangle(int in_trackedSPolyID, int in_trackedSPolySTriangleID, DebugOption in_debugOption)
{
	fushionMachineRelationshipDebugFlags[in_trackedSPolyID].handleSpecificTrackedSPolySTriangleDebugOption(in_trackedSPolySTriangleID, in_debugOption);
}

void CoplanarFusionMachine::fmInsertDOForSpecificTrackedCutterCuttingTriangle(int in_trackedSPolyID, int in_trackedSPolyCuttingTriangleID, DebugOption in_debugOption)
{
	fushionMachineRelationshipDebugFlags[in_trackedSPolyID].handleSpecificTrackedSPolyCutterCuttingTriangleDebugOption(in_trackedSPolyCuttingTriangleID, in_debugOption);
}

void CoplanarFusionMachine::printRelationshipsInTrackers()
{
	std::cout << "(CoplanarFusionMachine): |||||||| Printing relationships of oldSetTracker: " << std::endl;
	oldSetTracker.printRelationshipData();
	std::cout << "(CoplanarFusionMachine): |||||||| Printing relationships of newSetTracker: " << std::endl;
	newSetTracker.printRelationshipData();
}

void CoplanarFusionMachine::insertSPolyGroupMapLink(int in_sPolyID, int in_groupID, SPoly* in_sPolyRef)
{
	//sPolyGroupMap[in_sPolyID] = in_groupID;
	SPolyRefAndGroup newRefAndGroup(in_groupID, in_sPolyRef);
	sPolyGroupMap[in_sPolyID] = newRefAndGroup;
}

void CoplanarFusionMachine::runAllCuttingSequenceTests()
{
	// OLD SET
	coplanarFusionMachineLogger.log("(CoplanarFusionMachine): Running sequence tests for oldSetTracker ", "\n");
	// move any debug options into the oldSetTracker; do this by checking the relationshipContainer for its tracked SPoly ID (key value of the map)
	auto oldSetFlagsBegin = oldSetTracker.relationshipContainer.begin();
	auto oldSetFlagsEnd = oldSetTracker.relationshipContainer.end();
	for (; oldSetFlagsBegin != oldSetFlagsEnd; oldSetFlagsBegin++)
	{
		auto oldExistingDOFinder = fushionMachineRelationshipDebugFlags.find(oldSetFlagsBegin->first);
		if (oldExistingDOFinder != fushionMachineRelationshipDebugFlags.end())
		{
			// move the option down to the oldSetTracker; doesn't need to be here.
			oldSetTracker.relationshipDebugFlags[oldSetFlagsBegin->first] = std::move(fushionMachineRelationshipDebugFlags[oldSetFlagsBegin->first]);	
		}
	}

	oldSetTracker.runAllCuttingSequenceTests();
	coplanarFusionMachineLogger.log("(CoplanarFusionMachine): Printing removable SPolys in the oldSetTracker...", "\n");
	if (coplanarFusionMachineLogger.isLoggingSet())
	{
		oldSetTracker.printRemovableSPolys();
	}




	// NEW SET
	coplanarFusionMachineLogger.log("(CoplanarFusionMachine): Running sequence tests for newSetTracker", "\n");
	// move any debug options into the newSetTracker; do this by checking the relationshipContainer for its tracked SPoly ID (key value of the map)
	auto newSetFlagsBegin = newSetTracker.relationshipContainer.begin();
	auto newSetFlagsEnd = newSetTracker.relationshipContainer.end();
	for (; newSetFlagsBegin != newSetFlagsEnd; newSetFlagsBegin++)
	{
		auto newExistingDOFinder = fushionMachineRelationshipDebugFlags.find(newSetFlagsBegin->first);
		if (newExistingDOFinder != fushionMachineRelationshipDebugFlags.end())
		{
			newSetTracker.relationshipDebugFlags[newSetFlagsBegin->first] = std::move(fushionMachineRelationshipDebugFlags[newSetFlagsBegin->first]);
		}
	}

	newSetTracker.runAllCuttingSequenceTests();
	coplanarFusionMachineLogger.log("(CoplanarFusionMachine): Printing removable SPolys in the newSetTracker...", "\n");
	if (coplanarFusionMachineLogger.isLoggingSet())
	{
		newSetTracker.printRemovableSPolys();
	}
}

void CoplanarFusionMachine::determineFusionType()
{
	int group0Count = 0;
	int group1Count = 0;

	// check group0 and group1 counts.
	auto sPolyGroupMapBegin = sPolyGroupMap.begin();
	auto sPolyGroupMapEnd = sPolyGroupMap.end();
	for (; sPolyGroupMapBegin != sPolyGroupMapEnd; sPolyGroupMapBegin++)
	{
		// if the SPoly is group 1 and has valid CleaveSequences, increment group0Count.
		if
		(
			(sPolyGroupMapBegin->second.groupID == 0)
			&&
			(sPolyGroupMapBegin->second.sPolyRef->cleaveMap.size() != 0)
		)
		{
			group0Count++;
		}
		else if
		(
			(sPolyGroupMapBegin->second.groupID == 1)
			&&
			(sPolyGroupMapBegin->second.sPolyRef->cleaveMap.size() != 0)
		)
		{
			group1Count++;
		}
	}

	// check the counts, to determine the enum type.
	// both are > 0
	if 
	(
		(group0Count > 0)
		&&
		(group1Count > 0)
	)
	{

	}

	// group0 is > 0
	else if
	(
		(group0Count > 0)
	)
	{
		coplanarFusionMachineLogger.log("(CoplanarFusionMachine): Type would be: KEEP_NEW_APPEND_OLD", "\n");
		resultingFusionType = CoplanarFusionType::KEEP_NEW_APPEND_OLD;
		fusedRemovables += oldSetTracker.removableSPolys;
	}

	// group1 is > 0
	else if
	(
		(group1Count > 0)
	)
	{
		coplanarFusionMachineLogger.log("(CoplanarFusionMachine): Type would be: KEEP_OLD_APPEND_NEW", "\n");
		resultingFusionType = CoplanarFusionType::KEEP_OLD_APPEND_NEW;
		fusedRemovables += newSetTracker.removableSPolys;
	}
	// otherwise, if there aren't any cleave sequences in either group, BUT there are removables in the NEW set:
	else if
	(
		(group0Count == 0)
		&&
		(group1Count == 0)
		&&
		(newSetTracker.removableSPolys.getSet().size() != 0)
	)
	{
		fusedRemovables += newSetTracker.removableSPolys;
	}

	if (coplanarFusionMachineLogger.isLoggingSet())
	{
		coplanarFusionMachineLogger.log("(CoplanarFusionMachine): ++++++++++++ Final values of fusedRemovables: ", "\n");
		auto removablesBegin = fusedRemovables.intSet.begin();
		auto removablesEnd = fusedRemovables.intSet.end();
		for (; removablesBegin != removablesEnd; removablesBegin++)
		{
			std::cout << *removablesBegin << std::endl;
		}
	}
}